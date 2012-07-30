#include <myrng/myrngWELL.h>
#include <myrng/util.h>

#include "EvolutionController.h"
#include <largenet2/generators/generators.h>
#include <largenet2/generators/lattice.h>
#include <largenet2/base/converters.h>
#include "loggers/AverageEvolutionLogger.h"
#include "StepRepeater.h"
#include "SingleCorrelationObserver.h"
#include "Filename.h"
#include "model/edge_states.h"
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
	edge_size_t initialInactiveEdges = static_cast<edge_size_t>(round(
			(1.0 - par_.average_active_connectivity) * graph_.numberOfNodes()));
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

void EvolutionController::createSquareLattice()
{
	size_t side_length = static_cast<size_t>(round(sqrt(par_.num_nodes)));
	generators::mooreLattice2DPeriodic(graph_, side_length, side_length);
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

void EvolutionController::setup()
{
	createRandomNetwork();
	initEdges();
	initModel();
}

void EvolutionController::writeInfo(ostream& strm) const
{
	strm << "# Bornholdt/Röhl model on ER network.\n";
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
	if (abs(correlation) > par_.alpha)
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

	ostream& averages_stream = openStream("averages");
	writeInfo(averages_stream);
	AverageEvolutionLogger ael(graph_, *weights_);
	ael.setStream(averages_stream);
	ael.writeHeader(0);

	SnapshotLogger snapshot_logger(graph_, *weights_, *model_);
	snapshot_logger.setStream(openStream("networks"));
	snapshot_logger.writeHeader(0);

	size_t next = 0;
	for (size_t i = 0; i < par_.num_topological_updates; ++i)
	{
		StepRepeater stepper(*model_);
		stepper.makeSteps(par_.num_iterations / 2);
		Graph::EdgeIterator edge = random_from(graph_.edges(), rng);
		SingleCorrelationObserver corr_obs(model_->spin(edge->source()->id()),
				model_->spin(edge->target()->id()));
		stepper.addObserver(&corr_obs);
		stepper.makeSteps(par_.num_iterations / 2);
		ael.log(i);
		if (i >= next)
		{
			snapshot_logger.log(i);
			next += par_.snapshot_interval;
		}
		updateTopology(edge, corr_obs.mean());
	}
	snapshot_logger.log(par_.num_topological_updates);
	ael.log(par_.num_topological_updates);
	return 0;
}
