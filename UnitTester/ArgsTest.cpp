#include "pch.h"

#include "Helium/src/Args.h"

using testing::AllOf;
using testing::ExplainMatchResult;
using testing::Field;

MATCHER_P(MatchesArgs, args, "Args struct does not match expected args struct")
{
	return ExplainMatchResult(AllOf(
		Field("inputFile", &Args::inputFile, args.inputFile),
		Field("outputFile", &Args::outputFile, args.outputFile),
		Field("printHelp", &Args::printHelp, args.printHelp)
	), arg, result_listener);
}

template<usize size>
static inline std::array<char*, size> argvFromStrArr(std::array<std::string, size>& args)
{
	std::array<char*, size> argv{};

	for (it i = 0; i < args.size(); i++)
		argv[i] = args[i].data();

	return argv;
}

TEST(ArgsTest, ParsesArgs)
{
	std::array<std::string, 6> args1Data = {
		"helium",
		"-i",
		"hello_world.he",
		"-h",
		"-o",
		"example.exe",
	};

	std::array<char*, args1Data.size()> args1 = argvFromStrArr(args1Data);

	Args expected = {
		.inputFile = "hello_world.he",
		.outputFile = "example.exe",
		.printHelp = true,
	};

	Args parsed = parseArgs((int)args1.size(), args1.data());
	ASSERT_THAT(parsed, MatchesArgs(expected));

	std::array<std::string, 5> args2Data = {
		"helium",
		"-i",
		"example.he",
		"-o",
		"example.exe",
	};

	std::array<char*, args2Data.size()> args2 = argvFromStrArr(args2Data);

	expected = {
		.inputFile = "example.he",
		.outputFile = "example.exe",
		.printHelp = false,
	};

	parsed = parseArgs((int)args2.size(), args2.data());
	ASSERT_THAT(parsed, MatchesArgs(expected));

	std::array<std::string, 7> args3Data = {
		"helium",
		"-o",
		"example",
		"-h",
		"-h",
		"-i",
		"example.he",
	};

	std::array<char*, args3Data.size()> args3 = argvFromStrArr(args3Data);

	expected = {
		.inputFile = "example.he",
		.outputFile = "example.exe",
		.printHelp = true,
	};

	parsed = parseArgs((int)args3.size(), args3.data());
	ASSERT_THAT(parsed, MatchesArgs(expected));
}

TEST(ArgsTest, ValidatesArgs)
{
	Args to_validate = {
		.inputFile = "example.he",
		.outputFile = "example",
		.printHelp = true,
	};

	Args expected = {
		.inputFile = "example.he",
		.outputFile = "example.exe",
		.printHelp = true,
	};

	validateArgs(to_validate);
	ASSERT_THAT(to_validate, MatchesArgs(expected));

	to_validate = {
		.inputFile = "example.he",
		.outputFile = "example.bin.docx",
		.printHelp = true,
	};

	expected = {
		.inputFile = "example.he",
		.outputFile = "example.bin.docx.exe",
		.printHelp = true,
	};

	validateArgs(to_validate);
	ASSERT_THAT(to_validate, MatchesArgs(expected));
}