// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "parsing.h"

using StatementType = Statement::Type;

#ifdef _DEBUG

const std::unordered_map<StatementType, std::string> Statement::statementNameMap = {
	{ StatementType::EMPTY,			"empty" },
	{ StatementType::EXIT,			"exit" },
	{ StatementType::ASSIGN,		"assign" },
};

#endif