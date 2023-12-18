// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

struct Args
{
	std::string inputFile = "";
	std::string outputFile = "";
};

enum class ArgType
{
	NONE,
	OUTPUT_FILE_NAME,
	INPUT_FILE_NAME,
	PRINT_HELP,
};

[[nodiscard]] Args parseArgs(int argc, char* argv[]);