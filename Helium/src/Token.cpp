#include "Token.h"

#include <string>
#include <stdexcept>
#include <utility>

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

const Token Token::errorToken = { TokenType::ERR, 0, 0 };

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
			tokens.push_back(std::forward<Token>(readKeywordOrVar()));
		}
		else if (type == CharacterType::DIGIT)
		{
			// int literal
			tokens.push_back(std::forward<Token>(readI32Literal()));
		}
		else if (type == CharacterType::SPECIAL_CHAR)
		{
			tokens.push_back(std::forward<Token>(readSpecialChar()));
		}
	}

	return tokens;
}

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

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE);
	return Token::errorToken;
}

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

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE);
	return Token::errorToken;
}

Token Tokenizer::readSpecialChar()
{
	char character = input[index];

	auto search = Token::tokenTypeMap.find({ character });

	if (search != Token::tokenTypeMap.end())
		return { search->second, row, index - colOffset };
	else
		exitWithError(ErrorCode::UNEXPECTED_CHARACTER);

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE);
	return Token::errorToken;
}

Token::Token(TokenType tokenType, usize row, usize col)
	: tokenType(tokenType), row(row), col(col), literal(nullptr), variable(nullptr) {}

Token::Token(const Token& other)
	: tokenType(other.tokenType), row(other.row), col(other.col)
{
	if (other.literal)
		literal = createPtr<Literal>(other.literal->type, other.literal->value);

	if (other.variable)
		variable = createPtr<Variable>(other.variable->type);
}

#ifdef _DEBUG

#include <iostream>

const std::unordered_map<TokenType, std::string> Token::tokenNameMap = {
	{ TokenType::ERR, ERR_STR },
	{ TokenType::EXIT, "exit" },
	{ TokenType::LITERAL, "literal" },
	{ TokenType::SEMICOLON, "semicolon" },
};

std::string Token::getTokenStr(TokenType type)
{
	auto search = tokenNameMap.find(type);

	if (search != tokenNameMap.end())
		return search->second;

	HE_DEBUG_BREAK;
	return ERR_STR;
}

#endif