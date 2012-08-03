#ifndef SCANLOGGER_H_
#define SCANLOGGER_H_

#include "Logger.h"
#include <largenet2.h>
#include "../model/EdgeWeights.h"
#include "../model/edge_states.h"

class ScanLogger: public StreamLogger
{
public:
	ScanLogger(const largenet::Graph& graph, const EdgeWeights& weights) :
			StreamLogger(), graph_(graph), weights_(weights), meanCorrelation_(
					0), aboveAlphaFraction_(0), belowAlphaFraction_(0), switchedFraction_(0)
	{
	}

	void setMeanCorrelation(double value)
	{
		meanCorrelation_ = value;
	}

	void setAboveAlphaFraction(double value)
	{
		aboveAlphaFraction_ = value;
	}

	void setBelowAlphaFraction(double value)
	{
		belowAlphaFraction_ = value;
	}

	void setSwitchedFraction(double value)
	{
		switchedFraction_ = value;
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
						/ graph_.numberOfNodes() << "\t" << sigma << "\t"
				<< meanCorrelation_ << "\t" << aboveAlphaFraction_ << "\t"
				<< belowAlphaFraction_ << "\t" << switchedFraction_ << "\n";
	}

	void writeHeader(double t)
	{
		stream() << "# t\t<k>\t<weight>\t<c_ij>\t[c_ij>alpha]\t[c_ij<alpha]\t[fired]\n";
	}

	void reset()
	{
		stream() << "\n\n";
	}
private:
	const largenet::Graph& graph_;
	const EdgeWeights& weights_;
	double meanCorrelation_;
	double aboveAlphaFraction_, belowAlphaFraction_;
	double switchedFraction_;
};

#endif /* SCANLOGGER_H_ */
