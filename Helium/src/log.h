// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <iostream>

inline void printHelp()
{
	// TODO: implement help here

	std::cout << std::endl;
	std::cout << "Helium" << std::endl;
	std::cout << "TODO: Implement help page here" << std::endl;
	std::cout << std::endl;
}

#ifdef _DEBUG

#include <vector>

#include "common.h"
#include "Tokenizer.h"

inline std::ostream& operator<<(std::ostream& stream, const Literal& literal)
{
	stream << "{ literalType: " << getHeTypeStr(literal.type) << ", value: ";

	switch (literal.type)
	{
	case HeType::I32:
		stream << literal.getValue<i32>(HeType::I32);
		break;
	default:
		HE_DEBUG_BREAK;
	}

	stream << " }";

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Variable& variable)
{
	stream << "{ variableType: " << getHeTypeStr(variable.type) << ", name: "
		<< variable.name << " }";

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	stream << "{ tokenType: " << Token::getTokenStr(token.tokenType) << ", "
		<< token.row << ":" << token.col << " }";

	if (token.literal)
	{
		std::cout << std::endl;
		std::cout << '\t' << *token.literal;
	}

	if (token.variable)
	{
		std::cout << std::endl;
		std::cout << '\t' << *token.variable;
	}

	return stream;
}

inline void printTokens(const std::vector<Token>& tokens)
{
	for (const Token& token : tokens)
		std::cout << token << std::endl;
}

inline void printCode(std::string_view code)
{
	std::cout << code << std::endl;
}

#endif // _DEBUG