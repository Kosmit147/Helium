#include "args.h"

#include <string>

Args parseArgs(int argc, char* argv[])
{
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
				prevArg = ArgType::OUTPUT_FILE_NAME;
			else if (argStr == "-i")
				prevArg = ArgType::INPUT_FILE_NAME;
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