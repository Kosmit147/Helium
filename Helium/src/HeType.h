// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

enum class HeType
{
	I32
};

#ifdef _DEBUG

#include <string>

[[nodiscard]] constexpr const char* getHeTypeStr(HeType type);

#endif