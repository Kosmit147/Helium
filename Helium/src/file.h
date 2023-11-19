// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <sstream>

#include "SegmentsData.h"

[[nodiscard]] std::stringstream readFile(std::string_view fileName);
void writeToFile(std::string_view fileName, const std::stringstream& stream);

// for writing contents of bss, data and text assembly sections into a file
void writeToFile(std::string_view fileName, const SegmentsData& segments);