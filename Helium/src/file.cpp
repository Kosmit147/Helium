// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "file.h"

#include <fstream>

#include "error.h"

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
	catch (std::ifstream::failure&)
	{
		exitWithError(ErrorCode::FAILED_TO_READ_FROM_INPUT_FILE);
	}

	return result;
}

void writeToFile(std::string_view fileName, const std::stringstream& stream)
{
	std::ofstream outputFile;

	try
	{
		outputFile.open(fileName.data(), std::ios::in);
		outputFile << stream.str();
		outputFile.close();
	}
	catch (std::ofstream::failure&)
	{
		exitWithError(ErrorCode::FAILED_TO_WRITE_TO_OUTPUT_FILE);
	}
}

void writeToFile(std::string_view fileName, const SegmentsData& segments)
{
	std::ofstream outputFile;

	try
	{
		outputFile.open(fileName.data(), std::ios::in);
		outputFile << segments.bss.str();
		outputFile << segments.data.str();
		outputFile << segments.text.str();
		outputFile.close();
	}
	catch (std::ofstream::failure&)
	{
		exitWithError(ErrorCode::FAILED_TO_WRITE_TO_OUTPUT_FILE);
	}
}
