/*
 * EvolutionController.cpp
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#include <myrng/myrngWELL.h>
#include <myrng/util.h>

#include "EvolutionController.h"
#include <largenet2/generators/generators.h>
#include "loggers/AverageEvolutionLogger.h"
#include "Observer.h"
#include "CorrelationObserver.h"
#include "Filename.h"

using namespace std;
using namespace largenet;
using largenet::generators::util::random_from;

EvolutionController::EvolutionController(BornholdtParameters par) :
		par_(par), graph_(1, 1), weights_(0), model_(0), streams_()
{
	if (par_.seed != 0)
		rng.seed(par_.seed);
	graph_.setElementFactory(
			auto_ptr<ElementFactory>(new MultiEdgeElementFactory));
}

void EvolutionController::setup()
{
	generators::randomGnm(graph_, par_.num_nodes,
			par_.average_degree * par_.num_nodes, rng, true);

	/// TODO use some logger/logging framework for this
	cout << "ER network with N = " << graph_.numberOfNodes() << " and L = "
			<< graph_.numberOfEdges() << " (<k> = "
			<< graph_.numberOfEdges() / graph_.numberOfNodes() << ").\n";

	weights_.reset(
			new EdgeWeights(graph_.numberOfEdges(), graph_.numberOfNodes()));
	BOOST_FOREACH(const Edge& e, graph_.edges())
	{
		weights_->setWeight(e, rng.FromTo(-1, 1));
	}
	BornholdtModel::Params par =
	{ par_.beta, par_.epsilon, par_.mu };
	model_.reset(new BornholdtModel(graph_, *weights_, par));
}

void EvolutionController::writeInfo(ostream& strm) const
{
	strm << "# Bornholdt/Röhl model on ER network.\n";
	strm << par_ << "\n";
}

void EvolutionController::updateTopology(Graph::EdgeIterator edge,
		double correlation)
{
	if (abs(correlation) > par_.alpha)
	{
		// add new link
		edge_id_t eid = graph_.addEdge(edge->source()->id(),
				edge->target()->id(), true);
		weights_->setWeight(*graph_.edge(eid), rng.FromTo(-1, 1));
	}
	else
	{
		graph_.removeEdge(edge->id());
	}
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

int EvolutionController::exec()
{
	setup();

	ostream& averages_stream = streams_.openStream(Filename(name(), par_, "averages"));
	writeInfo(averages_stream);
	AverageEvolutionLogger ael(graph_, *weights_);
	ael.setStream(averages_stream);
	ael.writeHeader(0);
	for (size_t i = 0; i < par_.num_topological_updates; ++i)
	{
		iterate(par_.num_iterations / 2);
		Graph::EdgeIterator edge = myrng::util::random_from(graph_.edges(),
				rng);
		CorrelationObserver corr_obs(model_->spin(edge->source()->id()),
				model_->spin(edge->target()->id()));
		iterate(par_.num_iterations / 2, &corr_obs);
		ael.log(i);
		updateTopology(edge, corr_obs.mean());
	}
	ael.log(par_.num_topological_updates);
	return 0;
}
