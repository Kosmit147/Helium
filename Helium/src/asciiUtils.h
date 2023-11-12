#pragma once

enum class CharacterType
{
	WHITESPACE,
	ALPHABETIC,
	DIGIT,
	SPECIAL_CHAR,
	OTHER,
};

inline bool isWhiteSpace(char c);
inline bool isSpecialChar(char c);
CharacterType getCharacterType(char c);