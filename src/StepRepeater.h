#ifndef STEPREPEATER_H_
#define STEPREPEATER_H_

#include "Stepper.h"
#include "Observer.h"

class StepRepeater: public Observable
{
public:
	StepRepeater(Stepper& s) : stepper(&s) {}
	void makeSteps(unsigned long nSteps)
	{
		for (unsigned long i = 0; i < nSteps; ++i)
		{
			stepper->step();
			notify();
		}
	}
private:
	Stepper* stepper;
};

#endif /* STEPREPEATER_H_ */
