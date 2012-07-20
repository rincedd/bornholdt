#include <stdexcept>

#include "BornholdtApp.h"
#include "EvolutionController.h"

using namespace std;

BornholdtApp::BornholdtApp() : opts_()
{
}

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

auto_ptr<Controller> BornholdtApp::createController()
{
	if (opts_.params().mode == "evolve")
	{
		auto_ptr<Controller> controller(new EvolutionController(opts_.params()));
		controller->setName("evol");
		return controller;
	}
	throw std::runtime_error("Unknown operation mode");
}

int BornholdtApp::exec()
{
	showHelpAndExitIfRequested();
	auto_ptr<Controller> controller = createController();
	return controller->exec();
}
