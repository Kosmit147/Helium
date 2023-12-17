// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "GrammarTree.h"

#include "common.h"

using Node = GrammarTree::Node;
using NodeType = Node::Type;
using TokenType = Token::TokenType;
using StatementType = Statement::Type;

struct NodeInitializer
{
	NodeType nodeType;
	TokenType tokenType;
	std::initializer_list<usize> branchingNodesIndexes;
};

struct BranchInitializer
{
	StatementType stmtType;
	std::initializer_list<NodeInitializer> nodesInitList;
};

GrammarTree::GrammarTree()
{
	static std::initializer_list<BranchInitializer> initList = {
		{ StatementType::EMPTY, {
			{ NodeType::TOKEN, TokenType::SEMICOLON, {} },
		}},
		{ StatementType::EXIT, {
			{ NodeType::TOKEN, TokenType::EXIT, { 1 } },
			{ NodeType::EXPRA, TokenType::NONE, { 2 } },
			{ NodeType::TOKEN, TokenType::SEMICOLON, {} },
		}},
		{ StatementType::ASSIGN, {
			{ NodeType::TOKEN, TokenType::VARIABLE, { 1 } },
			{ NodeType::TOKEN, TokenType::EQUALS, { 2 } },
			{ NodeType::EXPRA, TokenType::NONE,	 { 3 } },
			{ NodeType::TOKEN, TokenType::SEMICOLON, {} },
		}},
	};

	auto countNodes = [&]() {
		usize count = 0;
		for (const auto& branchInitList : initList)
			for (const auto& nodeInitList : branchInitList.nodesInitList)
				count++;
		return count;
	};

	static usize branchCount = initList.size();
	static usize nodeCount = countNodes();

	branches.reserve(initList.size());
	_nodes.reserve(nodeCount);

	// initialize nodes vector
	for (const auto& branchInitList : initList)
	{
		for (const auto& nodeInitList : branchInitList.nodesInitList)
		{
			_nodes.emplace_back(nodeInitList.nodeType, nodeInitList.tokenType);
		}
	}

	usize offset = 0;

	// initialize all the pointers in the graph
	for (const auto& branchInitList : initList)
	{
		branches.emplace_back(branchInitList.stmtType, &_nodes[offset]);

		usize currNodeIndex = 0;

		for (const auto& nodeInitList : branchInitList.nodesInitList)
		{
			usize branchingNodesCount = nodeInitList.branchingNodesIndexes.size();
			_nodes[offset + currNodeIndex].branchingNodes.reserve(branchingNodesCount);

			for (usize branchingNodeIndex : nodeInitList.branchingNodesIndexes)
			{
				Node* ptr = &_nodes[offset + branchingNodeIndex];
				_nodes[offset + currNodeIndex].branchingNodes.push_back(ptr);
			}

			currNodeIndex++;
		}

		offset += currNodeIndex;
	}
}