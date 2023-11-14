#pragma once

#include <string>

#include "common.h"
#include "errorCodes.h"

struct Args
{
	std::string inputFile = "";
	std::string outputFile = "";
};

enum class ArgType
{
	NONE,
	OUTPUT_FILE_NAME,
	INPUT_FILE_NAME
};

[[nodiscard]] Args parseArgs(int argc, char* argv[]);