#include "Compiler.h"

#include "file.h"

SegmentsData Compiler::segmentsData{};
std::stringstream& Compiler::bss = Compiler::segmentsData.bss;
std::stringstream& Compiler::data = Compiler::segmentsData.data;
std::stringstream& Compiler::text = Compiler::segmentsData.text;

const Args* Compiler::args = nullptr;
const std::vector<Token>* Compiler::tokens = nullptr;

void Compiler::compileIntoFile(const Args& args, const std::vector<Token>& tokens,
	std::string_view outputFileName)
{
	Compiler::args = &args;
	Compiler::tokens = &tokens;

	Asm::setBits(64);
	Asm::setAddressingMode(Asm::AddressingMode::REL);
	Asm::newLine();

	Asm::setSegment(Asm::Segment::TEXT);
	Asm::exportSymbol("main");
	Asm::importSymbol("_CRT_INIT");
	Asm::importSymbol("ExitProcess");
	Asm::newLine();

	Asm::setLabel("main");

	setUpShadowZone();

	Asm::call("_CRT_INIT");
	Asm::newLine();

	setReturnInt(0);
	setArgInt(0, tokens[1].literal->value);

	Asm::call("ExitProcess");

	writeToFile(outputFileName, segmentsData);
}

void Compiler::compileStatement(usize startTokenIndex, usize endTokenIndex)
{
	
}

inline void Compiler::setUpShadowZone()
{
#ifdef _DEBUG
	Asm::insComment("shadow zone");
#endif
	Asm::i_push(Asm::Register::RBP);
	Asm::i_mov(Asm::Register::RBP, Asm::Register::RSP);
	Asm::i_sub(Asm::Register::RSP, 32);
	Asm::newLine();
}

inline void Compiler::setReturnInt(u64 val)
{
	if (val == 0)
	{
		Asm::i_xor(Asm::Register::RAX, Asm::Register::RAX);
		return;
	}

	Asm::i_mov(Asm::Register::RAX, val);
}

inline void Compiler::setArgInt(usize pos, u64 val)
{
	// TODO: Map pos to appropriate registers

	Asm::Register r;

	switch (pos)
	{
	case 0:
		r = Asm::Register::RCX;
		break;
	case 1:
		r = Asm::Register::RDX;
		break;
	case 2:
		r = Asm::Register::R8;
		break;
	case 3:
		r = Asm::Register::R9;
		break;
	default:
		HE_DEBUG_BREAK;
		break;
	}

	Asm::i_mov(r, val);
}

inline void Compiler::Asm::newLine()
{
	text << NL;
}

inline void Compiler::Asm::comment(std::string_view comment)
{
	text << ";" << comment << NL;
}

inline void Compiler::Asm::insComment(std::string_view comment)
{
	text << TAB << ";" << comment << NL;
}

inline void Compiler::Asm::setBits(usize bits)
{
	text << "bits " << bits << NL;
}

inline void Compiler::Asm::setAddressingMode(AddressingMode mode)
{
	text << "default " << mode << NL;
}

inline void Compiler::Asm::setSegment(Segment segment)
{
	text << "segment " << segment << NL;
}

inline void Compiler::Asm::exportSymbol(const Symbol& symbol)
{
	text << "global " << symbol.name << NL;
}

inline void Compiler::Asm::exportSymbol(std::string_view name)
{
	text << "global " << name.data() << NL;
}

inline void Compiler::Asm::importSymbol(const Symbol& symbol)
{
	text << "global " << symbol.name << NL;
}

inline void Compiler::Asm::importSymbol(std::string_view name)
{
	text << "extern " << name.data() << NL;
}

inline void Compiler::Asm::call(const Symbol& symbol)
{
	text << TAB << "call " << symbol.name << NL;
}

inline void Compiler::Asm::call(std::string_view name)
{
	text << TAB << "call " << name.data() << NL;
}

inline void Compiler::Asm::setLabel(const Symbol& symbol)
{
	text << symbol.name << ':' << NL;
}

inline void Compiler::Asm::setLabel(std::string_view name)
{
	text << name.data() << ':' << NL;
}

inline void Compiler::Asm::i_push(Register r)
{
	text << TAB << "push " << r << NL;
}

inline void Compiler::Asm::i_mov(Register dst, Register src)
{
	text << TAB << "mov " << dst << ", " << src << NL;
}

inline void Compiler::Asm::i_mov(Register r, u64 val)
{
	text << TAB << "mov " << r << ", " << val << NL;
}

inline void Compiler::Asm::i_sub(Register r, u64 val)
{
	text << TAB << "sub " << r << ", " << val << NL;
}

inline void Compiler::Asm::i_xor(Register a, Register b)
{
	text << TAB << "xor " << a << ", " << b << NL;
}

Compiler::Asm::Symbol::Symbol(std::string&& name) : name(std::move(name)) {}

constexpr std::string operator+(const char* left, const Compiler::Asm::AddressingMode& right)
{
	return std::string(left) + Compiler::Asm::addressingModeToString(right);
}

constexpr std::string operator+(const char* left, const Compiler::Asm::Segment& right)
{
	return std::string(left) + Compiler::Asm::segmentToString(right);
}

constexpr std::string operator+(const char* left, const Compiler::Asm::Register& right)
{
	return std::string(left) + Compiler::Asm::registerToString(right);
}

inline std::ostream& operator<<(std::ostream& stream, const Compiler::Asm::AddressingMode& mode)
{
	stream << Compiler::Asm::addressingModeToString(mode);
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Compiler::Asm::Segment& segment)
{
	stream << Compiler::Asm::segmentToString(segment);
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Compiler::Asm::Register& r)
{
	stream << Compiler::Asm::registerToString(r);
	return stream;
}

constexpr std::string Compiler::Asm::addressingModeToString(AddressingMode mode)
{
	switch (mode)
	{
	case Compiler::Asm::AddressingMode::REL:
		return "rel";
		break;
	case Compiler::Asm::AddressingMode::ABS:
		return "abs";
		break;
	case Compiler::Asm::AddressingMode::BND:
		return "bnd";
		break;
	case Compiler::Asm::AddressingMode::NOBND:
		return "nobnd";
		break;
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}

constexpr std::string Compiler::Asm::segmentToString(Segment segment)
{
	switch (segment)
	{
	case Compiler::Asm::Segment::BSS:
		return ".bss";
		break;
	case Compiler::Asm::Segment::DATA:
		return ".data";
		break;
	case Compiler::Asm::Segment::TEXT:
		return ".text";
		break;
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}

constexpr std::string Compiler::Asm::registerToString(Register r)
{
	switch (r)
	{
	case Compiler::Asm::Register::RAX:
		return "rax";
		break;
	case Compiler::Asm::Register::RBX:
		return "rbx";
		break;
	case Compiler::Asm::Register::RCX:
		return "rcx";
		break;
	case Compiler::Asm::Register::RDX:
		return "rdx";
		break;
	case Compiler::Asm::Register::RSI:
		return "rsi";
		break;
	case Compiler::Asm::Register::RDI:
		return "rdi";
		break;
	case Compiler::Asm::Register::RBP:
		return "rbp";
		break;
	case Compiler::Asm::Register::RSP:
		return "rsp";
		break;
	case Compiler::Asm::Register::R8:
		return "r8";
		break;
	case Compiler::Asm::Register::R9:
		return "r9";
		break;
	case Compiler::Asm::Register::R10:
		return "r10";
		break;
	case Compiler::Asm::Register::R11:
		return "r11";
		break;
	case Compiler::Asm::Register::R12:
		return "r12";
		break;
	case Compiler::Asm::Register::R13:
		return "r13";
		break;
	case Compiler::Asm::Register::R14:
		return "r14";
		break;
	case Compiler::Asm::Register::R15:
		return "r15";
		break;
	case Compiler::Asm::Register::XMM0:
		return "xmm0";
		break;
	case Compiler::Asm::Register::XMM1:
		return "xmm1";
		break;
	case Compiler::Asm::Register::XMM2:
		return "xmm2";
		break;
	case Compiler::Asm::Register::XMM3:
		return "xmm3";
		break;
	case Compiler::Asm::Register::XMM4:
		return "xmm4";
		break;
	case Compiler::Asm::Register::XMM5:
		return "xmm5";
		break;
	case Compiler::Asm::Register::XMM6:
		return "xmm6";
		break;
	case Compiler::Asm::Register::XMM7:
		return "xmm7";
		break;
	case Compiler::Asm::Register::XMM8:
		return "xmm8";
		break;
	case Compiler::Asm::Register::XMM9:
		return "xmm9";
		break;
	case Compiler::Asm::Register::XMM10:
		return "xmm10";
		break;
	case Compiler::Asm::Register::XMM11:
		return "xmm11";
		break;
	case Compiler::Asm::Register::XMM12:
		return "xmm12";
		break;
	case Compiler::Asm::Register::XMM13:
		return "xmm13";
		break;
	case Compiler::Asm::Register::XMM14:
		return "xmm14";
		break;
	case Compiler::Asm::Register::XMM15:
		return "xmm15";
		break;	
	}

	HE_DEBUG_BREAK;
	return ERR_STR;
}
