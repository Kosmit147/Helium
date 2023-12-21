// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

#include "Token.h"

enum class ErrorCode
{
	NO_ERROR = 0,
	INCORRECT_USAGE = 1,
	UNRECOGNIZED_ARGUMENT,
	NO_INPUT_FILE_SPECIFIED,
	NO_OUTPUT_FILE_SPECIFIED,
	FAILED_TO_READ_FROM_INPUT_FILE,
	FAILED_TO_WRITE_TO_OUTPUT_FILE,
	I32_LITERAL_OUT_OF_RANGE,
	UNEXPECTED_CHARACTER,
	FAILED_TO_TOKENIZE,
	FAILED_TO_PARSE_STATEMENT,
	INVALID_TOKEN,
	EXPECTED_A_SEMICOLON,
	EXPECTED_EQUALS,
	EXPECTED_AN_EXPRESSION,
	EXPECTED_A_CLOSING_PAREN,
	SYNTAX_ERROR,
};

void exitWithError(ErrorCode errorCodeEnum, Token::FilePosition filePos = { 0, 0 });
void exitWithError(ErrorCode errorCodeEnum, std::string_view msg);