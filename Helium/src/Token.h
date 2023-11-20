// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <unordered_map>

#include "common.h"
#include "heType.h"
#include "error.h"

struct Literal
{
	const HeType type;

	union {
		i32 valI32;
	};

	template<typename T>
	inline Literal(HeType type, T value) : type(type)
	{
		switch (type)
		{
		case HeType::I32:
			valI32 = value;
			break;
		default:
			HE_DEBUG_BREAK;
			exitWithError(ErrorCode::INCORRECT_LITERAL_TYPE);
			break;
		}
	}

	template<typename T>
	inline T getValue(HeType valueType) const
	{
		switch (valueType)
		{
		case HeType::I32:
			return (T)valI32;
			break;
		default:
			HE_DEBUG_BREAK;
			return (T)0;
			break;
		}
	}
};

struct Variable
{
	const HeType type;
	const std::string name;

	Variable(HeType type, std::string_view name);
	Variable(HeType type, std::string&& name);
};

struct Token
{
	enum class TokenType : u8
	{
		ERR,
		EXIT,
		LITERAL,
		VARIABLE,
		SEMICOLON,
		OPEN_PAREN,
		CLOSE_PAREN,
		PLUS,
		MINUS,
		ASTERISK,
		FORWARD_SLASH,
		EQUALS,
		// when adding new tokens, update tokenTypeMap and tokenNameMap definitions
	};

	static const std::unordered_map<std::string, TokenType> tokenTypeMap;
	static const Token errorToken;

	const TokenType tokenType;
	const usize row;
	const usize col;
	Literal* literal;
	Variable* variable;

	Token(TokenType tokenType, usize row, usize col);
	Token(const Token& other);

#ifdef _DEBUG
	static const std::unordered_map<TokenType, std::string> tokenNameMap;
	[[nodiscard]] static const char* getTokenStr(TokenType type);
#endif

private:
	Token& operator=(const Token& other) = delete;
};