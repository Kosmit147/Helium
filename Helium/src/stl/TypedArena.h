// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include "pch.h"

template <typename ArenaSpecializationType>
class TypedArenaIterator
{
public:
	using ValueType = typename ArenaSpecializationType::ValueType;

	inline TypedArenaIterator(const ArenaSpecializationType& arena, usize index) 
		: _arena(arena), _index(index)
	{
		if (_index >= _arena.objectCount())
			_ptr = nullptr;
		else
			_ptr = _arena.addrAt(_index);
	}

	inline TypedArenaIterator& operator++()
	{
		_index++;

		if (_index >= _arena.objectCount())
			_ptr = nullptr;
		else
			_ptr = _arena.addrAt(_index);

		return *this;
	}

	inline TypedArenaIterator& operator++(int)
	{
		TypedArenaIterator& iterator = *this;
		++(*this);
		return iterator;
	}

	inline bool operator==(const TypedArenaIterator& other) const { return _ptr == other._ptr; }
	inline bool operator!=(const TypedArenaIterator& other) const { return !(*this == other); }

	inline ValueType& operator*() const { return *_ptr; }

private:
	const ArenaSpecializationType& _arena;
	ValueType* _ptr;
	usize _index;
};

// --- IMPORTANT! ---
// TypedArena must guarantee that a pointer to an object within the arena
// is valid as long as the arena exists and dealloc() wasn't explicitly called
template <typename T>
class TypedArena
{
public:
	using ValueType = T;
	using Iterator = TypedArenaIterator<TypedArena<T>>;

	explicit inline TypedArena(usize allocStrat = 100) : _allocStrat(allocStrat)
	{
		allocBlocks(1);
		T* blockAddr = addrAt(0);
		_front = blockAddr;
		_back = blockAddr;
	}

	inline ~TypedArena()
	{
		dealloc();
	}

	inline void allocBlocks(usize count)
	{
		for (it i = 0; i < count; i++)
		{
			T* const block = (T*)malloc(sizeof(T) * _allocStrat);
			_blocks.push_back(block);
			_capacityLeft += _allocStrat;
		}
	}

	inline void dealloc()
	{
		for (it i = 0; i < _objectCount; i++)
			at(i).~T();

		for (T* block : _blocks)
			free(block);

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
	TypedArena() = delete;
	TypedArena(const TypedArena& other) = delete;
	TypedArena(TypedArena&& other) = delete;
};