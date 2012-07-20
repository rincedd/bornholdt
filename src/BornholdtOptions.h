/*
 * BornholdtOptions.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef BORNHOLDTOPTIONS_H_
#define BORNHOLDTOPTIONS_H_

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

#include "params.h"

class BornholdtOptions
{
public:
	class ParsingError: public boost::program_options::error
	{
	public:
		ParsingError(const boost::program_options::error& e) :
				error(e)
		{
		}
		explicit ParsingError(const std::string& xwhat) :
				error(xwhat)
		{
		}
	};
public:
	BornholdtOptions();
	void parseCommandLine(int argc, char** argv);
	bool isHelpRequested() const;

	const BornholdtParameters& params() const
	{
		return par_;
	}
	void printHelpText(std::ostream& out) const
	{
		out << allOptions_ << "\n";
	}
private:
	void setup();
	void getOptionsFromCommandLine(int argc, char** argv);

private:
	BornholdtParameters par_;
	boost::program_options::options_description allOptions_;
	boost::program_options::variables_map vm_;
};

#endif /* BORNHOLDTOPTIONS_H_ */
