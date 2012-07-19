/*
 * BornholdtModel.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTMODEL_H_
#define BORNHOLDTMODEL_H_

#include "EdgeWeights.h"
#include <largenet2.h>
#include <stdexcept>

enum NodeState
{
	DOWN = -1, UP = 1
};

class BornholdtModel
{
public:
	struct Params
	{
		double beta;	///< inverse temperature
		double epsilon; ///< threshold distribution width
		double mu; 		///< threshold distribution mean
	};

	typedef std::vector<short> spin_v;	///< vector of neuron states (+1 or -1)
	typedef std::vector<double> threshold_v;

public:
	BornholdtModel(largenet::Graph& g, EdgeWeights& w, Params p);

	size_t size() const
	{
		return spins_.size();
	}

	void resize(size_t n)
	{
		if (n != spins_.size())
			spins_.resize(n, DOWN);
	}
	void init();
	void step();
	spin_v::const_reference spin(largenet::node_id_t i) const { return spins_[i]; }
	spin_v::reference spin(largenet::node_id_t i) { return spins_[i]; }

private:
	largenet::Graph& net_;
	EdgeWeights& weights_;
	Params par_;
	threshold_v thresholds_;
	spin_v spins_;
};

#endif /* BORNHOLDTMODEL_H_ */
