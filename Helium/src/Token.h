#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>

#include "common.h"
#include "HeType.h"

struct Literal
{
	HeType type;
	usize value;
};

struct Variable
{
	HeType type;

	// TODO
};

struct Token
{
	enum class TokenType
	{
		ERR,
		EXIT,
		LITERAL,
		SEMICOLON,
		// when adding new tokens, update tokenTypeMap and tokenNameMap definitions
	};

	static const std::unordered_map<std::string, TokenType> tokenTypeMap;

	TokenType tokenType;
	usize row;
	usize col;
	Ptr<Literal> literal;
	Ptr<Variable> variable;

	Token(TokenType tokenType, usize row, usize col);
	Token(const Token& other);

	static const Token errorToken;

#ifdef _DEBUG
	static const std::unordered_map<TokenType, std::string> tokenNameMap;
	static std::string getTokenStr(TokenType type);
#endif
};

class Tokenizer
{
public:
	static std::vector<Token> tokenize(std::string_view input);
	
private:
	static std::string_view input;
	static usize row;
	static usize col;
	static usize colOffset;
	static it index;

private:
	static Token readKeywordOrVar();
	static Token readI32Literal();
	static Token readSpecialChar();

public:
	Tokenizer() = delete;
};