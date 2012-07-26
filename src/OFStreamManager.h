#ifndef OFSTREAMMANAGER_H_
#define OFSTREAMMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <boost/ptr_container/ptr_vector.hpp>

class OFStreamManager
{
public:
	std::ostream& openStream(std::string filename)
	{
		std::auto_ptr<std::ofstream> o(new std::ofstream(filename.c_str()));
		if (o->fail())
		{
			o->close();
			throw("Could not open " + filename + " for writing!\n");
		}
		streams_.push_back(o);
		return streams_.back();

	}
private:
	typedef boost::ptr_vector<std::ostream> ostream_ptr_v;
	ostream_ptr_v streams_;
};

#endif /* OFSTREAMMANAGER_H_ */
