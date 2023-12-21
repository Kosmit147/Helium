// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "global.h"
#include "Tokenizer.h"
#include "Compiler.h"
#include "file.h"
#include "log.h"
#include "Parser.h"

int main(int argc, char** argv)
{
	global::args = parseArgs(argc, argv);

	std::stringstream heCode = readFile(global::args.inputFile);
	std::vector<Token> tokens = Tokenizer::tokenize(heCode.view());
	std::vector<Statement> statements = Parser::parseTokens(tokens);

#ifdef _DEBUG
	printTokens(tokens);
	std::cout << std::endl;
	printCode(heCode.view());
	std::cout << std::endl;
	printStatements(statements);
#endif

	// Compiler::compileIntoFile(global::args.outputFile);
}