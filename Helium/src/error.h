// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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
	INVALID_TOKEN,
	EXPECTED_A_SEMICOLON,
	EXPECTED_EQUALS,
	EXPECTED_AN_EXPRESSION,
	SYNTAX_ERROR,
};

void exitWithError(ErrorCode errorCodeEnum, std::string_view fileName = "", 
	usize row = 0, usize col = 0);