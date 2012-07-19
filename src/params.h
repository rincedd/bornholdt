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
						"n%1%-k%2%--beta%3%-eps%4%-mu%5%--t%6%--I%7%-alpha%8%")
						% num_nodes % average_degree % beta % epsilon % mu
						% num_iterations % num_topological_updates % alpha);
	}
public:
	double beta; ///< inverse temperature
	double mu; ///< threshold distribution mean
	double epsilon; ///< threshold distribution width

	double alpha; ///< correlation cutoff

	size_t num_nodes;
	double average_degree;

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
	out << "\n# N = " << p.num_nodes << ", <k> = " << p.average_degree;
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