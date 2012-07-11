#include "BornholdtApp.h"

int main(int argc, char **argv)
{
	BornholdtApp a;
	a.parseCommandLine(argc, argv);
	return a.exec();
}

