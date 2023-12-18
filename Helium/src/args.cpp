// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "Args.h"

#include "help.h"
#include "error.h"

Args parseArgs(int argc, char* argv[])
{
	if (argc <= 1)
		exitWithError(ErrorCode::INCORRECT_USAGE);

	Args args;

	ArgType prevArg = ArgType::NONE;
	std::string argStr;

	// skip first arg, it's the name of the executable
	for (it i = 1; i < argc; i++)
	{
		argStr = argv[i];

		if (argStr[0] == '-' && prevArg == ArgType::NONE)
		{
			if (argStr == "-o")
			{
				prevArg = ArgType::OUTPUT_FILE_NAME;
			}
			else if (argStr == "-i")
			{
				prevArg = ArgType::INPUT_FILE_NAME;
			}
			else if (argStr == "-h")
			{
				prevArg = ArgType::PRINT_HELP;
				printHelp();
			}
		}
		else if (argStr[0] != '-')
		{
			switch (prevArg)
			{
			case ArgType::NONE:
			case ArgType::INPUT_FILE_NAME:
				args.inputFile = argStr;
				break;
			case ArgType::OUTPUT_FILE_NAME:
				args.outputFile = argStr;
				break;
			case ArgType::PRINT_HELP:
				exitWithError(ErrorCode::INCORRECT_USAGE);
				break;
			}

			prevArg = ArgType::NONE;
		}
		else
		{
			exitWithError(ErrorCode::INCORRECT_USAGE);
		}
	}

	return args;
}