#pragma once

#include <string>
#include <vector>

#include "common.h"
#include "errorCodes.h"

struct Args
{
	std::string inputFile;
	std::string outputFile;

	Args() : inputFile(""), outputFile("") {}
};

enum class ArgType
{
	NONE,
	OUTPUT_FILE_NAME,
	INPUT_FILE_NAME
};

Args parseArgs(int argc, char* argv[]);