/*
 * CorrelationObserver.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef CORRELATIONOBSERVER_H_
#define CORRELATIONOBSERVER_H_

#include "Observer.h"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

namespace ba = boost::accumulators;

class CorrelationObserver: public Observer
{
	typedef ba::accumulator_set<double, ba::stats<ba::tag::mean> > mean_acc_t;
public:
	CorrelationObserver(const short& state_i, const short& state_j) :
			state_i_(state_i), state_j_(state_j)
	{
	}
	virtual ~CorrelationObserver()
	{
	}
	void notify()
	{
		acc_(state_i_ * state_j_);
	}
	double mean() const
	{
		return ba::mean(acc_);
	}
private:
	const short& state_i_;
	const short& state_j_;
	mean_acc_t acc_;
};

#endif /* CORRELATIONOBSERVER_H_ */
