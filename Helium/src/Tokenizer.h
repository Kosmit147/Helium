// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <string_view>

#include "Token.h"
#include "TokensData.h"
#include "common.h"
#include "Args.h"
#include "Arena.h"

class Tokenizer
{
public:
	[[nodiscard]] static TokensData tokenize(const Args& args, std::string_view input);
	[[nodiscard]] inline static usize semicolonCount() { return _semicolonCount; }
	
private:
	static std::string_view _input;
	static usize _row;
	static usize _col;
	static usize _colOffset;
	static it _index;
	static const Args* _args;
	static usize _semicolonCount;

	static Ref<Arena<Literal>> _literals;
	static Ref<Arena<Variable>> _variables;

private:
	[[nodiscard]] static Token readKeywordOrVar();
	[[nodiscard]] static Token readI32Literal();
	[[nodiscard]] static Token readSpecialChar();

public:
	Tokenizer() = delete;
};