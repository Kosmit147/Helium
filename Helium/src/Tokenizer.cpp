// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "Tokenizer.h"

#include <stdexcept>

#include "global.h"
#include "error.h"
#include "asciiUtils.h"

using TokenType = Token::TokenType;

std::string_view Tokenizer::_input;
usize Tokenizer::_row = 0;
usize Tokenizer::_col = 0;
usize Tokenizer::_colOffset = 0;
usize Tokenizer::_inputIndex = 0;
usize Tokenizer::_semicolonCount = 0;

std::vector<Token> Tokenizer::tokenize(std::string_view input)
{
	_input = input;

	std::vector<Token> tokens;

	for (; _inputIndex < _input.length(); _inputIndex++)
	{
		char character = _input[_inputIndex];
		CharacterType type = getCharacterType(character);

		if (character == '\n')
		{
			// TODO: this doesn't seem correct...
			// shouldn't it be _colOffset += _inputIndex - _colOffset;?
			_colOffset += _inputIndex + 1;
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

	return tokens;
}

Token Tokenizer::readKeywordOrVar()
{
	usize tokenStartCol = _inputIndex - _colOffset;
	std::string tokenStr = "";
	
	for (; _inputIndex < _input.length(); _inputIndex++)
	{
		char character = _input[_inputIndex];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::ALPHABETIC || type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			_inputIndex--;

			auto search = Token::tokenTypeMap.find(tokenStr);
			if (search != Token::tokenTypeMap.end())
			{
				return { search->second, { _row, tokenStartCol } };
			}
			else
			{
				Token token(TokenType::VARIABLE, { _row, tokenStartCol });
				token.variable = &global::variables.emplaceBack(HeType::I32, std::move(tokenStr));
				return token;
			}
		}
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, { _row, tokenStartCol });
	return Token::errorToken;
}

Token Tokenizer::readI32Literal()
{
	usize tokenStartCol = _inputIndex - _colOffset;

	std::string tokenStr = "";
	for (; _inputIndex < _input.length(); _inputIndex++)
	{
		char character = _input[_inputIndex];
		CharacterType type = getCharacterType(character);

		if (type == CharacterType::DIGIT)
		{
			tokenStr += character;
		}
		else
		{
			_inputIndex--;

			i32 value;

			try
			{
				value = std::stoi(tokenStr);
			}
			catch (std::out_of_range&)
			{
				exitWithError(ErrorCode::I32_LITERAL_OUT_OF_RANGE, { _row, tokenStartCol });
			}

			Token token(TokenType::LITERAL, { _row, tokenStartCol });
			token.literal = &global::literals.emplaceBack(value);
			return token;
		}
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, { _row, tokenStartCol });
	return Token::errorToken;
}

Token Tokenizer::readSpecialChar()
{
	char character = _input[_inputIndex];

	auto search = Token::tokenTypeMap.find({ character });

	if (search != Token::tokenTypeMap.end())
	{
		if (search->second == TokenType::SEMICOLON)
			_semicolonCount++;

		return { search->second, { _row, _inputIndex - _colOffset } };
	}
	else
	{
		exitWithError(ErrorCode::UNEXPECTED_CHARACTER, { _row, _col });
	}

	HE_DEBUG_BREAK;
	exitWithError(ErrorCode::FAILED_TO_TOKENIZE, { _row, _col });
	return Token::errorToken;
}