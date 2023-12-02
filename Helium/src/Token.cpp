// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Token.h"

using TokenType = Token::TokenType;

const Token Token::errorToken = { TokenType::ERR, Token::FilePosition{ .row = 0, .col = 0 } };

const std::unordered_map<std::string, TokenType> Token::tokenTypeMap =
{
	{ "exit", TokenType::EXIT },
	{ ";", TokenType::SEMICOLON },
	{ "(", TokenType::OPEN_PAREN },
	{ ")", TokenType::CLOSE_PAREN },
	{ "=", TokenType::EQUALS },
	{ "+", TokenType::PLUS },
	{ "-", TokenType::MINUS },
	{ "*", TokenType::ASTERISK },
	{ "/", TokenType::FORWARD_SLASH },
};

Variable::Variable(HeType type, std::string_view name)
	: type(type), name(name) {}

Variable::Variable(HeType type, std::string&& name)
	: type(type), name(std::move(name)) {}

Token::Token(TokenType tokenType, FilePosition filePos)
	: tokenType(tokenType), filePos(filePos), literal(nullptr) {}

#ifdef _DEBUG

const std::unordered_map<TokenType, std::string> Token::tokenNameMap = {
	{ TokenType::ERR,			ERR_STR },
	{ TokenType::EXIT,			"exit" },
	{ TokenType::LITERAL,		"literal" },
	{ TokenType::VARIABLE,		"variable" },
	{ TokenType::SEMICOLON,		"semicolon" },
	{ TokenType::OPEN_PAREN,	"openingParenthesis" },
	{ TokenType::CLOSE_PAREN,	"closingParenthesis" },
	{ TokenType::EQUALS,		"assignment" },
	{ TokenType::PLUS,			"plus" },
	{ TokenType::MINUS,			"minus" },
	{ TokenType::ASTERISK,		"asterisk" },
	{ TokenType::FORWARD_SLASH, "forwardSlash" },
};

#endif