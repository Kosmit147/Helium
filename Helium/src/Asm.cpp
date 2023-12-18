// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"

#include "Asm.h"

SegmentsData Asm::_segmentsData;
std::stringstream& Asm::_bss = Asm::_segmentsData.bss;
std::stringstream& Asm::_data = Asm::_segmentsData.data;
std::stringstream& Asm::_text = Asm::_segmentsData.text;

Asm::Symbol::Symbol(std::string&& name) : name(std::move(name)) {}