#include "file.h"

#include "errorCodes.h"

std::stringstream readFile(std::string_view fileName)
{
	std::stringstream result;
	std::ifstream inputFile;

	try
	{
		inputFile.open(fileName.data(), std::ios::in);
		result << inputFile.rdbuf();
		inputFile.close();
	}
	catch (std::ifstream::failure)
	{
		exitWithError(ErrorCode::FAILED_TO_READ_FROM_INPUT_FILE);
	}

	return result;
}
