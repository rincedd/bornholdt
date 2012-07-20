#ifndef SCANCONTROLLER_H_
#define SCANCONTROLLER_H_

#include <string>
#include <memory>
#include <iostream>
#include <largenet2.h>
#include "Controller.h"
#include "params.h"
#include "OFStreamManager.h"
#include "model/BornholdtModel.h"
#include "model/EdgeWeights.h"

class ScanController: public Controller
{
public:
	ScanController(BornholdtParameters par);
	int exec();
	void writeInfo(std::ostream& strm) const;
private:
	void setup();
	void initRandomNumberGenerator();
	void storeRandomNumberGeneratorSeed();
	void loadNetwork();
	void initEdgeWeights();
	void initModel();
	std::ostream& openStream(std::string tag);
	void checkNetworkFilename();
	std::auto_ptr<std::istream> openInputStream(std::string fileName);

private:
	BornholdtParameters par_;
	largenet::Graph graph_;
	std::auto_ptr<EdgeWeights> weights_;
	std::auto_ptr<BornholdtModel> model_;
	OFStreamManager streams_;
};

#endif /* SCANCONTROLLER_H_ */
