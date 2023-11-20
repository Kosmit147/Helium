// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Tokenizer.h"

#include <string>
#include <stdexcept>
#include <utility>

#include "common.h"
#include "asciiUtils.h"
#include "error.h"

using TokenType = Token::TokenType;

std::string_view Tokenizer::_input;
usize Tokenizer::_row = 0;
usize Tokenizer::_col = 0;
usize Tokenizer::_colOffset = 0;
it Tokenizer::_index = 0;
const Args* Tokenizer::_args = nullptr;
usize Tokenizer::_semicolonCount = 0;

Ref<Arena<Literal>> Tokenizer::_literals = nullptr;
Ref<Arena<Variable>> Tokenizer::_variables = nullptr;

TokensData Tokenizer::tokenize(const Args& args, std::string_view input)
{
	_args = &args;
	_input = input;

	TokensData tokensData;
	std::vector<Token>& tokens = tokensData.tokens;
	tokensData.literals = createRef<Arena<Literal>>(1000);
	tokensData.variables = createRef<Arena<Variable>>(1000);
	_literals = tokensData.literals;
	_variables = tokensData.variables;

	for (; _index < _input.length(); _index++)
	{
		char character = _input[_index];
		CharacterType type = getCharacterType(character);

		if (character == '\n')
		{
			_colOffset += _index + 1;
			_row++;
			continue;
		}
		else if (type == CharacterType::WHITESPACE)
		{
			continue;
		}
		else if (type == CharacterType::ALPHABETIC)
		{
			// we are reading either a keyword or a variable name
			tokens.emplace_back(readKeywordOrVar());
		}
		else if (type == CharacterType::DIGIT)
		{
			// int literal
			tokens.emplace_back(readI32Literal());
		}
		else if (type == CharacterType::SPECIAL_CHAR)
		{
			// special character
			tokens.emplace_back(readSpecialChar());
		}
	}

	return tokensData;
}

Token Tokenizer::readKeywordOrVar()
{
	usize tokenStartCol = _index - _colOffset;
	std::string tokenStr = "";
	
	for (; _index < _input.length(); _index++)
	{
		char character = _input[_index];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::ALPHABETIC || type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			_index--;

			auto search = Token::tokenTypeMap.find(tokenStr);
			if (search != Token::tokenTypeMap.end())
			{
				return { search->second, _row, tokenStartCol };
			}
			else
			{
				Token token(TokenType::VARIABLE, _row, tokenStartCol);
				token.variable = &_variables->emplaceBack(HeType::I32, std::move(tokenStr));
				return token;
			}
		}
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, _args->inputFile, _row, tokenStartCol);
	return Token::errorToken;
}

Token Tokenizer::readI32Literal()
{
	usize tokenStartCol = _index - _colOffset;

	std::string tokenStr = "";
	for (; _index < _input.length(); _index++)
	{
		char character = _input[_index];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			_index--;

			i32 value;

			try
			{
				value = std::stoi(tokenStr);
			}
			catch (std::out_of_range&)
			{
				exitWithError(ErrorCode::I32_LITERAL_OUT_OF_RANGE, _args->inputFile, _row, tokenStartCol);
			}

			Token token(TokenType::LITERAL, _row, tokenStartCol);
			token.literal = &_literals->emplaceBack(HeType::I32, value);
			return token;
		}
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, _args->inputFile, _row, tokenStartCol);
	return Token::errorToken;
}

Token Tokenizer::readSpecialChar()
{
	char character = _input[_index];

	auto search = Token::tokenTypeMap.find({ character });

	if (search != Token::tokenTypeMap.end())
	{
		if (search->second == TokenType::SEMICOLON)
			_semicolonCount++;
		return { search->second, _row, _index - _colOffset };
	}
	else
	{
		exitWithError(ErrorCode::UNEXPECTED_CHARACTER, _args->inputFile, _row, _col);
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, _args->inputFile, _row, _col);
	return Token::errorToken;
}