// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

class Compiler
{
public:
	static void compileIntoFile(std::string_view fileName);
};