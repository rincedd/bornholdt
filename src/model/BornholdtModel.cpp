#include "BornholdtModel.h"
#include <boost/foreach.hpp>
#include <myrng/myrngWELL.h>

using namespace largenet;

double coupling(double input, double beta)
{
	return 1.0 / (1.0 + exp(-2.0 * beta * input));
}

BornholdtModel::BornholdtModel(Graph& g, EdgeWeights& w, Params p) :
		net_(g), weights_(w), par_(p), thresholds_(g.numberOfNodes(), 0.0), spins_(
				g.numberOfNodes(), -1)
{
	init();
}

void BornholdtModel::init()
{
	BOOST_FOREACH(Node& n, net_.nodes())
	{
		spins_[n.id()] = rng.Chance(0.5) ? UP : DOWN;
		thresholds_[n.id()] = rng.GaussianPolar(par_.mu, par_.epsilon);
	}
}

BornholdtModel::~BornholdtModel()
{
}

void BornholdtModel::step()
{
	spin_v new_spins(spins_);
	BOOST_FOREACH(Node& n, net_.nodes())
	{
		double inputs = thresholds_[n.id()];
		BOOST_FOREACH(Edge* e, n.outEdges())
		{
			inputs += weights_.weight(e->id()) * spins_[e->target()->id()];
		}
		double p = coupling(inputs, par_.beta);
		if (rng.Chance(p))
			new_spins[n.id()] = UP;
		else
			new_spins[n.id()] = DOWN;
	}
	spins_ = new_spins;
}
