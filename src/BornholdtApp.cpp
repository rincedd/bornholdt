/*
 * BornholdtApp.cpp
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#include "BornholdtApp.h"
#include "EvolutionController.h"

BornholdtApp::BornholdtApp() : opts_()
{
}

void BornholdtApp::parseCommandLine(int argc, char** argv)
{
	try
	{
		opts_.parseCommandLine(argc, argv);
	} catch (BornholdtOptions::UsageError& e)
	{
		opts_.printHelpText(std::cout);
		exit(0);
	} catch (BornholdtOptions::ParsingError& e)
	{
		std::cerr << e.what() << "\n";
		exit(1);
	}
}

int BornholdtApp::exec()
{
	EvolutionController evol(opts_.params());
	evol.setName("evol");
	return evol.exec();
}
