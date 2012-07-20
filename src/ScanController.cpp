#include "ScanController.h"

#include <myrng/myrngWELL.h>
#include "model/GraphStateReader.h"
#include "Filename.h"
#include "loggers/AverageEvolutionLogger.h"
#include "StepRepeater.h"

using namespace std;
using namespace largenet;

ScanController::ScanController(BornholdtParameters par) :
		par_(par), graph_(1, 2), weights_(0), orig_weights_(0), model_(0), streams_(), scan_par_(
				par.scan_low)
{
	initRandomNumberGenerator();
}

void ScanController::storeRandomNumberGeneratorSeed()
{
	par_.seed = rng.getSeed();
}

void ScanController::initRandomNumberGenerator()
{
	if (par_.seed != 0)
		rng.seed(par_.seed);
	storeRandomNumberGeneratorSeed();
}

void ScanController::initEdgeWeights()
{
	weights_.reset(
			new EdgeWeights(par_.num_nodes * par_.average_degree,
					par_.num_nodes));
}

void ScanController::initModel()
{
	BornholdtModel::Params par =
	{ par_.beta, par_.epsilon, par_.mu };
	model_.reset(new BornholdtModel(graph_, *weights_, par));
}

void ScanController::checkNetworkFilename()
{
	if (par_.file.empty())
		throw std::runtime_error("No file name given");
}

auto_ptr<istream> ScanController::openInputStream(string fileName)
{
	auto_ptr<istream> input_stream(new ifstream(fileName.c_str()));
	if (input_stream->bad())
		throw std::runtime_error("Cannot read from input file");
	return input_stream;
}

void ScanController::loadNetwork()
{
	checkNetworkFilename();
	GraphStateReader reader;
	reader.createFromStream(*openInputStream(par_.file), graph_, *weights_,
			*model_);
	orig_weights_.reset(new EdgeWeights(*weights_));
}

void ScanController::setup()
{
	initEdgeWeights();
	initModel();
	loadNetwork();
}

void ScanController::writeInfo(ostream& strm) const
{
	strm << par_ << "\n";
}

ostream& ScanController::openStream(string tag)
{
	return streams_.openStream(Filename(name(), par_, tag));
}

void ScanController::updateTopology()
{
	// FIXME this is stupid, as we are interested in the average connectivity...
	weights_->assign(*orig_weights_);
	weights_->scale(scan_par_);
}

int ScanController::exec()
{
	setup();

	size_t iterations = 0;

	ostream& scan_stream = openStream("scan");
	writeInfo(scan_stream);
	AverageEvolutionLogger scan_logger(graph_, *weights_);
	scan_logger.setStream(scan_stream);
	scan_logger.writeHeader(iterations);

	double step = (par_.scan_high - par_.scan_low) / par_.num_topological_updates;
	scan_par_ = par_.scan_low;
	updateTopology();
	for (; iterations < par_.num_topological_updates; ++iterations)
	{
		StepRepeater stepper(*model_);
		stepper.makeSteps(par_.num_iterations);
		scan_logger.log(scan_par_);
		updateTopology();
		scan_par_ += step;
	}
	scan_logger.log(iterations);
	return 0;
}