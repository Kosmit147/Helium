#pragma once

#include "args.h"

class Compiler
{
public:
	static void compileIntoFile(const Args& args, std::string_view fileName);

private:
	static const Args* args;
};