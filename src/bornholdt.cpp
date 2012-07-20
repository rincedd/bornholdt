#include <exception>
#include <iostream>

#include "BornholdtApp.h"

int main(int argc, char **argv)
{
	try
	{
		BornholdtApp a;
		a.parseCommandLine(argc, argv);
		return a.exec();
	} catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}
}

