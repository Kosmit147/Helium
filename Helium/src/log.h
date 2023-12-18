// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#ifdef _DEBUG

#include "pch.h"

#include "Token.h"
#include "parsing.h"

static usize indentLevel = 0;

inline std::ostream& indent(std::ostream& stream)
{
	for (it i = 0; i < indentLevel; i++)
		stream << '\t';

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Literal& literal)
{
	indent(stream);

	stream << "{ literalType: " << getHeTypeStr(literal.type) << ", value: ";

	switch (literal.type)
	{
	case HeType::I32:
		stream << literal.valI32;
		break;
	default:
		HE_DEBUG_BREAK;
	}

	stream << " }";

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Variable& variable)
{
	indent(stream);

	stream << "{ variableType: " << getHeTypeStr(variable.type) << ", name: "
		<< variable.name << " }";

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	indent(stream);

	stream << "{ tokenType: " << Token::getTokenTypeStr(token.type) << ", "
		<< token.filePos.row << ":" << token.filePos.col << " }";

	indentLevel++;

	switch (token.type)
	{
	case Token::TokenType::LITERAL:
		stream << std::endl;
		stream << *token.literal;
		break;
	case Token::TokenType::VARIABLE:
		stream << std::endl;
		stream << *token.variable;
		break;
	}

	indentLevel--;

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Expression& expr)
{
	indent(stream);

	stream << "{ expression }" << std::endl;

	indentLevel++;

	for (const Token& token : expr.rpn)
		stream << token << std::endl;

	indentLevel--;

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Statement& statement)
{
	indent(stream);

	stream << "{ statementType: " << Statement::getStatementTypeStr(statement.type) << ", "
		<< statement.row << " }";

	indentLevel++;

	if (statement.a)
	{
		stream << std::endl;
		stream << *statement.a;
	}

	if (statement.b)
	{
		stream << std::endl;
		stream << *statement.b;
	}

	indentLevel--;

	return stream;
}

inline void printTokens(const std::vector<Token>& tokens)
{
	std::cout << "--- TOKENS ---" << std::endl;

	indentLevel = 0;

	for (const Token& token : tokens)
		std::cout << token << std::endl;
}

inline void printCode(std::string_view code)
{
	std::cout << "--- SOURCE ---" << std::endl;

	indentLevel = 0;

	std::cout << code << std::endl;
}

inline void printStatements(const std::vector<Statement>& statements)
{
	std::cout << "--- STATEMENTS ---" << std::endl;

	indentLevel = 0;

	for (const Statement& stmt : statements)
		std::cout << stmt << std::endl;
}

#endif // _DEBUG