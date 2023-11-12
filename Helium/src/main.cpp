#include <iostream>

#include "common.h"
#include "args.h"
#include "Token.h"
#include "Compiler.h"
#include "file.h"
#include "log.h"

int main(int argc, char* argv[])
{
	Args args = parseArgs(argc, argv);

	std::stringstream heCode = readFile(args.inputFile);
	std::vector<Token> tokens = Tokenizer::tokenize(heCode.view());

#ifdef _DEBUG
	printTokens(tokens);
	std::cout << std::endl;
	printCode(heCode.view());
#endif
}