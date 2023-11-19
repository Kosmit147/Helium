// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Parser.h"
#include "error.h"

using TokenType = Token::TokenType;
using ExprType = Expression::Type;
using StatementType = Statement::Type;

const Args* Parser::_args = nullptr;

std::vector<Statement> Parser::parseTokens(const Args& args, const std::vector<Token>& tokens)
{
	_args = &args;

	std::vector<Statement> statements;
	statements.reserve(Tokenizer::semicolonCount());
	const Token* stmtStartToken = &tokens[0];

	for (const Token& token : tokens)
	{
		if (token.tokenType == TokenType::SEMICOLON)
		{
			statements.emplace_back(std::forward<Statement>(parseStatement(stmtStartToken, &token)));
			stmtStartToken = (&token) + 1;
		}
	}

	const Token& lastToken = tokens.back();

	if (lastToken.tokenType != TokenType::SEMICOLON)
		exitWithError(ErrorCode::EXPECTED_A_SEMICOLON, _args->inputFile, lastToken.col, lastToken.row);

	return statements;
}

Statement Parser::parseStatement(const Token* start, const Token* end)
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
		exitWithError(ErrorCode::INVALID_TOKEN, _args->inputFile, start->row, start->col);
		return { StatementType::EMPTY };
		break;
	default:
		exitWithError(ErrorCode::SYNTAX_ERROR, _args->inputFile, start->row, start->col);
		return { StatementType::EMPTY };
		break;
	}
}

Expression Parser::parseExpr(const Token* start, const Token* end)
{
	for (const Token* token = start; token <= end; token++)
	{
		switch (token->tokenType)
		{
		case TokenType::LITERAL:
			break;
		case TokenType::VARIABLE:
			break;
		case TokenType::OPEN_PAREN:
			break;
		case TokenType::CLOSE_PAREN:
			break;
		case TokenType::PLUS:
			break;
		case TokenType::MINUS:
			break;
		case TokenType::ASTERISK:
			break;
		case TokenType::FORWARD_SLASH:
			break;
		}
	}

	return Expression();
}

Statement Parser::parseExit(const Token* start, const Token* end)
{
	Statement statement;

	// *start is definitely an exit token
	const Token* token = start + 1;

	if (token == end)
	{
		exitWithError(ErrorCode::EXPECTED_AN_EXPRESSION, _args->inputFile, token->row, token->col);
		return { StatementType::EMPTY };
	}

	statement.type = StatementType::EXIT;
	statement.a = createPtr<Expression>(std::forward<Expression>(parseExpr(token, end)));

	return statement;
}

Statement Parser::parseAssign(const Token* start, const Token* end)
{
	Statement statement;

	// *start is definitely a variable token
	const Token* token = start + 1;

	if (token >= end || token->tokenType != TokenType::ASSIGN)
	{
		HE_DEBUG_BREAK;
		exitWithError(ErrorCode::EXPECTED_EQUALS, _args->inputFile, token->row, token->col);
		return { StatementType::EMPTY };
	}

	// a - variable to assign an expression to
	// b - the expression

	statement.type = StatementType::ASSIGN;
	statement.a = createPtr<Expression>();
	statement.a->type = ExprType::VARIABLE;
	statement.a->variable = start->variable;
	statement.b = createPtr<Expression>(std::forward<Expression>(parseExpr(token + 1, end)));

	return statement;
}