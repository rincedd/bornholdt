#ifndef CORRELATIONOBSERVER_H_
#define CORRELATIONOBSERVER_H_

#include <largenet2.h>
#include <vector>

#include "Observer.h"
#include "model/BornholdtModel.h"
#include "CorrelationAccumulator.h"
#include "model/edge_states.h"

class CorrelationObserver: public Observer
{
public:
	CorrelationObserver(const largenet::Graph& graph,
			const BornholdtModel& model)
	{
		correlations_.reserve(graph.numberOfEdges(ACTIVE));
		BOOST_FOREACH(const largenet::Edge& edge, graph.edges(ACTIVE))
		{
			correlations_.push_back(
					CorrelationAccumulator(model.spin(edge.source()->id()),
							model.spin(edge.target()->id())));
		}
	}
	void notify()
	{
		BOOST_FOREACH(CorrelationAccumulator& acc, correlations_)
		{
			acc.update();
		}
	}

	double mean() const
	{
		double mean = 0;
		BOOST_FOREACH(const CorrelationAccumulator& acc, correlations_)
		{
			mean += acc.mean();
		}
		return mean / correlations_.size();
	}

	double fractionWithAbsoluteValueAbove(double threshold = 0.0) const
	{
		double frac = 0;
		BOOST_FOREACH(const CorrelationAccumulator& acc, correlations_)
		{
			if (abs(acc.mean()) > threshold)
				frac += 1;
		}
		return frac / correlations_.size();
	}
private:
	std::vector<CorrelationAccumulator> correlations_;
};

#endif /* CORRELATIONOBSERVER_H_ */
