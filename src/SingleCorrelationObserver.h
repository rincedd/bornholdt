#ifndef SINGLECORRELATIONOBSERVER_H_
#define SINGLECORRELATIONOBSERVER_H_

#include "Observer.h"
#include "CorrelationAccumulator.h"

class SingleCorrelationObserver: public Observer
{
public:
	SingleCorrelationObserver(const short& state_i, const short& state_j) :
			acc_(state_i, state_j)
	{
	}
	void notify()
	{
		acc_.update();
	}
	double mean() const
	{
		return acc_.mean();
	}
private:
	CorrelationAccumulator acc_;
};

#endif /* SINGLECORRELATIONOBSERVER_H_ */
