// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "asciiUtils.h"

[[nodiscard]] inline static bool isWhiteSpace(char c)
{
	static constexpr char whiteSpaceChars[] = {
		' ', '\f', '\n',
		'\r', '\t', '\v',
	};

	for (char whiteSpaceChar : whiteSpaceChars)
		if (c == whiteSpaceChar)
			return true;

	return false;
};

[[nodiscard]] inline static bool isSpecialChar(char c)
{
	return	(c >= ' ' && c <= '/') ||
		(c >= ':' && c <= '@') ||
		(c >= '[' && c <= '`') ||
		(c >= '{' && c <= '~');
};

CharacterType getCharacterType(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return CharacterType::ALPHABETIC;
	else if (c >= '0' && c <= '9')
		return CharacterType::DIGIT;
	else if (isWhiteSpace(c))
		return CharacterType::WHITESPACE;
	else if (isSpecialChar(c))
		return CharacterType::SPECIAL_CHAR;

	return CharacterType::OTHER;
};