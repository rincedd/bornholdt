#include "BornholdtModel.h"
#include <boost/foreach.hpp>
#include <myrng/myrngWELL.h>

using namespace largenet;

double coupling(double input, double beta)
{
	return 1.0 / (1.0 + exp(-2.0 * beta * input));
}

BornholdtModel::BornholdtModel(Graph& g, EdgeWeights& w, Params p) :
		net_(g), weights_(w), par_(p), thresholds_(g.numberOfNodes(), 0.0), states_(
				g.numberOfNodes(), 0), outputs_(g.numberOfNodes(), QUIET)
{
	init();
}

void BornholdtModel::initOutputs()
{
	BOOST_FOREACH(short& output, outputs_)
	{
		output = rng.Chance(0.5) ? SPIKING : QUIET;
	}
}

void BornholdtModel::initThresholds()
{
	BOOST_FOREACH(double& threshold, thresholds_)
	{
		threshold = rng.GaussianPolar(par_.mu, par_.epsilon);
	}
}

void BornholdtModel::initSwitchInfo()
{
	spikeInfo_.assign(outputs_.size(), false);
}

void BornholdtModel::init()
{
	initOutputs();
	initThresholds();
	initSwitchInfo();
}

double BornholdtModel::computeInputs(const Node& n) const
{
	double inputs = 0;
	BOOST_FOREACH(Edge* e, n.outEdges())
	{
		inputs += weights_.weight(e->id()) * outputs_[e->target()->id()];
	}
	return inputs;
}

void BornholdtModel::step()
{
	spike_v new_outputs(outputs_);
	BOOST_FOREACH(Node& n, net_.nodes())
	{
		states_[n.id()] += computeInputs(n);
		double p = coupling(states_[n.id()] + thresholds_[n.id()], par_.beta);
		if (rng.Chance(p))
		{
			new_outputs[n.id()] = SPIKING;
			spikeInfo_[n.id()] = true;
			states_[n.id()] = 0;
		}
		else
			new_outputs[n.id()] = QUIET;
	}
	outputs_ = new_outputs;
}
