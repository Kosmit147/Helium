// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "common.h"
#include "heType.h"

struct Literal
{
	const HeType type;

	union {
		i32 valI32;
		/* TODO: add more types */
	};

	inline Literal(i32 value) : type(HeType::I32), valI32(value) {}
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
	struct FilePosition {
		usize row;
		usize col;
	};

	enum class TokenType : u8
	{
		NONE,
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

	TokenType tokenType;
	FilePosition filePos;

	// TODO: make this into a union

	union {
		Literal* literal;
		Variable* variable;
	};

	Token(TokenType tokenType, FilePosition filePos );

#ifdef _DEBUG
	static const std::unordered_map<TokenType, std::string> tokenNameMap;
	[[nodiscard]] static inline const char* getTokenTypeStr(TokenType type)
	{
		auto search = tokenNameMap.find(type);

		if (search != tokenNameMap.end())
			return search->second.c_str();

		HE_DEBUG_BREAK;
		return ERR_STR;
	}
#endif
};