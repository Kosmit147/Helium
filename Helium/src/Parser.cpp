// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Parser.h"

#include <stack>

#include "global.h"
#include "Tokenizer.h"
#include "error.h"

using TokenType = Token::TokenType;
using StatementType = Statement::Type;

const GrammarTree Parser::_grammarTree;

std::vector<Statement> Parser::parseTokens(const std::vector<Token>& tokens)
{
	std::vector<Statement> statements;
	statements.reserve(Tokenizer::semicolonCount());

	const Token* stmtStartPtr = &tokens[0];

	for (const Token& token : tokens)
	{
		if (token.type == TokenType::SEMICOLON /* TODO: or closing brace */)
		{
			const Token* stmtEndPtr = &token;
			statements.emplace_back(parseStatement({ stmtStartPtr, stmtEndPtr }));
			stmtStartPtr = stmtEndPtr + 1;
		}
	}

	const Token& lastToken = tokens.back();

	if (lastToken.type != TokenType::SEMICOLON /* TODO: or closing brace */)
		exitWithError(ErrorCode::EXPECTED_A_SEMICOLON, lastToken.filePos);

	return statements;
}

Statement Parser::parseStatement(TokenView view)
{
	for (const auto& grammarBranch : _grammarTree.branches)
	{
		auto stmt = matchBranch(grammarBranch, { view.first, view.last });

		if (stmt.has_value())
			return *stmt;
	}

	const Token& stmtStart = *view.first;

	exitWithError(ErrorCode::FAILED_TO_PARSE_STATEMENT, stmtStart.filePos);
	return { StatementType::EMPTY };
}

std::optional<Statement> Parser::matchBranch(GrammarTree::Branch branch, 
	TokenView view)
{
	Statement stmt;
	
	if (traverseNodes(stmt, branch.startingNode, view))
	{
		stmt.type = branch.statementType;
		stmt.row = view.first->filePos.row;
		return stmt;
	}

	return {};
}

// returns true if managed to successfully traverse through a branch of the grammar tree
bool Parser::traverseNodes(Statement& stmt, const GrammarTree::Node* node, TokenView view)
{
	using Node = GrammarTree::Node;

	TokenView remainingTokensView = view;

	Node::Type nodeType = node->type;

	if (nodeType == Node::Type::EXPRA || nodeType == Node::Type::EXPRB)
	{
		auto exprData = parseExpr(view);

		if (exprData)
		{
			Expression& expr = exprData->expr;
			const Token* exprEnd = exprData->exprSpan.last;

			if (nodeType == Node::Type::EXPRA)
				stmt.a = &global::expressions.pushBack(std::move(expr));
			else // if (nodeType == Node::Type::EXPRB)
				stmt.b = &global::expressions.pushBack(std::move(expr));
			
			remainingTokensView.first = exprEnd;
		}
		else
		{
			return false;
		}
	}
	else if (nodeType == Node::Type::TOKEN)
	{
		if (node->tokenType != remainingTokensView.first->type)
			return false;
	}

	if (!node->branchingNodes.empty())
	{
		for (const Node* branchingNode : node->branchingNodes)
		{
			if (traverseNodes(stmt, branchingNode, { remainingTokensView.first + 1, remainingTokensView.last }))
				return true;
		}
	}
	
	if (remainingTokensView.first == remainingTokensView.last)
	{
		// we've reached the last token and matched every one so far, job done
		return true;
	}

	return false;
}

constexpr inline static usize getOperatorPrecedence(TokenType tokenType)
{
	switch (tokenType)
	{
	case TokenType::ASTERISK:
	case TokenType::FORWARD_SLASH:
		return 2;
		break;
	case TokenType::PLUS:
	case TokenType::MINUS:
		return 1;
		break;
	}

	HE_DEBUG_BREAK;
	return 0;
}

enum class TokenRpnType
{
	OPERAND,
	OPERATOR,
	OPEN_PAREN,
	CLOSE_PAREN,
	TERMINATOR,
	INVALID,
};

constexpr inline static TokenRpnType getTokenRpnType(TokenType tokenType)
{
	switch (tokenType)
	{
	case TokenType::LITERAL:
	case TokenType::VARIABLE:
		return TokenRpnType::OPERAND;
		break;
	case TokenType::PLUS:
	case TokenType::MINUS:
	case TokenType::ASTERISK:
	case TokenType::FORWARD_SLASH:
		return TokenRpnType::OPERATOR;
		break;
	case TokenType::OPEN_PAREN:
		return TokenRpnType::OPEN_PAREN;
		break;
	case TokenType::CLOSE_PAREN:
		return TokenRpnType::CLOSE_PAREN;
		break;
	// TODO: case TokenType::CLOSE_BRACE:
	case TokenType::SEMICOLON:
		return TokenRpnType::TERMINATOR;
		break;
	case TokenType::NONE:
	case TokenType::ERR:
	case TokenType::EXIT:
	case TokenType::EQUALS:
		return TokenRpnType::INVALID;
		break;
	}

	HE_DEBUG_BREAK;
	return TokenRpnType::INVALID;
}

std::optional<ExprData> Parser::parseExpr(TokenView view)
{
	using TokenStack = std::stack<const Token*, std::vector<const Token*>>;

	Expression expr;
	TokenStack tokenStack;
	std::vector<Token>& rpnVec = expr.rpn;

	auto popIntoRpnAfterClosingParen = [&](const Token& closingParen) {
		while (tokenStack.top()->type != TokenType::OPEN_PAREN)
		{
			if (tokenStack.empty())
				exitWithError(ErrorCode::UNEXPECTED_CHARACTER, closingParen.filePos);

			rpnVec.push_back(*tokenStack.top());
			tokenStack.pop();
		}
	};

	auto popOperatorsOfHigherPrecedenceIntoRpn = [&](const Token& rpnOperator) {
		usize opPrecedence = getOperatorPrecedence(rpnOperator.type);
		while (!tokenStack.empty() && getOperatorPrecedence(tokenStack.top()->type) >= opPrecedence)
		{
			rpnVec.push_back(*tokenStack.top());
			tokenStack.pop();
		}
		tokenStack.push(&rpnOperator);
	};

	auto emptyStackIntoRpn = [&]() {
		while (!tokenStack.empty())
		{
			rpnVec.push_back(*tokenStack.top());
			tokenStack.pop();
		}
	};

	const Token* exprEnd = view.last;

	for (const Token& token : view)
	{
		switch (getTokenRpnType(token.type))
		{
		case TokenRpnType::OPERAND:
			rpnVec.push_back(token);
			break;
		case TokenRpnType::OPEN_PAREN:
			tokenStack.push(&token);
			break;
		case TokenRpnType::CLOSE_PAREN:
			popIntoRpnAfterClosingParen(token);
			break;
		case TokenRpnType::OPERATOR:
			popOperatorsOfHigherPrecedenceIntoRpn(token);
			break;
		// TODO: case TokenType::CLOSE_BRACE:
		case TokenRpnType::TERMINATOR:
			emptyStackIntoRpn();
			exprEnd = &token - 1;
			return std::optional<ExprData>({ expr, { view.first, exprEnd } });
			break;
		case TokenRpnType::INVALID:
			return {};
			break;
		}
	}

	return std::optional<ExprData>({ expr, { view.first, exprEnd } });
}