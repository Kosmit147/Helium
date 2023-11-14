#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "common.h"
#include "HeType.h"
#include "args.h"

struct Literal
{
public:
	const HeType type;
	const std::variant<i32> value;

	template<typename T>
	inline T getValue() const
	{
		return std::get<T>(value);
	}
};

struct Variable
{
	const HeType type;
	const std::string name;

	Variable(HeType type, const std::string& name);
	Variable(HeType type, std::string&& name);
};

struct Token
{
	enum class TokenType
	{
		ERR,
		EXIT,
		LITERAL,
		VARIABLE,
		SEMICOLON,
		// when adding new tokens, update tokenTypeMap and tokenNameMap definitions
	};

	static const std::unordered_map<std::string, TokenType> tokenTypeMap;

	const TokenType tokenType;
	const usize row;
	const usize col;
	Ptr<Literal> literal;
	Ptr<Variable> variable;

	Token(TokenType tokenType, usize row, usize col);
	Token(const Token& other);

	static const Token errorToken;

#ifdef _DEBUG
	static const std::unordered_map<TokenType, std::string> tokenNameMap;
	[[nodiscard]] static std::string getTokenStr(TokenType type);
#endif
};

class Tokenizer
{
public:
	[[nodiscard]] static std::vector<Token> tokenize(const Args& args, std::string_view input);
	
private:
	static std::string_view input;
	static usize row;
	static usize col;
	static usize colOffset;
	static it index;
	static const Args* args;

private:
	[[nodiscard]] static Token readKeywordOrVar();
	[[nodiscard]] static Token readI32Literal();
	[[nodiscard]] static Token readSpecialChar();

public:
	Tokenizer() = delete;
};