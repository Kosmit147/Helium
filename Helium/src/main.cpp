// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include "common.h"
#include "Args.h"
#include "Tokenizer.h"
#include "Compiler.h"
#include "file.h"
#include "log.h"

int main(int argc, char* argv[])
{
	Args args = parseArgs(argc, argv);

	std::stringstream heCode = readFile(args.inputFile);
	std::vector<Token> tokens = Tokenizer::tokenize(args, heCode.view());

#ifdef _DEBUG
	printTokens(tokens);
	std::cout << std::endl;
	printCode(heCode.view());
#endif

	Compiler::compileIntoFile(args, args.outputFile);
}