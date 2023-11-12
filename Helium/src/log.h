#pragma once

#ifdef _DEBUG

#include <iostream>
#include <vector>

#include "common.h"
#include "Token.h"

inline std::ostream& operator<<(std::ostream& stream, const Literal& literal)
{
	stream << "{ literalType:" << getHeTypeStr(literal.type) << ",value:"
		<< literal.value << " }";

	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	stream << "{ tokenType:" << Token::getTokenStr(token.tokenType) << ",row:"
		<< token.row << ",col:" << token.col << " }";

	if (token.literal)
	{
		std::cout << std::endl;
		std::cout << '\t' << *token.literal;
	}

	return stream;
}

inline void printTokens(const std::vector<Token>& tokens)
{
	for (const Token& token : tokens)
		std::cout << token << std::endl;
}

inline void printCode(std::string_view heCode)
{
	std::cout << heCode << std::endl;
}

#endif