// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Statement.h"

using TokenType = Token::TokenType;
using ExprType = Expression::Type;
using StatementType = Statement::Type;

static const Args* args = nullptr;

std::vector<Statement> parseTokens(const Args& setArgs, const std::vector<Token>& tokens)
{
	args = &setArgs;

	std::vector<Statement> statements;
	statements.reserve(Tokenizer::getSemicolonCount());
	const Token* statementStart = &tokens[0];

	for (const Token& token : tokens)
	{
		if (token.tokenType == TokenType::SEMICOLON)
		{
			statements.push_back(std::move(parseStatement(statementStart, &token)));
			statementStart = (&token) + 1;
		}
	}

	const Token& lastToken = tokens.back();

	if (lastToken.tokenType != TokenType::SEMICOLON)
		exitWithError(ErrorCode::EXPECTED_A_SEMICOLON, args->inputFile, lastToken.col, lastToken.row);

	return statements;
}

Statement parseStatement(const Token* start, const Token* end)
{
	Statement statement;

	switch (start->tokenType)
	{
	case TokenType::SEMICOLON:
		statement.type = StatementType::EMPTY;
		break;
	case TokenType::EXIT:
		statement.type = StatementType::EXIT;
		break;
	case TokenType::VARIABLE:
		statement.type = StatementType::ASSIGN;
		break;
	case TokenType::ERR:
		HE_DEBUG_BREAK;
		exitWithError(ErrorCode::INVALID_TOKEN, args->inputFile, start->row, start->col);
		break;
	default:
		exitWithError(ErrorCode::SYNTAX_ERROR, args->inputFile, start->row, start->col);
		break;
	}

	for (const Token* token = start; token <= end; token++)
	{
		switch (token->tokenType)
		{
		case TokenType::SEMICOLON:
			continue;
		case TokenType::VARIABLE:
			// TODO
			break;
		case TokenType::LITERAL:
			// TODO
			break;
		case TokenType::ERR:
			HE_DEBUG_BREAK;
			exitWithError(ErrorCode::INVALID_TOKEN, args->inputFile, token->row, token->col);
			break;
		default:
			exitWithError(ErrorCode::SYNTAX_ERROR, args->inputFile, token->row, token->col);
			break;
		}
	}

	return statement;
}

Expression parseExpr(const Token* start, const Token* end)
{
	// TODO
	return Expression();
}
