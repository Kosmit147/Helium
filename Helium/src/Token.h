// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

#include "heType.h"

struct Literal
{
	const HeType type;

	union {
		i32 valI32;
		/* TODO: add more types */
	};

	inline Literal(i32 value) : type(HeType::I32), valI32(value) {}

	inline bool operator==(const Literal& other) const { 
		return type == other.type && valI32 == other.valI32; 
	}
};

struct Variable
{
	const HeType type;
	const std::string name;

	// TODO: constructor which only initializes the name

	inline Variable(HeType type, std::string_view name)
		: type(type), name(name) {}

	inline Variable(HeType type, std::string&& name)
		: type(type), name(std::move(name)) {}

	inline auto operator<=>(const Variable&) const = default;
};

struct Token
{
	struct FilePosition 
	{
		usize row;
		usize col;

		inline auto operator<=>(const FilePosition&) const = default;
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
		// when adding new tokens, update tokenTypeMap, tokenNameMap definitions and TokenizerTest
	};

	static const std::unordered_map<std::string, TokenType> tokenTypeMap;
	static const Token errorToken;

	TokenType type;
	FilePosition filePos;

	union {
		void* null;
		Literal* literal;
		Variable* variable;
	};

	inline Token(TokenType type, FilePosition filePos)
		: type(type), filePos(filePos), null(nullptr) {}

	inline Token(TokenType type, FilePosition filePos, Literal* literal) 
		: type(type), filePos(filePos), literal(literal) {}

	inline Token(TokenType type, FilePosition filePos, Variable* variable)
		: type(type), filePos(filePos), variable(variable) {}

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