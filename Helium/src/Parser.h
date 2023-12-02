// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>
#include <optional>

#include "GrammarTree.h"
#include "parsing.h"

class Parser
{
public:
	using TokenType = Token::TokenType;

	[[nodiscard]] static std::vector<Statement> parseTokens(const std::vector<Token>& tokens);
	[[nodiscard]] static Statement parseStatement(TokenView view);
	[[nodiscard]] static std::optional<Statement> matchBranch(GrammarTree::Branch branch, 
		TokenView view);
	[[nodiscard]] static bool traverseNodes(Statement& stmt, const GrammarTree::Node* node, 
		TokenView view);
	[[nodiscard]] static std::optional<ExprData> parseExpr(TokenView view);

private:
	static const GrammarTree _grammarTree;
};