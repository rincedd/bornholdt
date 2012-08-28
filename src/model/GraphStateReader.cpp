/*
 * GraphStateReader.cpp
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#include "GraphStateReader.h"
#include <sstream>
#include <utility>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace largenet;

namespace detail
{

struct node_info
{
	node_id_t id;
	short spin;
	double threshold;
};

struct edge_info
{
	node_info source;
	node_info target;
	edge_state_t state;
	double weight;
};

}

void GraphStateReader::createFromStream(istream& strm, Graph& graph,
		EdgeWeights& weights, BornholdtModel& model)
{
	if (!strm)
		return;

	typedef vector<detail::edge_info> edge_v;
	node_id_t maxNodeID = 0;
	edge_v edges;
	string line;
	stringstream ss;

	// now read: source target weight s_s s_t thresh_s thresh_t

	while (getline(strm, line))
	{
		if (line.empty())
			break;

		line += " ";
		if (line[0] == '#')
			continue;

		ss.str(line);
		detail::edge_info e;
		ss >> e.source.id >> e.target.id;
		if (ss.fail())
			throw std::runtime_error("Cannot read input file");

		maxNodeID = max(e.source.id, maxNodeID);
		maxNodeID = max(e.target.id, maxNodeID);

		ss >> e.weight >> e.state >> e.source.spin >> e.target.spin
				>> e.source.threshold >> e.target.threshold;
		if (ss.fail())
			throw std::runtime_error("Cannot read input file");

		edges.push_back(e);
	}

	graph.clear();
	while (graph.numberOfNodes() < maxNodeID + 1)
		graph.addNode();

	if (model.size() < graph.numberOfNodes())
		model.resize(graph.numberOfNodes());

	BOOST_FOREACH(detail::edge_info& e, edges)
	{
		edge_id_t eid = graph.addEdge(e.source.id, e.target.id, true);
		graph.setEdgeState(eid, e.state);
		weights.setWeight(*graph.edge(eid), e.weight);
		model.output(e.source.id) = e.source.spin;
		model.output(e.target.id) = e.target.spin;
		model.threshold(e.source.id) = e.source.threshold;
		model.threshold(e.target.id) = e.target.threshold;
	}
}
