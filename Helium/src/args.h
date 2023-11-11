#pragma once

#include <vector>

#include "common.h"
#include "errorCodes.h"

struct Args
{
	static std::string inputFile;
	static std::string outputFile;
};

enum class ArgType
{
	NONE,
	OUTPUT_FILE_NAME,
	INPUT_FILE_NAME
};

void parseArgs(int argc, char* argv[]);