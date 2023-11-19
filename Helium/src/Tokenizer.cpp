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

const std::unordered_map<std::string, TokenType> Token::tokenTypeMap =
{
	{ "exit", TokenType::EXIT },
	{ ";", TokenType::SEMICOLON },
	{ "(", TokenType::OPEN_PAREN },
	{ ")", TokenType::CLOSE_PAREN },
	{ "=", TokenType::ASSIGN },
};

std::string_view Tokenizer::_input;
usize Tokenizer::_row = 0;
usize Tokenizer::_col = 0;
usize Tokenizer::_colOffset = 0;
it Tokenizer::_index = 0;
const Args* Tokenizer::_args = nullptr;
usize Tokenizer::_semicolonCount = 0;

const Token Token::errorToken = { TokenType::ERR, 0, 0 };

Variable::Variable(HeType type, std::string_view name)
	: type(type), name(name) {}

Variable::Variable(HeType type, std::string&& name)
	: type(type), name(std::move(name)) {}

std::vector<Token> Tokenizer::tokenize(const Args& args, std::string_view input)
{
	_args = &args;
	_input = input;

	std::vector<Token> tokens;

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
			tokens.emplace_back(std::forward<Token>(readKeywordOrVar()));
		}
		else if (type == CharacterType::DIGIT)
		{
			// int literal
			tokens.emplace_back(std::forward<Token>(readI32Literal()));
		}
		else if (type == CharacterType::SPECIAL_CHAR)
		{
			// special character
			tokens.emplace_back(std::forward<Token>(readSpecialChar()));
		}
	}

	return tokens;
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
				token.variable = createRef<Variable>(HeType::I32, std::move(tokenStr));
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
			token.literal = createRef<Literal>(HeType::I32, value);
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

Token::Token(TokenType tokenType, usize row, usize col)
	: tokenType(tokenType), row(row), col(col), literal(nullptr), variable(nullptr) {}

Token::Token(const Token& other)
	: tokenType(other.tokenType), row(other.row), col(other.col)
{
	if (other.literal)
		literal = createRef<Literal>(other.literal->type, other.literal->value);

	if (other.variable)
		variable = createRef<Variable>(other.variable->type, other.variable->name);
}

#ifdef _DEBUG

#include <iostream>

const std::unordered_map<TokenType, std::string> Token::tokenNameMap = {
	{ TokenType::ERR, ERR_STR },
	{ TokenType::EXIT, "exit" },
	{ TokenType::LITERAL, "literal" },
	{ TokenType::VARIABLE, "variable" },
	{ TokenType::SEMICOLON, "semicolon" },
	{ TokenType::OPEN_PAREN, "openingParenthesis" },
	{ TokenType::CLOSE_PAREN, "closingParenthesis" },
	{ TokenType::ASSIGN, "assignment" },
};

const char* Token::getTokenStr(TokenType type)
{
	auto search = tokenNameMap.find(type);

	if (search != tokenNameMap.end())
		return search->second.c_str();

	HE_DEBUG_BREAK;
	return ERR_STR;
}

#endif