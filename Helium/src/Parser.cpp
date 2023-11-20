// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Parser.h"
#include "error.h"

using TokenType = Token::TokenType;
using ExprType = Expression::Type;
using StatementType = Statement::Type;

const Args* Parser::_args = nullptr;
Ref<Arena<Literal>> Parser::_literals = nullptr;
Ref<Arena<Variable>> Parser::_variables = nullptr;
Ref<Arena<Expression>> Parser::_expressions = nullptr;

std::vector<Statement> Parser::parseTokens(const Args& args, const TokensData& tokensData)
{
	_args = &args;
	_literals = tokensData.literals;
	_variables = tokensData.variables;
	_expressions = createRef<Arena<Expression>>(10000);

	std::vector<Statement> statements;
	statements.reserve(Tokenizer::semicolonCount());

	const std::vector<Token>& tokens = tokensData.tokens;
	const Token* stmtStartToken = &tokens[0];

	for (const Token& token : tokens)
	{
		if (token.tokenType == TokenType::SEMICOLON)
		{
			statements.push_back(parseStatement(stmtStartToken, &token));
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
	if (start->tokenType == TokenType::OPEN_PAREN &&
		end->tokenType == TokenType::CLOSE_PAREN)
	{
		// "(...)"
		if (end - start >= 2)
		{
			start++;
			end--;
		}
		// empty expression "()"
		else
		{
			return { ExprType::EMPTY };
		}
	}

	auto findClosingParen = [&](const Token* openParen)
	{
		for (const Token* i = openParen + 1; i <= end; i++)
			if (i->tokenType == TokenType::CLOSE_PAREN)
				return i;

		exitWithError(ErrorCode::EXPECTED_A_CLOSING_PAREN, _args->inputFile, openParen->row, openParen->col);
		return (const Token*)nullptr;
	};

	Expression expr;

	for (const Token* token = start; token <= end; token++)
	{
		const Token* closingParen;

		switch (token->tokenType)
		{
		case TokenType::OPEN_PAREN:
			closingParen = findClosingParen(token);
			expr.a = &_expressions->pushBack(parseExpr(token + 1, closingParen - 1));
			break;
		case TokenType::CLOSE_PAREN:
			exitWithError(ErrorCode::UNEXPECTED_CHARACTER, _args->inputFile, token->row, token->col);
			break;
		case TokenType::LITERAL:
			break;
		case TokenType::VARIABLE:
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
	statement.a = &_expressions->pushBack(parseExpr(token, end));

	return statement;
}

Statement Parser::parseAssign(const Token* start, const Token* end)
{
	Statement statement;

	// *start is definitely a variable token
	const Token* token = start + 1;

	if (token >= end || token->tokenType != TokenType::EQUALS)
	{
		HE_DEBUG_BREAK;
		exitWithError(ErrorCode::EXPECTED_EQUALS, _args->inputFile, token->row, token->col);
		return { StatementType::EMPTY };
	}

	// a - variable to assign an expression to
	// b - the expression

	statement.type = StatementType::ASSIGN;
	statement.a = &_expressions->pushBack({ .type = ExprType::VARIABLE , .variable = start->variable });
	statement.b = &_expressions->pushBack(parseExpr(token + 1, end));

	return statement;
}