/*
 * Logger.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

class Logger
{
public:
	Logger()
	{
	}
	virtual ~Logger()
	{
	}
	virtual void log(double t) = 0;
	virtual void writeHeader(double t) = 0;
	virtual void reset() {}
};

class StreamLogger: public Logger
{
public:
	StreamLogger() :
			Logger(), strm_(0)
	{
		setStream(std::cout);
	}
	virtual ~StreamLogger()
	{
	}
	void setStream(std::ostream& strm)
	{
		strm_ = &strm;
	}
	std::ostream& stream()
	{
		return *strm_;
	}
private:
	std::ostream* strm_;
};

#endif /* LOGGER_H_ */
