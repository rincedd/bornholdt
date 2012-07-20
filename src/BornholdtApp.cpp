#include <stdexcept>

#include "BornholdtApp.h"
#include "EvolutionController.h"
#include "ScanController.h"

using namespace std;

void BornholdtApp::parseCommandLine(int argc, char** argv)
{
	try
	{
		opts_.parseCommandLine(argc, argv);
	} catch (BornholdtOptions::ParsingError& e)
	{
		cerr << e.what() << "\n";
		exit(1);
	}
}

void BornholdtApp::showHelpAndExitIfRequested()
{
	if (opts_.isHelpRequested())
	{
		opts_.printHelpText(cout);
		exit(0);
	}
}

void BornholdtApp::createController()
{
	if (opts_.params().mode == "evolve")
	{
		controller_.reset(new EvolutionController(opts_.params()));
		controller_->setName("evol");
	}
	else if (opts_.params().mode == "scan")
	{
		controller_.reset(new ScanController(opts_.params()));
		controller_->setName("scan");
	}
	else
		throw std::runtime_error("Unknown operation mode");
}

int BornholdtApp::executeController()
{
	try
	{
		return controller_->exec();
	} catch (exception& e)
	{
		cerr << e.what() << "\n";
	}
	return 1;
}

int BornholdtApp::exec()
{
	showHelpAndExitIfRequested();
	createController();
	return executeController();
}
