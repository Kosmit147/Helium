#include "Token.h"

#include "common.h"
#include "asciiUtils.h"
#include "errorCodes.h"

using TokenType = Token::TokenType;

const std::unordered_map<std::string, TokenType> Token::tokenTypeMap =
{
	{ "exit", TokenType::EXIT },
	{ ";", TokenType::SEMICOLON },
};

std::string_view Tokenizer::input;
usize Tokenizer::row = 0;
usize Tokenizer::col = 0;
usize Tokenizer::colOffset = 0;
it Tokenizer::index = 0;

std::vector<Token> Tokenizer::tokenize(std::string_view input)
{
	Tokenizer::input = input;

	std::vector<Token> tokens;

	for (; index < input.length(); index++)
	{
		char character = input[index];
		CharacterType type = getCharacterType(character);

		if (character == '\n')
		{
			colOffset += index;
			row++;
			continue;
		}
		else if (type == CharacterType::WHITESPACE)
		{
			continue;
		}
		else if (type == CharacterType::ALPHABETIC)
		{
			// we are reading either a keyword or a variable name
			tokens.push_back(readKeywordOrVar());
		}
		else if (type == CharacterType::DIGIT)
		{
			// int literal
			tokens.push_back(readI32Literal());
		}
		else if (type == CharacterType::SPECIAL_CHAR)
		{
			tokens.push_back(readSpecialChar());
		}
	}

	return tokens;
}

#pragma warning(suppress: HE_RETURN_CHECK)
Token Tokenizer::readKeywordOrVar()
{
	usize tokenStartCol = index - colOffset;
	std::string tokenStr = "";
	
	for (; index < input.length(); index++)
	{
		char character = input[index];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::ALPHABETIC || type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			index--;

			auto search = Token::tokenTypeMap.find(tokenStr);
			if (search != Token::tokenTypeMap.end())
			{
				return { search->second, row, tokenStartCol };
			}
			else
			{
				// TODO: variable
			}
		}
	}

	HE_DEBUG_BREAK
}

#pragma warning(suppress: HE_RETURN_CHECK)
Token Tokenizer::readI32Literal()
{
	usize tokenStartCol = index - colOffset;

	std::string tokenStr = "";
	for (; index < input.length(); index++)
	{
		char character = input[index];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			index--;

			i32 value;

			try
			{
				value = std::stoi(tokenStr);
			}
			catch (std::out_of_range)
			{
				exitWithError(ErrorCode::I32_LITERAL_OUT_OF_RANGE);
			}

			Token token(TokenType::LITERAL, row, tokenStartCol);
			token.literal = createPtr<Literal>(HeType::I32, value);
			return token;
		}
	}

	HE_DEBUG_BREAK
}

#pragma warning(suppress: HE_RETURN_CHECK)
Token Tokenizer::readSpecialChar()
{
	char character = input[index];

	auto search = Token::tokenTypeMap.find({ character });

	if (search != Token::tokenTypeMap.end())
		return { search->second, row, index - colOffset };
	else
		exitWithError(ErrorCode::UNEXPECTED_CHARACTER);
}

Token::Token(TokenType tokenType, usize row, usize col)
	: tokenType(tokenType), row(row), col(col), literal(nullptr), variable(nullptr) {}

Literal::Literal(HeType type, usize value) 
	: type(type), value(value) {}

#ifdef _DEBUG

#include <iostream>

const std::unordered_map<TokenType, std::string> Token::tokenNameMap = {
	{ TokenType::EXIT, "exit" },
	{ TokenType::LITERAL, "literal" },
	{ TokenType::SEMICOLON, "semicolon" },
};

#pragma warning(suppress: HE_RETURN_CHECK)
std::string Token::getTokenStr(TokenType type)
{
	auto search = tokenNameMap.find(type);

	if (search != tokenNameMap.end())
		return search->second;

	HE_DEBUG_BREAK
}

#endif