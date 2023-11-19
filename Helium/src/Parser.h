// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>

#include "Tokenizer.h"

struct Expression
{
	enum class Type : u8
	{
		ERR_TYPE,
		VALUE,
		VARIABLE,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
	};

	Type type = Type::ERR_TYPE;
	Ref<Literal> value = nullptr;
	Ref<Variable> variable = nullptr;
	Ptr<Expression> a = nullptr;
	Ptr<Expression> b = nullptr;
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
	Ptr<Expression> a = nullptr;
	Ptr<Expression> b = nullptr;
};

// TODO: make this into a class with static functions

[[nodiscard]] std::vector<Statement> parseTokens(const Args& setArgs, const std::vector<Token>& tokens);
[[nodiscard]] Statement parseStatement(const Token* start, const Token* end);
[[nodiscard]] Expression parseExpr(const Token* start, const Token* end);
[[nodiscard]] Statement parseExit(const Token* start, const Token* end);
[[nodiscard]] Statement parseAssign(const Token* start, const Token* end);