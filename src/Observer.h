/*
 * Observer.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <list>

class Observer
{
public:
	virtual ~Observer()
	{
	}
	virtual void notify() = 0;
};

class Observable
{
	typedef std::list<Observer*> observers_t;
public:
	virtual ~Observable()
	{
	}
	void addObserver(Observer* o)
	{
		observers.push_back(o);
	}

	void notify()
	{
		for (observers_t::iterator it = observers.begin();
				it != observers.end(); ++it)
			(*it)->notify();
	}
private:
	observers_t observers;
};

#endif /* OBSERVER_H_ */
