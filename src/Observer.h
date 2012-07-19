/*
 * Observer.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

class Observer
{
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void notify() = 0;
};


#endif /* OBSERVER_H_ */
