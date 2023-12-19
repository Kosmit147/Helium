#pragma once

#include "Args.h"
#include "stl/TypedArena.h"
#include "parsing.h"

namespace global {
	inline Args args;
	inline TypedArena<Literal> literals(1000);
	inline TypedArena<Variable> variables(1000);
	inline TypedArena<Expression> expressions(1000);
}