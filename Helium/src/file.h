#pragma once

#include <fstream>
#include <sstream>

#include "Compiler.h"

std::stringstream readFile(std::string_view fileName);
void writeToFile(std::string_view fileName, const std::stringstream& stream);

// for writing contents of bss, data and text assembly sections into a file
void writeToFile(std::string_view fileName, const SegmentsData& segments);