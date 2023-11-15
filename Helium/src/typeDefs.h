// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

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

template<typename T>
using Ptr = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Ptr<T> createPtr(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> createRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
