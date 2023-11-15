// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Compiler.h"

#include "Asm.h"

const Args* Compiler::args;

void Compiler::compileIntoFile(const Args& args, std::string_view fileName)
{
	Compiler::args = &args;

	Asm::setBits(64);
	Asm::setAddressingMode(Asm::AddressingMode::REL);
	Asm::newLine();

	Asm::setSegment(Asm::Segment::TEXT);
	Asm::exportSymbol("main");
	Asm::importSymbol("_CRT_INIT");
	Asm::importSymbol("ExitProcess");
	Asm::newLine();

	Asm::setLabel("main");

	Asm::setUpShadowZone();

	Asm::call("_CRT_INIT");
	Asm::newLine();

	Asm::setReturnInt(0);

	Asm::call("ExitProcess", 1);

	Asm::assembleIntoFile(fileName);
}
