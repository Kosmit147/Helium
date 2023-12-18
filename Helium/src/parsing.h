#pragma once

#include "pch.h"

#include "common.h"
#include "Token.h"

struct TokenView
{
	const Token* first;
	const Token* last;

	inline const Token* begin() const { return first; }
	inline const Token* end() const { return last + 1; }
};

struct Expression
{
	// tokens are stored in reverse polish notation
	std::vector<Token> rpn;
};

struct ExprData
{
	Expression expr;
	TokenView exprSpan;
};

struct Statement
{
	enum class Type : u8
	{
		EMPTY,
		EXIT,
		ASSIGN,
		// when adding new types, update statementNameMap definition
	};

	Type type = Type::EMPTY;

	Expression* a = nullptr;
	Expression* b = nullptr;

	usize row = 0;

#ifdef _DEBUG
	static const std::unordered_map<Type, std::string> statementNameMap;
	[[nodiscard]] inline static const char* getStatementTypeStr(Type type) 
	{
		auto search = statementNameMap.find(type);

		if (search != statementNameMap.end())
			return search->second.c_str();

		HE_DEBUG_BREAK;
		return ERR_STR;
	}
#endif
};
