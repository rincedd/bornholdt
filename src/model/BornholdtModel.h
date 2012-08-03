/*
 * BornholdtModel.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTMODEL_H_
#define BORNHOLDTMODEL_H_

#include "../Stepper.h"
#include "EdgeWeights.h"
#include <largenet2.h>
#include <stdexcept>
#include <algorithm>
#include <functional>

enum NodeState
{
	DOWN = -1, UP = 1
};

class BornholdtModel: public Stepper
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
	typedef std::vector<bool> switch_info_v;

public:
	BornholdtModel(largenet::Graph& g, EdgeWeights& w, Params p);

	size_t size() const
	{
		return spins_.size();
	}

	void resize(size_t n)
	{
		if (n != spins_.size())
		{
			spins_.resize(n);
			thresholds_.resize(n);
			switchInfo_.resize(n);
		}
		init();
	}
	void init();
	void initThresholds();
	void step();
	spin_v::const_reference spin(largenet::node_id_t i) const
	{
		return spins_[i];
	}
	spin_v::reference spin(largenet::node_id_t i)
	{
		return spins_[i];
	}
	threshold_v::const_reference threshold(largenet::node_id_t i) const
	{
		return thresholds_[i];
	}
	threshold_v::reference threshold(largenet::node_id_t i)
	{
		return thresholds_[i];
	}

	size_t numberOfNodesThatSwitched() const
	{
		return std::count_if(switchInfo_.begin(), switchInfo_.end(),
				std::bind1st(std::equal_to<bool>(), true));
	}

private:
	largenet::Graph& net_;
	EdgeWeights& weights_;
	Params par_;
	threshold_v thresholds_;
	spin_v spins_;
	switch_info_v switchInfo_;

	void initSpins();
	double computeInputs(const largenet::Node& n) const;
	void initSwitchInfo();
};

#endif /* BORNHOLDTMODEL_H_ */
