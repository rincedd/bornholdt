/*
 * EvolutionController.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef EVOLUTIONCONTROLLER_H_
#define EVOLUTIONCONTROLLER_H_

#include "Controller.h"
#include "model/EdgeWeights.h"
#include "model/BornholdtModel.h"
#include "OStreamCollection.h"
#include "params.h"
#include <memory>

class Observer;

class EvolutionController: public Controller
{
public:
	EvolutionController(BornholdtParameters par);
	int exec();
	virtual ~EvolutionController() {}
	void writeInfo(std::ostream& strm) const;
private:
	void setup();
	void iterate(unsigned long steps, Observer* obs = 0);
	void updateTopology(largenet::Graph::EdgeIterator edge, double correlation);

private:
	BornholdtParameters par_;
	largenet::Graph graph_;
	std::auto_ptr<EdgeWeights> weights_;
	std::auto_ptr<BornholdtModel> model_;
	OStreamCollection streams_;
};

#endif /* EVOLUTIONCONTROLLER_H_ */
