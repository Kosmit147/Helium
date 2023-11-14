#pragma once

#include <string>

#include "common.h"

enum class ErrorCode
{
	INCORRECT_USAGE = 1,
	FAILED_TO_READ_FROM_INPUT_FILE,
	FAILED_TO_WRITE_TO_OUTPUT_FILE,
	I32_LITERAL_OUT_OF_RANGE,
	UNEXPECTED_CHARACTER,
	FAILED_TO_TOKENIZE,
};

void exitWithError(ErrorCode errorCodeEnum, std::string_view fileName = "", 
	usize row = 0, usize col = 0);