/*
 * BornholdtApp.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTAPP_H_
#define BORNHOLDTAPP_H_

#include "BornholdtOptions.h"
#include <memory>

class Controller;

class BornholdtApp
{
public:
	BornholdtApp();

	void parseCommandLine(int argc, char** argv);
	int exec();
private:
	void showHelpAndExitIfRequested();
	std::auto_ptr<Controller> createController();
private:
	BornholdtOptions opts_;
};

#endif /* BORNHOLDTAPP_H_ */
