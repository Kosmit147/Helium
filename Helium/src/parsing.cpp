#include "parsing.h"

using StatementType = Statement::Type;

#ifdef _DEBUG

const std::unordered_map<StatementType, std::string> Statement::statementNameMap = {
	{ StatementType::EMPTY,			"empty" },
	{ StatementType::EXIT,			"exit" },
	{ StatementType::ASSIGN,		"assign" },
};

#endif