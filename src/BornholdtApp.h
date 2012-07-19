/*
 * BornholdtApp.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTAPP_H_
#define BORNHOLDTAPP_H_

#include "BornholdtOptions.h"

class BornholdtApp
{
public:
	BornholdtApp();

	void parseCommandLine(int argc, char** argv);
	int exec();
private:
	BornholdtOptions opts_;
};

#endif /* BORNHOLDTAPP_H_ */
