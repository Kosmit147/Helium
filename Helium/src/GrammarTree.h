// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <array>
#include <vector>

#include "parsing.h"

struct GrammarTree
{
public:
	using TokenType = Token::TokenType;
	using StatementType = Statement::Type;

	struct Node
	{
		enum class Type 
		{
			TOKEN,
			EXPRA,
			EXPRB,
		};

		Type type;
		TokenType tokenType;
		std::vector<Node*> branchingNodes;

		constexpr inline Node(Type type, TokenType tokenType)
			: type(type), tokenType(tokenType), branchingNodes({}) {}
	};

	struct Branch
	{
		StatementType statementType;
		Node* startingNode;
	};

	const std::vector<Branch>& branches() const { return _branches; }

	GrammarTree();

private:
	std::vector<Branch> _branches;
	std::vector<Node> _nodes;
};