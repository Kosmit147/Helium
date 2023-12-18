// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

#include "common.h"
#include "Token.h"

class Tokenizer
{
public:
	[[nodiscard]] static std::vector<Token> tokenize(std::string_view input);
	[[nodiscard]] inline static usize semicolonCount() { return _semicolonCount; }
	
private:
	static std::string_view _input;
	static usize _row;
	static usize _col;
	static usize _colOffset;
	static usize _inputIndex;
	static usize _semicolonCount;

private:
	[[nodiscard]] static Token readKeywordOrVar();
	[[nodiscard]] static Token readI32Literal();
	[[nodiscard]] static Token readSpecialChar();

public:
	Tokenizer() = delete;
};