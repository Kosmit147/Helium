// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>

#include "Token.h"

struct Expression
{
	enum class Type
	{
		VALUE,
		VARIABLE,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
	};

	Type type;
	Ptr<Literal> value;
	Ptr<Variable> variable;
	Ptr<Expression> a;
	Ptr<Expression> b;
};

struct Statement
{
	enum class Type
	{
		EMPTY,
		EXIT,
		ASSIGN,
	};

	Type type;
	Ptr<Expression> a;
	Ptr<Expression> b;
};

[[nodiscard]] std::vector<Statement> parseTokens(const Args& setArgs, const std::vector<Token>& tokens);
[[nodiscard]] Statement parseStatement(const Token* start, const Token* end);
[[nodiscard]] Expression parseExpr(const Token* start, const Token* end);