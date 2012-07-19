/*
 * AverageEvolutionLogger.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef AVERAGEEVOLUTIONLOGGER_H_
#define AVERAGEEVOLUTIONLOGGER_H_

#include "Logger.h"
#include <largenet2.h>
#include "../model/EdgeWeights.h"
#include "../model/edge_states.h"

class AverageEvolutionLogger: public StreamLogger
{
public:
	AverageEvolutionLogger(const largenet::Graph& graph,
			const EdgeWeights& weights) :
			StreamLogger(), graph_(graph), weights_(weights)
	{
	}

	virtual ~AverageEvolutionLogger()
	{
	}

	void log(double t)
	{
		double sigma = 0;
		BOOST_FOREACH(EdgeWeights::edge_weight_map_t::value_type it, weights_.weights())
		{
			sigma += it.second;
		}
		sigma /= graph_.numberOfEdges(ACTIVE);
		stream() << t << "\t"
				<< static_cast<double>(graph_.numberOfEdges(ACTIVE))
						/ graph_.numberOfNodes() << "\t" << sigma << "\n";
	}

	void writeHeader(double t)
	{
		stream() << "# t\t<k>\t<weight>\n";
	}

	void reset()
	{
		stream() << "\n\n";
	}
private:
	const largenet::Graph& graph_;
	const EdgeWeights& weights_;
};

#endif /* AVERAGEEVOLUTIONLOGGER_H_ */
