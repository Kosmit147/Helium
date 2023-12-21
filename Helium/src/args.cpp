// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "Args.h"

#include "error.h"

Args parseArgs(int argc, char** argv)
{
	if (argc <= 1)
		exitWithError(ErrorCode::INCORRECT_USAGE);

	Args args;
	ArgType prevArg = ArgType::NONE;
	bool expectingValue = false;

	// skip first arg, it's the path to the executable
	for (it i = 1; i < argc; i++)
	{
		const char* currArg = argv[i];

		if (currArg[0] == '-')
		{
			switch (currArg[1])
			{
			case 'i':
				prevArg = ArgType::INPUT_FILE_NAME;
				expectingValue = true;
				break;
			case 'o':
				prevArg = ArgType::OUTPUT_FILE_NAME;
				expectingValue = true;
				break;
			case 'h':
				args.printHelp = true;
				prevArg = ArgType::PRINT_HELP;
				expectingValue = false;
				break;
			default:
				std::string msg;
				msg.reserve(26);
				msg.append("Unrecognized argument: -");
				msg.push_back(currArg[1]);
				msg.push_back('.');
				exitWithError(ErrorCode::UNRECOGNIZED_ARGUMENT, msg);
				break;
			}
		}
		else
		{
			if (!expectingValue)
				exitWithError(ErrorCode::INCORRECT_USAGE);

			switch (prevArg)
			{
			case ArgType::INPUT_FILE_NAME:
				args.inputFile = currArg;
				break;
			case ArgType::OUTPUT_FILE_NAME:
				args.outputFile = currArg;
				break;
			case ArgType::NONE:
			default:
				exitWithError(ErrorCode::INCORRECT_USAGE);
				break;
			}

			expectingValue = false;
		}
	}

	validateArgs(args);
	return args;
}

void validateArgs(Args& args)
{
	std::string& inputFile = args.inputFile;
	std::string& outputFile = args.outputFile;

	if (inputFile.empty())
		exitWithError(ErrorCode::NO_INPUT_FILE_SPECIFIED);

	if (outputFile.empty())
		exitWithError(ErrorCode::NO_OUTPUT_FILE_SPECIFIED);

	usize dotIndex = outputFile.rfind('.');

	if (dotIndex != std::string::npos)
	{
		bool hasExeExtension = (strcmp(outputFile.c_str() + dotIndex + 1, "exe") == 0);

		if (!hasExeExtension)
			outputFile.append(".exe");
	}
	else
	{
		outputFile.append(".exe");
	}
}