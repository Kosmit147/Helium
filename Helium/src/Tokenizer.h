// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "common.h"
#include "HeType.h"
#include "Args.h"
#include "error.h"

struct Literal
{
	const HeType type;

	union {
		i32 value;
	};

	template<typename T>
	inline Literal(HeType type, T newVal) : type(type)
	{
		switch (type)
		{
		case HeType::I32:
			value = newVal;
			break;
		default:
			HE_DEBUG_BREAK;
			exitWithError(ErrorCode::INCORRECT_LITERAL_TYPE);
			break;
		}
	}

	template<typename T>
	inline T getValue() const
	{
		return (T)value;
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
		ASSIGN,
		// when adding new tokens, update tokenTypeMap and tokenNameMap definitions
	};

	static const std::unordered_map<std::string, TokenType> tokenTypeMap;

	const TokenType tokenType;
	const usize row;
	const usize col;
	Ref<Literal> literal;
	Ref<Variable> variable;

	Token(TokenType tokenType, usize row, usize col);
	Token(const Token& other);

	static const Token errorToken;

#ifdef _DEBUG
	static const std::unordered_map<TokenType, std::string> tokenNameMap;
	[[nodiscard]] static const char* getTokenStr(TokenType type);
#endif

private:
	Token& operator=(const Token& other) = delete;
};

class Tokenizer
{
public:
	[[nodiscard]] static std::vector<Token> tokenize(const Args& args, std::string_view input);
	[[nodiscard]] inline static usize semicolonCount() { return _semicolonCount; }
	
private:
	static std::string_view _input;
	static usize _row;
	static usize _col;
	static usize _colOffset;
	static it _index;
	static const Args* _args;
	static usize _semicolonCount;

private:
	[[nodiscard]] static Token readKeywordOrVar();
	[[nodiscard]] static Token readI32Literal();
	[[nodiscard]] static Token readSpecialChar();

public:
	Tokenizer() = delete;
};