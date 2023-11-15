// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <sstream>

#include "common.h"
#include "file.h"

#define TAB '\t'
#define NL '\n'

class Asm
{
public:
	inline static void assembleIntoFile(std::string_view outputFileName);

	inline static void setUpShadowZone();

	inline static void setArgInt(usize pos, u64 val);
	inline static void setReturnInt(u64 val);

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
	inline static void call(const Symbol& symbol, u64 val);
	inline static void call(std::string_view name, u64 val);

	inline static void setLabel(const Symbol& symbol);
	inline static void setLabel(std::string_view name);

	inline static void i_push(Register r);
	inline static void i_mov(Register dst, Register src);
	inline static void i_mov(Register r, u64 val);
	inline static void i_sub(Register r, u64 val);
	inline static void i_xor(Register a, Register b);

	[[nodiscard]] constexpr static const char* registerToString(Register r);
	[[nodiscard]] constexpr static const char* segmentToString(Segment segment);
	[[nodiscard]] constexpr static const char* addressingModeToString(AddressingMode mode);

private:
	static SegmentsData segmentsData;
	static std::stringstream& bss;
	static std::stringstream& data;
	static std::stringstream& text;

public:
	Asm() = delete;
};

inline std::ostream& operator<<(std::ostream& stream, Asm::AddressingMode mode)
{
	stream << Asm::addressingModeToString(mode);
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, Asm::Segment segment)
{
	stream << Asm::segmentToString(segment);
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, Asm::Register r)
{
	stream << Asm::registerToString(r);
	return stream;
}

inline void Asm::assembleIntoFile(std::string_view outputFileName)
{
	writeToFile(outputFileName.data(), segmentsData);
}

inline void Asm::setUpShadowZone()
{
#ifdef _DEBUG
	insComment("shadow zone");
#endif
	i_push(Register::RBP);
	i_mov(Register::RBP, Register::RSP);
	i_sub(Register::RSP, 32);
	newLine();
}

inline void Asm::setArgInt(usize pos, u64 val)
{
	// TODO: Map pos to appropriate registers

	Register r;

	switch (pos)
	{
	case 0:
		r = Register::RCX;
		break;
	case 1:
		r = Register::RDX;
		break;
	case 2:
		r = Register::R8;
		break;
	case 3:
		r = Register::R9;
		break;
	default:
		HE_DEBUG_BREAK;
		r = Register::RCX;
		break;
	}

	i_mov(r, val);
}

inline void Asm::setReturnInt(u64 val)
{
	if (val == 0)
	{
		i_xor(Register::RAX, Register::RAX);
		return;
	}

	i_mov(Register::RAX, val);
}

inline void Asm::newLine()
{
	text << NL;
}

inline void Asm::comment(std::string_view comment)
{
	text << ";" << comment.data() << NL;
}

inline void Asm::insComment(std::string_view comment)
{
	text << TAB << ";" << comment.data() << NL;
}

inline void Asm::setBits(usize bits)
{
	text << "bits " << bits << NL;
}

inline void Asm::setAddressingMode(AddressingMode mode)
{
	text << "default " << mode << NL;
}

inline void Asm::setSegment(Segment segment)
{
	text << "segment " << segment << NL;
}

inline void Asm::exportSymbol(const Symbol& symbol)
{
	text << "global " << symbol.name << NL;
}

inline void Asm::exportSymbol(std::string_view name)
{
	text << "global " << name.data() << NL;
}

inline void Asm::importSymbol(const Symbol& symbol)
{
	text << "extern " << symbol.name << NL;
}

inline void Asm::importSymbol(std::string_view name)
{
	text << "extern " << name.data() << NL;
}

inline void Asm::call(const Symbol& symbol)
{
	text << TAB << "call " << symbol.name << NL;
}

inline void Asm::call(std::string_view name)
{
	text << TAB << "call " << name.data() << NL;
}

inline void Asm::call(const Symbol& symbol, u64 val)
{
	setArgInt(0, val);
	text << TAB << "call " << symbol.name << NL;
}

inline void Asm::call(std::string_view name, u64 val)
{
	setArgInt(0, val);
	text << TAB << "call " << name.data() << NL;
}

inline void Asm::setLabel(const Symbol& symbol)
{
	text << symbol.name << ':' << NL;
}

inline void Asm::setLabel(std::string_view name)
{
	text << name.data() << ':' << NL;
}

inline void Asm::i_push(Register r)
{
	text << TAB << "push " << r << NL;
}

inline void Asm::i_mov(Register dst, Register src)
{
	text << TAB << "mov " << dst << ", " << src << NL;
}

inline void Asm::i_mov(Register r, u64 val)
{
	text << TAB << "mov " << r << ", " << val << NL;
}

inline void Asm::i_sub(Register r, u64 val)
{
	text << TAB << "sub " << r << ", " << val << NL;
}

inline void Asm::i_xor(Register a, Register b)
{
	text << TAB << "xor " << a << ", " << b << NL;
}

constexpr const char* Asm::addressingModeToString(AddressingMode mode)
{
	switch (mode)
	{
	case AddressingMode::REL:
		return "rel";
		break;
	case AddressingMode::ABS:
		return "abs";
		break;
	case AddressingMode::BND:
		return "bnd";
		break;
	case AddressingMode::NOBND:
		return "nobnd";
		break;
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}

constexpr const char* Asm::segmentToString(Segment segment)
{
	switch (segment)
	{
	case Segment::BSS:
		return ".bss";
		break;
	case Segment::DATA:
		return ".data";
		break;
	case Segment::TEXT:
		return ".text";
		break;
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}

constexpr const char* Asm::registerToString(Register r)
{
	switch (r)
	{
	case Register::RAX:
		return "rax";
		break;
	case Register::RBX:
		return "rbx";
		break;
	case Register::RCX:
		return "rcx";
		break;
	case Register::RDX:
		return "rdx";
		break;
	case Register::RSI:
		return "rsi";
		break;
	case Register::RDI:
		return "rdi";
		break;
	case Register::RBP:
		return "rbp";
		break;
	case Register::RSP:
		return "rsp";
		break;
	case Register::R8:
		return "r8";
		break;
	case Register::R9:
		return "r9";
		break;
	case Register::R10:
		return "r10";
		break;
	case Register::R11:
		return "r11";
		break;
	case Register::R12:
		return "r12";
		break;
	case Register::R13:
		return "r13";
		break;
	case Register::R14:
		return "r14";
		break;
	case Register::R15:
		return "r15";
		break;
	case Register::XMM0:
		return "xmm0";
		break;
	case Register::XMM1:
		return "xmm1";
		break;
	case Register::XMM2:
		return "xmm2";
		break;
	case Register::XMM3:
		return "xmm3";
		break;
	case Register::XMM4:
		return "xmm4";
		break;
	case Register::XMM5:
		return "xmm5";
		break;
	case Register::XMM6:
		return "xmm6";
		break;
	case Register::XMM7:
		return "xmm7";
		break;
	case Register::XMM8:
		return "xmm8";
		break;
	case Register::XMM9:
		return "xmm9";
		break;
	case Register::XMM10:
		return "xmm10";
		break;
	case Register::XMM11:
		return "xmm11";
		break;
	case Register::XMM12:
		return "xmm12";
		break;
	case Register::XMM13:
		return "xmm13";
		break;
	case Register::XMM14:
		return "xmm14";
		break;
	case Register::XMM15:
		return "xmm15";
		break;
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}
