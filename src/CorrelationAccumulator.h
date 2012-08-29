#ifndef CORRELATIONACCUMULATOR_H_
#define CORRELATIONACCUMULATOR_H_

class CorrelationAccumulator
{
public:
	CorrelationAccumulator() :
			state_i_(0), state_j_(0), productSum_(0), sumSum_(0)
	{
	}
	CorrelationAccumulator(const short& state_i, const short& state_j) :
			state_i_(&state_i), state_j_(&state_j), productSum_(0), sumSum_(0)
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
		{
			productSum_ += *state_i_ * *state_j_;
			sumSum_ += *state_i_ + *state_j_;
		}
	}
	double mean() const
	{
		if (sumSum_ > 0)
			return 2 * productSum_ / sumSum_;
		else
			return 0;
	}
private:
	const short* state_i_;
	const short* state_j_;
	double productSum_, sumSum_;
};

#endif /* CORRELATIONACCUMULATOR_H_ */
