
#ifndef STEPPER_H_
#define STEPPER_H_

class Stepper
{
public:
	virtual ~Stepper() {}
	virtual void step() = 0;
};

#endif /* STEPPER_H_ */
