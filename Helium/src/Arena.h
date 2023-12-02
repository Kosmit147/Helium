// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <vector>

#include "common.h"

template <typename ArenaType>
class ArenaIterator
{
public:
	using ValueType = typename ArenaType::ValueType;

	inline ArenaIterator(const ArenaType& arena, usize index) : _arena(arena), _index(index)
	{
		if (_index >= _arena.objectCount())
			_ptr = nullptr;
		else
			_ptr = _arena.addrAt(_index);
	}

	inline ArenaIterator& operator++()
	{
		_index++;

		if (_index >= _arena.objectCount())
			_ptr = nullptr;
		else
			_ptr = _arena.addrAt(_index);

		return *this;
	}

	inline ArenaIterator& operator++(int)
	{
		ArenaIterator& iterator = *this;
		++(*this);
		return iterator;
	}

	inline bool operator==(const ArenaIterator& other) const { return _ptr == other._ptr; }
	inline bool operator!=(const ArenaIterator& other) const { return !(*this == other); }

	inline ValueType& operator*() const { return *_ptr; }

private:
	const ArenaType& _arena;
	ValueType* _ptr;
	usize _index;
};

// --- IMPORTANT! ---
// The arena must guarantee that a pointer to an object within the arena
// is valid as long as the arena exists and dealloc() wasn't explicitly called
template <typename T>
class Arena
{
public:
	using ValueType = T;
	using Iterator = ArenaIterator<Arena<T>>;

	explicit inline Arena(usize allocStrat = 100) : _allocStrat(allocStrat)
	{
		allocBlocks(1);
		T* blockAddr = addrAt(0);
		_front = blockAddr;
		_back = blockAddr;
	}

	inline ~Arena()
	{
		dealloc();
	}

	inline void allocBlocks(usize count)
	{
		for (it i = 0; i < count; i++)
		{
			T* const block = (T*)malloc(sizeof(T) * _allocStrat);
#ifdef _DEBUG
			std::cout << "New block(" << sizeof(T) * _allocStrat << " bytes) at " << block << '.' << std::endl;
#endif
			_blocks.push_back(block);
			_capacityLeft += _allocStrat;
		}
	}

	inline void dealloc()
	{
		for (it i = 0; i < _objectCount; i++)
			at(i).~T();

		for (T* block : _blocks)
		{
#ifdef _DEBUG
			std::cout << "Freed block at " << block << '.' << std::endl;
#endif
			free(block);
		}

		_objectCount = 0;
		_capacityLeft = 0;
		_front = nullptr;
		_back = nullptr;
		_blocks.clear();
	}

	inline T& pushBack(const T& elem)
	{
		if (_capacityLeft == 0)
			allocBlocks(1);

		T* addr = addrAt(_objectCount);
		new(addr) T(elem);
		_capacityLeft--;
		_objectCount++;
		_back = addr;
		return *addr;
	}

	inline T& pushBack(T&& elem)
	{
		if (_capacityLeft == 0)
			allocBlocks(1);

		T* addr = addrAt(_objectCount);
		new(addr) T(std::move(elem));
		_capacityLeft--;
		_objectCount++;
		_back = addr;
		return *addr;
	}

	template<typename... Args>
	inline T& emplaceBack(Args&&... args)
	{
		if (_capacityLeft == 0)
			allocBlocks(1);

		T* addr = addrAt(_objectCount);
		new(addr) T(std::forward<Args>(args)...);
		_capacityLeft--;
		_objectCount++;
		_back = addr;
		return *addr;
	}

	inline T& at(usize index) const
	{
		usize blockIndex = index / _allocStrat;
		T* block = _blocks[blockIndex];
		return block[index - blockIndex * _allocStrat];
	}

	inline T* addrAt(usize index) const
	{
		usize blockIndex = index / _allocStrat;
		T* block = _blocks[blockIndex];
		return block + (index - blockIndex * _allocStrat);
	}

	inline T& operator[](usize index) const { return at(index); }

	inline Iterator begin() const { return Iterator(*this, 0); }
	inline Iterator end() const { return Iterator(*this, _objectCount); }

	inline T& front() const { return *_front; }
	inline T& back() const { return *_back; }
	inline usize objectCount() const { return _objectCount; }
	inline usize capacityLeft() const { return _capacityLeft; }

private:
	// number of elements of type T allocated per block
	const usize _allocStrat;
	usize _objectCount = 0;
	usize _capacityLeft = 0;
	T* _front;
	T* _back;

	// pointers to blocks of data that store n = _allocStrat elements of type T
	std::vector<T*> _blocks;

public:
	Arena() = delete;
	Arena(const Arena& other) = delete;
	Arena(Arena&& other) = delete;
};