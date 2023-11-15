// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Asm.h"

SegmentsData Asm::segmentsData{};
std::stringstream& Asm::bss = Asm::segmentsData.bss;
std::stringstream& Asm::data = Asm::segmentsData.data;
std::stringstream& Asm::text = Asm::segmentsData.text;

Asm::Symbol::Symbol(std::string&& name) : name(std::move(name)) {}