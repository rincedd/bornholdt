#include "GraphStateWriter.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace largenet;

void GraphStateWriter::write(const Graph& g, ostream& strm)
{
	BOOST_FOREACH(const Edge& e, g.edges())
	{
		strm << e.source()->id() << "\t" << e.target()->id() << "\t"
				<< weights_(e) << "\t" << g.edgeState(e.id()) << "\t"
				<< model_.output(e.source()->id()) << "\t"
				<< model_.output(e.target()->id()) << "\t"
				<< model_.threshold(e.source()->id()) << "\t"
				<< model_.threshold(e.target()->id()) << "\n";
	}
}
