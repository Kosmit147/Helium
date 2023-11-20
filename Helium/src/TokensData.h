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