#include <iostream>

#include "common.h"
#include "args.h"

int main(int argc, char* argv[])
{
	parseArgs(argc, argv);

	std::cout << "Input File: " << Args::inputFile << std::endl;
	std::cout << "Output File: " << Args::outputFile << std::endl;
}