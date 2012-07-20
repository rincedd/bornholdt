#include <myrng/myrngWELL.h>
#include <myrng/util.h>

#include "EvolutionController.h"
#include <largenet2/generators/generators.h>
#include "loggers/AverageEvolutionLogger.h"
#include "Observer.h"
#include "CorrelationObserver.h"
#include "Filename.h"
#include "model/edge_states.h"
#include "loggers/SnapshotLogger.h"

using namespace std;
using namespace largenet;
using largenet::generators::util::random_from;

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

void EvolutionController::createRandomNetwork()
{
	generators::randomGnm(graph_, par_.num_nodes,
			par_.average_degree * par_.num_nodes, rng, true);
	BOOST_FOREACH(Edge& e, graph_.edges())
	{
		graph_.setEdgeState(e.id(), ACTIVE);
	}
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
	initEdgeWeights();
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

void EvolutionController::iterate(unsigned long steps, Observer* obs)
{
	for (unsigned long i = 0; i < steps; ++i)
	{
		model_->step();
		if (obs != 0)
			obs->notify();
	}
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
		iterate(par_.num_iterations / 2);
		Graph::EdgeIterator edge = myrng::util::random_from(graph_.edges(),
				rng);
		CorrelationObserver corr_obs(model_->spin(edge->source()->id()),
				model_->spin(edge->target()->id()));
		iterate(par_.num_iterations / 2, &corr_obs);
		ael.log(i);
		if (i >= next)
		{
			snapshot_logger.log(i);
			next += par_.snapshot_interval;
		}
		updateTopology(edge, corr_obs.mean());
	}
	ael.log(par_.num_topological_updates);
	return 0;
}
