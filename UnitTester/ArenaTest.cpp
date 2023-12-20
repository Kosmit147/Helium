#include "pch.h"

#include "Helium/src/stl/TypedArena.h"

TEST(TypedArenaTest, CanCreateArena)
{
	TypedArena<int> arena(100);
}

TEST(TypedArenaTest, CanPushBackNewObjects)
{
	TypedArena<int> intArena(5);

	ASSERT_EQ(intArena.objectCount(), 0);
	ASSERT_EQ(intArena.capacityLeft(), 5);

	intArena.pushBack(5);
	intArena.pushBack(2);
	intArena.emplaceBack(3);
	intArena.emplaceBack(4);

	ASSERT_EQ(intArena.objectCount(), 4);
	ASSERT_EQ(intArena.capacityLeft(), 1);

	TypedArena<std::string> strArena(10);

	ASSERT_EQ(strArena.objectCount(), 0);
	ASSERT_EQ(strArena.capacityLeft(), 10);

	strArena.pushBack("example1");
	strArena.pushBack("example2");
	strArena.emplaceBack("example3");
	strArena.emplaceBack("example4");

	EXPECT_EQ(strArena.objectCount(), 4);
	EXPECT_EQ(strArena.capacityLeft(), 6);
}

TEST(TypedArenaTest, AllocatesNewBlockWhenOverCapacity) 
{
	TypedArena<std::string> arena(2);

	ASSERT_EQ(arena.objectCount(), 0);
	ASSERT_EQ(arena.capacityLeft(), 2);

	arena.emplaceBack("one");

	ASSERT_EQ(arena.objectCount(), 1);
	ASSERT_EQ(arena.capacityLeft(), 1);

	arena.emplaceBack("two");

	ASSERT_EQ(arena.objectCount(), 2);
	ASSERT_EQ(arena.capacityLeft(), 0);

	arena.emplaceBack("three");

	ASSERT_EQ(arena.objectCount(), 3);
	ASSERT_EQ(arena.capacityLeft(), 1);

	arena.emplaceBack("four");

	ASSERT_EQ(arena.objectCount(), 4);
	ASSERT_EQ(arena.capacityLeft(), 0);

	arena.emplaceBack("five");
	arena.emplaceBack("six");
	arena.emplaceBack("seven");
	arena.emplaceBack("eight");
	arena.emplaceBack("nine");
	arena.emplaceBack("ten");

	EXPECT_EQ(arena.objectCount(), 10);
	EXPECT_EQ(arena.capacityLeft(), 0);
}

TEST(TypedArenaTest, CanAllocBlocksManually)
{
	TypedArena<bool> arena(100);

	ASSERT_EQ(arena.objectCount(), 0);
	ASSERT_EQ(arena.capacityLeft(), 100);

	arena.allocBlocks(9);

	ASSERT_EQ(arena.objectCount(), 0);
	ASSERT_EQ(arena.capacityLeft(), 1000);

	arena.pushBack(true);

	EXPECT_EQ(arena.objectCount(), 1);
	EXPECT_EQ(arena.capacityLeft(), 999);
}

TEST(TypedArenaTest, CanIterateOverObjects)
{
	constexpr static std::array<int, 10> input = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	};

	constexpr static std::array<int, 10> expected = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	};

	TypedArena<int> arena(5);

	for (int num : input)
		arena.pushBack(num);

	std::vector<int> results;
	results.reserve(expected.size());

	for (int num : arena)
		results.push_back(num);

	ASSERT_THAT(results, ::testing::ElementsAreArray(expected)) 
		<< "Elements resulting from iterating over TypedArena differ from expected.";
}

TEST(TypedArenaTest, CanGetObjectAtIndex)
{
	TypedArena<int> intArena(2);
	TypedArena<std::string> strArena(2);

	intArena.pushBack(1);
	intArena.pushBack(2);
	intArena.pushBack(3);

	EXPECT_EQ(intArena.at(0), 1);
	EXPECT_EQ(intArena.at(1), 2);
	EXPECT_EQ(intArena.at(2), 3);

	EXPECT_EQ(intArena[0], 1);
	EXPECT_EQ(intArena[1], 2);
	EXPECT_EQ(intArena[2], 3);

	strArena.emplaceBack("One");
	strArena.emplaceBack("Two");
	strArena.emplaceBack("Three");

	EXPECT_EQ(strArena.at(0), "One");
	EXPECT_EQ(strArena.at(1), "Two");
	EXPECT_EQ(strArena.at(2), "Three");

	EXPECT_EQ(strArena[0], "One");
	EXPECT_EQ(strArena[1], "Two");
	EXPECT_EQ(strArena[2], "Three");
}

TEST(TypedArenaTest, BeginAndEndWork)
{
	TypedArena<int> arena(1);

	ASSERT_EQ(arena.objectCount(), 0);
	ASSERT_EQ(arena.capacityLeft(), 1);

	TypedArenaIterator<TypedArena<int>> invalidIter(arena, 1000000);

	EXPECT_TRUE(arena.begin() == invalidIter);
	EXPECT_TRUE(arena.end() == invalidIter);

	arena.pushBack(1);
	arena.pushBack(10);
	arena.pushBack(100);

	EXPECT_EQ(*arena.begin(), 1);
	EXPECT_EQ(arena.front(), 1);
	EXPECT_TRUE(arena.end() == invalidIter);
	EXPECT_EQ(arena.back(), 100);
}