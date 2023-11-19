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
	switch (start->tokenType)
	{
	case TokenType::SEMICOLON:
		return { StatementType::EMPTY };
		break;
	case TokenType::EXIT:
		return parseExit(start, end);
		break;
	case TokenType::VARIABLE:
		return parseAssign(start, end);
		break;
	case TokenType::ERR:
		HE_DEBUG_BREAK;
		exitWithError(ErrorCode::INVALID_TOKEN, args->inputFile, start->row, start->col);
		return { StatementType::EMPTY };
		break;
	default:
		exitWithError(ErrorCode::SYNTAX_ERROR, args->inputFile, start->row, start->col);
		return { StatementType::EMPTY };
		break;
	}
}

Expression parseExpr(const Token* start, const Token* end)
{
	// TODO
	return Expression();
}

Statement parseExit(const Token* start, const Token* end)
{
	Statement statement;

	// *start is definitely an exit token
	const Token* token = start + 1;

	if (token == end)
	{
		exitWithError(ErrorCode::EXPECTED_AN_EXPRESSION, args->inputFile, end->row, end->col);
		return { StatementType::EMPTY };
	}

	statement.type = StatementType::EXIT;
	statement.a = createPtr<Expression>(std::forward<Expression>(parseExpr(token, end)));

	return statement;
}

Statement parseAssign(const Token* start, const Token* end)
{
	Statement statement;

	// *start is definitely a variable token
	const Token* token = start + 1;

	if (token >= end || token->tokenType != TokenType::ASSIGN)
	{
		HE_DEBUG_BREAK;
		exitWithError(ErrorCode::EXPECTED_EQUALS, args->inputFile, token->row, token->col);
		return { StatementType::EMPTY };
	}

	// a - variable to assign an expression to
	// b - the expression

	statement.type = StatementType::ASSIGN;
	statement.a = createPtr<Expression>();
	statement.a->type = ExprType::VARIABLE;
	statement.a->variable = start->variable; // TODO
	statement.b = createPtr<Expression>(std::forward<Expression>(parseExpr(token + 1, end)));

	return statement;
}