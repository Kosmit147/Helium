#pragma once

enum class HeType
{
	I32
};

#ifdef _DEBUG

#include <string>

std::string getHeTypeStr(HeType type);

#endif