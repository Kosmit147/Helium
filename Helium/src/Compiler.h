#pragma once

#include <sstream>

#include "args.h"
#include "Token.h"

#define TAB '\t'
#define NL '\n'

struct SegmentsData
{
	std::stringstream bss, data, text;
};

class Compiler
{
public:
	static void compileIntoFile(const Args& args, const std::vector<Token>& tokens, 
		std::string_view outputFileName);
	static void compileStatement(usize startTokenIndex, usize endTokenIndex);

	inline static void setUpShadowZone();

	inline static void setArgInt(usize pos, u64 val);
	inline static void setReturnInt(u64 val);

	class Asm
	{
	public:
		enum class Register
		{
			RAX,
			RBX,
			RCX,
			RDX,
			RSI,
			RDI,
			RBP,
			RSP,
			R8,
			R9,
			R10,
			R11,
			R12,
			R13,
			R14,
			R15,

			XMM0,
			XMM1,
			XMM2,
			XMM3,
			XMM4,
			XMM5,
			XMM6,
			XMM7,
			XMM8,
			XMM9,
			XMM10,
			XMM11,
			XMM12,
			XMM13,
			XMM14,
			XMM15,
		};

		enum class Segment
		{
			BSS,
			DATA,
			TEXT
		};

		enum class AddressingMode
		{
			REL,
			ABS,
			BND,
			NOBND
		};

		struct Symbol
		{
			std::string name;

			Symbol(std::string&& name);
		};

		inline static void newLine();
		inline static void comment(std::string_view comment);
		inline static void insComment(std::string_view comment);

		inline static void setBits(usize bits);
		inline static void setAddressingMode(AddressingMode mode);
		inline static void setSegment(Segment segment);

		inline static void exportSymbol(const Symbol& symbol);
		inline static void exportSymbol(std::string_view name);
		inline static void importSymbol(const Symbol& symbol);
		inline static void importSymbol(std::string_view name);

		inline static void call(const Symbol& symbol);
		inline static void call(std::string_view name);
		inline static void setLabel(const Symbol& symbol);
		inline static void setLabel(std::string_view name);

		inline static void i_push(Register r);
		inline static void i_mov(Register dst, Register src);
		inline static void i_mov(Register r, u64 val);
		inline static void i_sub(Register r, u64 val);
		inline static void i_xor(Register a, Register b);

		[[nodiscard]] constexpr static std::string registerToString(Register r);
		[[nodiscard]] constexpr static std::string segmentToString(Segment segment);
		[[nodiscard]] constexpr static std::string addressingModeToString(AddressingMode mode);
	};

private:
	static SegmentsData segmentsData;
	static std::stringstream& bss;
	static std::stringstream& data;
	static std::stringstream& text;
	static const Args* args;
	static const std::vector<Token>* tokens;

public:
	Compiler() = delete;
};

inline std::ostream& operator<<(std::ostream& stream, Compiler::Asm::AddressingMode mode);
inline std::ostream& operator<<(std::ostream& stream, Compiler::Asm::Segment segment);
inline std::ostream& operator<<(std::ostream& stream, Compiler::Asm::Register r);