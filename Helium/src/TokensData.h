// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>

#include "Token.h"
#include "Arena.h"

struct TokensData
{
	std::vector<Token> tokens;
	Ref<Arena<Literal>> literals;
	Ref<Arena<Variable>> variables;
};