#pragma once

enum class HeType
{
	I32
};

#ifdef _DEBUG

#include <string>

[[nodiscard]] std::string getHeTypeStr(HeType type);

#endif