// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "common.h"

enum class CharacterType
{
	WHITESPACE,
	ALPHABETIC,
	DIGIT,
	SPECIAL_CHAR,
	OTHER,
};

[[nodiscard]] CharacterType getCharacterType(char c);