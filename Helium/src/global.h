#pragma once

#include "Args.h"
#include "Arena.h"
#include "parsing.h"

namespace global {
	inline Args args;
	inline Arena<Literal> literals(1000);
	inline Arena<Variable> variables(1000);
	inline Arena<Expression> expressions(1000);
}