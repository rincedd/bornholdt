#ifndef CORRELATIONACCUMULATOR_H_
#define CORRELATIONACCUMULATOR_H_

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

namespace ba = boost::accumulators;

class CorrelationAccumulator
{
	typedef ba::accumulator_set<double, ba::stats<ba::tag::mean> > mean_acc_t;
public:
	CorrelationAccumulator() :
			state_i_(0), state_j_(0)
	{
	}
	CorrelationAccumulator(const short& state_i, const short& state_j) :
			state_i_(&state_i), state_j_(&state_j)
	{
	}

	void setObservedStates(const short& state_i, const short& state_j)
	{
		state_i_ = &state_i;
		state_j_ = &state_j;
	}

	void update()
	{
		if ((state_i_ != 0) && (state_j_ != 0))
			acc_(*state_i_ + *state_j_);
	}
	double mean() const
	{
		return ba::mean(acc_) / 2.0;
	}
private:
	const short* state_i_;
	const short* state_j_;
	mean_acc_t acc_;
};

#endif /* CORRELATIONACCUMULATOR_H_ */
