// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>

#include "TokensData.h"
#include "Tokenizer.h"
#include "Arena.h"

struct Expression
{
	enum class Type : u8
	{
		ERR_TYPE,
		EMPTY,
		LITERAL,
		VARIABLE,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
	};

	Type type = Type::ERR_TYPE;
	Literal* literal = nullptr;
	Variable* variable = nullptr;
	Expression* a = nullptr;
	Expression* b = nullptr;
};

struct Statement
{
	enum class Type : u8
	{
		EMPTY,
		EXIT,
		ASSIGN,
	};

	Type type = Type::EMPTY;
	Expression* a = nullptr;
	Expression* b = nullptr;
};

class Parser
{
public:
	[[nodiscard]] static std::vector<Statement> parseTokens(const Args& setArgs, 
		const TokensData& tokensData);
	[[nodiscard]] static Statement parseStatement(const Token* start, const Token* end);
	[[nodiscard]] static Expression parseExpr(const Token* start, const Token* end);
	[[nodiscard]] static Statement parseExit(const Token* start, const Token* end);
	[[nodiscard]] static Statement parseAssign(const Token* start, const Token* end);

private:
	static const Args* _args;
	static Ref<Arena<Literal>> _literals;
	static Ref<Arena<Variable>> _variables;
	static Ref<Arena<Expression>> _expressions;
};