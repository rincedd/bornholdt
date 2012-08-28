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
	QUIET = 0, SPIKING = 1
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

	typedef std::vector<double> state_v;
	typedef std::vector<short> spike_v;	///< vector of neuron outputs (quiet or spiking)
	typedef std::vector<double> threshold_v;
	typedef std::vector<bool> switch_info_v;

public:
	BornholdtModel(largenet::Graph& g, EdgeWeights& w, Params p);

	size_t size() const
	{
		return outputs_.size();
	}

	void resize(size_t n)
	{
		if (n != outputs_.size())
		{
			states_.resize(n);
			outputs_.resize(n);
			thresholds_.resize(n);
			spikeInfo_.resize(n);
		}
		init();
	}
	void init();
	void initThresholds();
	void step();
	spike_v::const_reference output(largenet::node_id_t i) const
	{
		return outputs_[i];
	}
	spike_v::reference output(largenet::node_id_t i)
	{
		return outputs_[i];
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
		return std::count_if(spikeInfo_.begin(), spikeInfo_.end(),
				std::bind1st(std::equal_to<bool>(), true));
	}

private:
	largenet::Graph& net_;
	EdgeWeights& weights_;
	Params par_;
	threshold_v thresholds_;
	state_v states_;
	spike_v outputs_;
	switch_info_v spikeInfo_;

	void initOutputs();
	double computeInputs(const largenet::Node& n) const;
	void initSwitchInfo();
};

#endif /* BORNHOLDTMODEL_H_ */
