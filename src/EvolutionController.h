#ifndef EVOLUTIONCONTROLLER_H_
#define EVOLUTIONCONTROLLER_H_

#include "Controller.h"
#include "model/EdgeWeights.h"
#include "model/BornholdtModel.h"
#include "OFStreamManager.h"
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
	void initRandomNumberGenerator();
	void storeRandomNumberGeneratorSeed();
	void createRandomNetwork();
	void initEdgeWeights();
	void initModel();
	void activateEdge(const largenet::Graph::EdgeIterator& edge);
	void deactivateEdge(const largenet::Graph::EdgeIterator& edge);
	std::ostream& openStream(std::string tag);

private:
	BornholdtParameters par_;
	largenet::Graph graph_;
	std::auto_ptr<EdgeWeights> weights_;
	std::auto_ptr<BornholdtModel> model_;
	OFStreamManager streams_;
};

#endif /* EVOLUTIONCONTROLLER_H_ */
