/*
 * SnapshotLogger.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef SNAPSHOTLOGGER_H_
#define SNAPSHOTLOGGER_H_

#include <largenet2.h>
#include "../model/EdgeWeights.h"
#include "../model/GraphStateWriter.h"
#include "../model/BornholdtModel.h"

class SnapshotLogger: public StreamLogger
{
public:
	SnapshotLogger(const largenet::Graph& graph, const EdgeWeights& weights, const BornholdtModel& model) :
			StreamLogger(), graph_(graph), weights_(weights), model_(model)
	{
	}

	void log(double t)
	{
		stream() << "# t = " << t << "\n";
		GraphStateWriter writer(weights_, model_);
		writer.write(graph_, stream());
		stream() << "\n\n";
	}

	void writeHeader(double t)
	{
		stream() << "# source\ttarget\tweight\tstate\tspin_s\tspin_t\n";
	}

private:
	const largenet::Graph& graph_;
	const EdgeWeights& weights_;
	const BornholdtModel& model_;
};

#endif /* SNAPSHOTLOGGER_H_ */
