#ifndef GRAPHSTATEWRITER_H_
#define GRAPHSTATEWRITER_H_

#include <largenet2/io/GraphWriter.h>
#include "EdgeWeights.h"
#include "BornholdtModel.h"

class GraphStateWriter: public largenet::io::GraphWriter
{
public:
	GraphStateWriter(const EdgeWeights& weights,
			const BornholdtModel& model) :
			GraphWriter(), weights_(weights), model_(model)
	{
	}
	virtual ~GraphStateWriter()
	{
	}
	void write(const largenet::Graph& g, std::ostream& strm);
private:
	const EdgeWeights& weights_;
	const BornholdtModel& model_;
};

#endif /* GRAPHSTATEWRITER_H_ */
