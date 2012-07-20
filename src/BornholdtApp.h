/*
 * BornholdtApp.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTAPP_H_
#define BORNHOLDTAPP_H_

#include "BornholdtOptions.h"
#include "Controller.h"
#include <memory>

class BornholdtApp
{
public:
	void parseCommandLine(int argc, char** argv);
	int exec();
private:
	void showHelpAndExitIfRequested();
	void createController();
	int executeController();

private:
	BornholdtOptions opts_;
	std::auto_ptr<Controller> controller_;
};

#endif /* BORNHOLDTAPP_H_ */
