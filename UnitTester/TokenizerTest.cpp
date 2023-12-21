#include "pch.h"

#include "Helium/src/Tokenizer.h"
#include "Helium/src/Token.h"
#include "Helium/src/log.h"

using TokenType = Token::TokenType;
using FilePosition = Token::FilePosition;

inline static bool tokensMatch(const Token& a, const Token& b)
{
	if (a.type != b.type)
		return false;

	if (a.filePos != b.filePos)
		return false;

	switch (a.type)
	{
	case TokenType::LITERAL:
		if (*a.literal != *b.literal)
			return false;
		break;
	case TokenType::VARIABLE:
		if (*a.variable != *b.variable)
			return false;
		break;
	default:
		// shouldn't point to a variable or a literal
		if (a.null != nullptr)
			return false;
		break;
	}

	return true;
}

MATCHER_P(TokenVectorMatches, vec, "Tokens vector does not match expected vector")
{
	if (arg.size() != vec.size())
		return false;

	for (it i = 0; i < arg.size(); i++)
	{
		const Token& resultToken = arg[i];
		const Token& expectedToken = vec[i];

		if (!tokensMatch(resultToken, expectedToken))
			return false;
	}

	return true;
}

TEST(TokenizerTest, TokenizesProperly)
{
	std::string input = 
R"(exit 0;
a = 2 + 5;
b = -1 * (4 / 2);)";

	Literal zero(0);
	Literal one(1);
	Literal two(2);
	Literal four(4);
	Literal five(5);

	Variable a(HeType::I32, std::string_view("a"));
	Variable b(HeType::I32, std::string_view("b"));

	std::vector<Token> expected = {
		// first row
		{ TokenType::EXIT, {0, 0} },
		{ TokenType::LITERAL, {0, 5}, &zero },
		{ TokenType::SEMICOLON, {0, 6} },

		// second row
		{ TokenType::VARIABLE, {1, 0}, &a },
		{ TokenType::EQUALS, {1, 2} },
		{ TokenType::LITERAL, {1, 4}, &two },
		{ TokenType::PLUS, {1, 6} },
		{ TokenType::LITERAL, {1, 8}, &five },
		{ TokenType::SEMICOLON, {1, 9} },

		// third row
		{ TokenType::VARIABLE, {2, 0}, &b },
		{ TokenType::EQUALS, {2, 2} },
		{ TokenType::MINUS, {2, 4} },
		{ TokenType::LITERAL, {2, 5}, &one },
		{ TokenType::ASTERISK, {2, 7} },
		{ TokenType::OPEN_PAREN, {2, 9} },
		{ TokenType::LITERAL, {2, 10}, &four },
		{ TokenType::FORWARD_SLASH, {2, 12} },
		{ TokenType::LITERAL, {2, 14}, &two },
		{ TokenType::CLOSE_PAREN, {2, 15} },
		{ TokenType::SEMICOLON, {2, 16} },
	};

	std::vector<Token> tokens = Tokenizer::tokenize(input);
	EXPECT_EQ(Tokenizer::semicolonCount(), 3);
	ASSERT_THAT(tokens, TokenVectorMatches(expected));
}