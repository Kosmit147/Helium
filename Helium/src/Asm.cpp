#include "Asm.h"

SegmentsData Asm::segmentsData{};
std::stringstream& Asm::bss = Asm::segmentsData.bss;
std::stringstream& Asm::data = Asm::segmentsData.data;
std::stringstream& Asm::text = Asm::segmentsData.text;

Asm::Symbol::Symbol(std::string&& name) : name(std::move(name)) {}