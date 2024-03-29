#include <myrng/myrngWELL.h>
#include <myrng/util.h>

#include <largenet2/generators/generators.h>
#include <largenet2/generators/lattice.h>
#include <largenet2/base/converters.h>
#include <stdexcept>
#include "EvolutionController.h"
#include "model/edge_states.h"
#include "Filename.h"
#include "StepRepeater.h"
#include "SingleCorrelationObserver.h"
#include "loggers/AverageEvolutionLogger.h"
#include "loggers/SnapshotLogger.h"

using namespace std;
using namespace largenet;
using namespace myrng::util;

EvolutionController::EvolutionController(BornholdtParameters par) :
		par_(par), graph_(1, 2), weights_(0), model_(0), streams_()
{
	initRandomNumberGenerator();
}

void EvolutionController::storeRandomNumberGeneratorSeed()
{
	par_.seed = rng.getSeed();
}

void EvolutionController::initRandomNumberGenerator()
{
	if (par_.seed != 0)
		rng.seed(par_.seed);
	storeRandomNumberGeneratorSeed();
}

void EvolutionController::initEdgeStates()
{
	edge_size_t numberOfActiveEdges = static_cast<edge_size_t>(round(
			par_.average_active_connectivity * graph_.numberOfNodes()));
	if (numberOfActiveEdges > graph_.numberOfEdges())
		throw std::runtime_error(
				"Average connectivity cannot exceed average degree");
	edge_size_t initialInactiveEdges = graph_.numberOfEdges()
			- numberOfActiveEdges;
	while (graph_.numberOfEdges(INACTIVE) < initialInactiveEdges)
	{
		Graph::EdgeStateIterator e = myrng::util::random_from(
				graph_.edges(ACTIVE), rng);
		graph_.setEdgeState(e.id(), INACTIVE);
	}
}

void EvolutionController::resetEdgeStates()
{
	BOOST_FOREACH(Edge& e, graph_.edges())
	{
		graph_.setEdgeState(e.id(), ACTIVE);
	}
}

void EvolutionController::initEdges()
{
	resetEdgeStates();
	initEdgeStates();
	initEdgeWeights();
}

void EvolutionController::storeNetworkParameters()
{
	par_.average_degree = static_cast<double>(graph_.numberOfEdges())
			/ graph_.numberOfNodes();
}

void EvolutionController::createSquareLattice()
{
	size_t sideLength = static_cast<size_t>(round(sqrt(par_.num_nodes)));
	generators::mooreLattice2D(graph_, sideLength, sideLength);
	converters::toDirected(graph_);
}

void EvolutionController::createPeriodicSquareLattice()
{
	size_t sideLength = static_cast<size_t>(round(sqrt(par_.num_nodes)));
	generators::mooreLattice2DPeriodic(graph_, sideLength, sideLength);
	converters::toDirected(graph_);
}

void EvolutionController::createRandomNetwork()
{
	generators::randomGnm(graph_, par_.num_nodes,
			par_.average_degree * par_.num_nodes, rng, true);
}

void EvolutionController::initEdgeWeights()
{
	weights_.reset(
			new EdgeWeights(graph_.numberOfEdges(), graph_.numberOfNodes()));
	BOOST_FOREACH(const Edge& e, graph_.edges())
	{
		weights_->setWeight(e, rng.FromTo(-1, 1));
	}
}

void EvolutionController::initModel()
{
	BornholdtModel::Params par =
	{ par_.beta, par_.epsilon, par_.mu };
	model_.reset(new BornholdtModel(graph_, *weights_, par));
}

void EvolutionController::createNetwork()
{
	if (par_.network == "random")
		createRandomNetwork();
	else if (par_.network == "square")
		createSquareLattice();
	else if (par_.network == "periodic-square")
		createPeriodicSquareLattice();
	else
		throw std::runtime_error("Unknown network type");
}

void EvolutionController::setup()
{
	createNetwork();
	storeNetworkParameters();
	initEdges();
	initModel();
}

void EvolutionController::writeInfo(ostream& strm) const
{
	strm << "# Bornholdt/Röhl model.\n";
	strm << par_ << "\n";
}

void EvolutionController::activateEdge(const Graph::EdgeIterator& edge)
{
	graph_.setEdgeState(edge->id(), ACTIVE);
	weights_->setWeight(*edge, rng.FromTo(-1, 1));
}

void EvolutionController::deactivateEdge(const Graph::EdgeIterator& edge)
{
	graph_.setEdgeState(edge->id(), INACTIVE);
	weights_->setWeight(*edge, 0.0);
}

void EvolutionController::updateTopology(Graph::EdgeIterator edge,
		double correlation)
{
	if (abs(correlation) >= par_.alpha)
		activateEdge(edge);
	else
		deactivateEdge(edge);
}
ostream& EvolutionController::openStream(string tag)
{
	return streams_.openStream(Filename(name(), par_, tag));
}

int EvolutionController::exec()
{
	setup();

	ostream& averagesStream = openStream("averages");
	writeInfo(averagesStream);
	AverageEvolutionLogger evolutionLogger(graph_, *weights_);
	evolutionLogger.setStream(averagesStream);
	evolutionLogger.writeHeader(0);

	SnapshotLogger snapshotLogger(graph_, *weights_, *model_);
	snapshotLogger.setStream(openStream("networks"));
	snapshotLogger.writeHeader(0);

	size_t next = 0;
	for (size_t i = 0; i < par_.num_topological_updates; ++i)
	{
		StepRepeater stepper(*model_);
		stepper.makeSteps(par_.num_iterations / 2);
		Graph::EdgeIterator edge = random_from(graph_.edges(), rng);
		SingleCorrelationObserver correlationObserver(
				model_->spin(edge->source()->id()),
				model_->spin(edge->target()->id()));
		stepper.addObserver(&correlationObserver);
		stepper.makeSteps(par_.num_iterations / 2);
		evolutionLogger.log(i);
		if (i >= next)
		{
			snapshotLogger.log(i);
			next += par_.snapshot_interval;
		}
		updateTopology(edge, correlationObserver.mean());
	}
	snapshotLogger.log(par_.num_topological_updates);
	evolutionLogger.log(par_.num_topological_updates);
	return 0;
}
