#pragma once

enum class CharacterType
{
	WHITESPACE,
	ALPHABETIC,
	DIGIT,
	SPECIAL_CHAR,
	OTHER,
};

[[nodiscard]] inline bool isWhiteSpace(char c);
[[nodiscard]] inline bool isSpecialChar(char c);
[[nodiscard]] CharacterType getCharacterType(char c);