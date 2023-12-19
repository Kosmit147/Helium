#include "pch.h"

#include "Helium/src/Args.h"

TEST(TestCaseName, TestName) 
{
	char* argdd = (char*)"xdd";
	char** argvv = &argdd;
	Args args = parseArgs(1, argvv);

	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}