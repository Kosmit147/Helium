#include "errorCodes.h"

#include <string>
#include <iostream>

#include "common.h"

void exitWithError(ErrorCode errorCodeEnum, std::string_view fileName,
	usize row, usize col)
{
	std::string errorMsg;
	std::string fileInfStr;

	if (!fileInfStr.empty())
	{
		fileInfStr = (std::string)fileName + ':'
			+ std::to_string(row) + ':' + std::to_string(col);
	}

	switch (errorCodeEnum)
	{
	case ErrorCode::INCORRECT_USAGE:
		errorMsg = "Incorrect usage. See helium -h.";
		break;
	case ErrorCode::FAILED_TO_READ_FROM_INPUT_FILE:
		errorMsg = "Failed to read from input file.";
		break;
	case ErrorCode::FAILED_TO_WRITE_TO_OUTPUT_FILE:
		errorMsg = "Failed to write to output file.";
		break;
	case ErrorCode::I32_LITERAL_OUT_OF_RANGE:
		errorMsg = fileInfStr + ' ' + "i32 literal out of range.";
		break;
	case ErrorCode::UNEXPECTED_CHARACTER:
		errorMsg = fileInfStr + ' ' + "Unexpected character.";
		break;
	case ErrorCode::FAILED_TO_TOKENIZE:
		errorMsg = fileInfStr + ' ' + "Failed to tokenize.";
		break;
	default:
		errorMsg = ERR_STR;
		HE_DEBUG_BREAK // Incorrect error code
		break;
	}

	int errorCode = (int)errorCodeEnum;

	std::cerr << "ERROR" << errorCode << ": " << errorMsg << std::endl;
	exit(errorCode);
}
