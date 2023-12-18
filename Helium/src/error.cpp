// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "error.h"

#include "global.h"

void exitWithError(ErrorCode errorCodeEnum, Token::FilePosition filePos)
{
	std::string errorMsg;
	std::string fileInfStr;

	if (!global::args.inputFile.empty())
	{
		fileInfStr = (std::string)global::args.inputFile + ':'
			+ std::to_string(filePos.row) + ':' + std::to_string(filePos.col);
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
	case ErrorCode::FAILED_TO_PARSE_STATEMENT:
		errorMsg = fileInfStr + ' ' + "Failed to parse statement.";
		break;
	case ErrorCode::INVALID_TOKEN:
		errorMsg = fileInfStr + ' ' + "Parser encountered an invalid token.";
		break;
	case ErrorCode::EXPECTED_A_SEMICOLON:
		errorMsg = fileInfStr + ' ' + "Expected a ';'.";
		break;
	case ErrorCode::EXPECTED_EQUALS:
		errorMsg = fileInfStr + ' ' + "Expected an '='.";
		break;
	case ErrorCode::EXPECTED_AN_EXPRESSION:
		errorMsg = fileInfStr + ' ' + "Expected an expression.";
		break;
	case ErrorCode::EXPECTED_A_CLOSING_PAREN:
		errorMsg = fileInfStr + ' ' + "Expected a ')'";
		break;
	case ErrorCode::SYNTAX_ERROR:
		errorMsg = fileInfStr + ' ' + "Syntax error.";
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
