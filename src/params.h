/*
 * params.h
 *
 *  Created on: 19.07.2012
 *      Author: gerd
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>
#include <boost/format.hpp>

class BornholdtParameters
{
public:
	std::string toStr() const
	{
		return str(
				boost::format(
						"%1%-n%2%-k%3%-K%4%--beta%5%-eps%6%-mu%7%--t%8%--I%9%-alpha%10%")
						% network % num_nodes % average_degree
						% average_active_connectivity % beta % epsilon % mu
						% num_iterations % num_topological_updates % alpha);
	}
public:
	double beta; ///< inverse temperature
	double mu; ///< threshold distribution mean
	double epsilon; ///< threshold distribution width

	double alpha; ///< correlation cutoff

	std::string network;	///< network type
	size_t num_nodes;
	double average_degree;
	double average_active_connectivity;

	size_t num_iterations;
	size_t num_topological_updates;
	size_t snapshot_interval;
	unsigned long seed;

	std::string mode, file;

	double scan_low, scan_high;
};

inline std::ostream& operator<<(std::ostream& out, const BornholdtParameters& p)
{
	out << "# Mode: " << p.mode << "\n";
	out << "# Parameters: beta = " << p.beta << ", mu = " << p.mu
			<< ", epsilon = " << p.epsilon;
	out << "\n# Correlation cutoff alpha = " << p.alpha;
	out << "\n# Network type: " << p.network;
	out << "\n# N = " << p.num_nodes << ", <k> = " << p.average_degree
			<< ", <K> = " << p.average_active_connectivity;
	out << "\n# Number of iterations: " << p.num_iterations
			<< "\n# Number of topological updates: "
			<< p.num_topological_updates;
	out << "\n# Snapshot interval: " << p.snapshot_interval;
	out << "\n# Lower scan bound: " << p.scan_low << ", upper scan bound: "
			<< p.scan_high;
	out << "\n# Random number generator seed: " << p.seed;
	return out;
}

#endif /* PARAMS_H_ */
