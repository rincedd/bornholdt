/*
 * BornholdtApp.h
 *
 *  Created on: 11.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTAPP_H_
#define BORNHOLDTAPP_H_

class BornholdtApp
{
public:
	BornholdtApp();
	virtual ~BornholdtApp();

	void parseCommandLine(int argc, char** argv);
	int exec();
};

#endif /* BORNHOLDTAPP_H_ */
