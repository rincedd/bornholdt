/*
 * BornholdtModel.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTMODEL_H_
#define BORNHOLDTMODEL_H_

#include <largenet2.h>

class BornholdtModel
{
public:
	struct Params
	{
		double beta;	///< inverse temperature
		double epsilon; ///< threshold distribution width
		double mu; 		///< threshold distribution mean
	};

	// TODO do we need this, or should we use node states for this?
	typedef std::vector<short> state_t;	///< vector of neuron states (+1 or -1)

public:
	BornholdtModel(largenet::Graph& g, EdgeWeights& w, Params p);
	~BornholdtModel();

	void step();
private:
	largenet::Graph& net_;
	Params par_;
};

#endif /* BORNHOLDTMODEL_H_ */
