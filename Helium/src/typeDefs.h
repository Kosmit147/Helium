#pragma once

#include <cstdint>
#include <cstddef>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

static_assert(sizeof(float) == 4, "sizeof(float) should be 4.");
static_assert(sizeof(double) == 8, "sizeof(double) should be 8.");
using f32 = float;
using f64 = double;

using usize = std::size_t;
using it = usize;
