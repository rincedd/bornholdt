/*
 * BornholdtOptions.cpp
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#include "BornholdtOptions.h"
#include <boost/format.hpp>

using namespace std;
namespace po = boost::program_options;

BornholdtOptions::BornholdtOptions() :
		par_(), allOptions_("Allowed options")
{
	allOptions_.add_options()("help,h", "This help message.");
	init();
}


void BornholdtOptions::init()
{
	po::options_description modelOpts("Model parameters"), simOpts(
			"Simulation parameters"), scanOpts("Scan options");

	modelOpts.add_options()
			("beta", po::value<double>(&par_.beta)->default_value(25), "Inverse temperature")
			("epsilon", po::value<double>(&par_.epsilon)->default_value(0.1), "Threshold distribution width.")
			("mu", po::value<double>(&par_.mu)->default_value(-0.1), "Threshold distribution mean.");

	simOpts.add_options()
			("nodes,n", po::value<size_t>(&par_.num_nodes)->default_value(100), "Number of nodes.")
			("avg-degree,k", po::value<double>(&par_.average_degree)->default_value(20), "Intended average degree of network.")
			("iterations", po::value<size_t>(&par_.num_iterations)->default_value(200), "Number of iterations between topological updates.")
			("updates", po::value<size_t>(&par_.num_topological_updates)->default_value(10), "Number of topological update steps.")
			("alpha", po::value<double>(&par_.alpha)->default_value(0.8), "Correlation cutoff.")
			("snapshot-interval", po::value<size_t>(&par_.snapshot_interval)->default_value(10), "Log network snapshots every ARG iterations.")
			("random-seed", po::value<unsigned long>(&par_.seed)->default_value(0), "Random number generator seed.");

	scanOpts.add_options()
			("low", po::value<double>(&par_.scan_low)->default_value(0.8), "Lower value of scan parameter.")
			("high", po::value<double>(&par_.scan_high)->default_value(1.2), "Upper value of scan parameter.");

	allOptions_.add_options()
			("mode", po::value<string>(&par_.mode)->default_value("evolve"), "Operation mode: evolve, scan.")
			("file", po::value<string>(&par_.file)->default_value(""), "Network file to load in scan mode.");

	allOptions_.add(modelOpts).add(simOpts).add(scanOpts);
}

void BornholdtOptions::parseCommandLine(int argc, char** argv)
{
	try
	{
		po::store(po::parse_command_line(argc, argv, allOptions_), vm_);
		po::notify(vm_);
	} catch (po::error& e)
	{
		throw ParsingError(e);
	}
	if (vm_.count("help"))
		throw UsageError("");

	if (par_.mode == "scan" && par_.file.empty())
		throw ParsingError("Must provide file name in scan mode.");
}

