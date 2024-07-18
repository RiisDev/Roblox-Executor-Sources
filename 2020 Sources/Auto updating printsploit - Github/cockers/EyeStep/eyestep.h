#ifndef H_EYESTEP
#define H_EYESTEP
#define MAX_INSTR_READBITS 128
#pragma warning(disable:4996)

/* Made by static, 8/22/2020
 * If you modify, redistribute, or copy parts of this source code
 * I'd highly appreciate some credit.
 * Do me a favor and leave this comment in.
 * I'm too poor to afford a legal copyright so enjoy
*/

#include <cstdint>
#include <vector>
#include <string>

#define PRE_REPNE   			0x0001
#define PRE_REPE   				0x0002
#define PRE_66   				0x0004
#define PRE_67   				0x0008
#define PRE_LOCK 				0x0010
#define PRE_SEG_CS  			0x0020
#define PRE_SEG_SS  			0x0040
#define PRE_SEG_DS  			0x0080
#define PRE_SEG_ES  			0x0100
#define PRE_SEG_FS  			0x0200
#define PRE_SEG_GS  			0x0400

#define OP_LOCK					0xF0
#define OP_REPNE				0xF2
#define OP_REPE					0xF3
#define OP_66					0x66
#define OP_67					0x67
#define OP_SEG_CS				0x2E
#define OP_SEG_SS				0x36
#define OP_SEG_DS				0x3E
#define OP_SEG_ES				0x26
#define OP_SEG_FS				0x64
#define OP_SEG_GS				0x65


// low-grade filters(or flags) for instructions

#define OP_NONE					0x00000000 // undefined or blank opcode
#define OP_SINGLE				0x00000001 // single operand in opcode (only Source)
#define OP_SRC_DEST				0x00000002 // two operands in opcode (Typical) (Source and Destination)
#define OP_EXTENDED				0x00000004 // more than two operands in the opcode
#define OP_IMM8					0x00000010 // this operand has an 8-bit offset
#define OP_IMM16				0x00000020 // this operand has a 16-bit offset
#define OP_IMM32				0x00000040 // this operand has a 32-bit offset
#define OP_DISP8				0x00000080 // this operand has an 8-bit constant value
#define OP_DISP16				0x00000100 // this operand has a 16-bit constant value
#define OP_DISP32				0x00000200 // this operand has a 32-bit constant value
#define OP_R8					0x00000400
#define OP_R16					0x00000800
#define OP_R32					0x00001000
#define OP_R64					0x00002000
#define OP_XMM					0x00004000
#define OP_MM					0x00008000
#define OP_ST					0x00010000
#define OP_SREG					0x00020000
#define OP_DR					0x00040000
#define OP_CR					0x00080000

namespace EyeStep
{
	extern HMODULE base_module;
	extern size_t base_module_size;
	extern bool external_mode;

	enum
	{
		R8_AL,
		R8_CL,
		R8_DL,
		R8_BL,
		R8_AH,
		R8_CH,
		R8_DH,
		R8_BH,
	};

	enum
	{
		R16_AX,
		R16_CX,
		R16_DX,
		R16_BX,
		R16_SP,
		R16_BP,
		R16_SI,
		R16_DI,
	};

	enum
	{
		R32_EAX,
		R32_ECX,
		R32_EDX,
		R32_EBX,
		R32_ESP,
		R32_EBP,
		R32_ESI,
		R32_EDI,
	};

	// See http://ref.x86asm.net/coder32.html documentation
	// These are all the operand-types currently supported
	// on the x86 chip set
	enum OP_TYPES : uint8_t
	{
		AL,
		AH,
		AX,
		EAX,
		ECX,
		EDX,
		ESP,
		EBP,
		CL,
		CX,
		DX,
		Sreg,
		ptr16_32,
		Flags,
		EFlags,
		ES,
		CS,
		DS,
		SS,
		FS,
		GS,
		one,
		r8,
		r16,
		r16_32,
		r32,
		r64,
		r_m8,
		r_m16,
		r_m16_32,
		r_m16_m32,
		r_m32,
		moffs8,
		moffs16_32,
		m16_32_and_16_32,
		m,
		m8,
		m14_28,
		m16,
		m16_32,
		m16_int,
		m32,
		m32_int,
		m32real,
		m64,
		m64real,
		m80real,
		m80dec,
		m94_108,
		m128,
		m512,
		rel8,
		rel16,
		rel16_32,
		rel32,
		imm8,
		imm16,
		imm16_32,
		imm32,
		mm,
		mm_m64,
		xmm,
		xmm0,
		xmm_m32,
		xmm_m64,
		xmm_m128,
		STi,
		ST1,
		ST2,
		ST,
		LDTR,
		GDTR,
		IDTR,
		PMC,
		TR,
		XCR,
		MSR,
		MSW,
		CRn,
		DRn,
		CR0,
		DR0,
		DR1,
		DR2,
		DR3,
		DR4,
		DR5,
		DR6,
		DR7,
		IA32_TIMESTAMP_COUNTER,
		IA32_SYS,
		IA32_BIOS,
	};

	struct OP_INFO
	{
		std::string code;
		const char* opcode_name;
		std::vector<OP_TYPES> operands;
		const char* description;
	};

	struct operand
	{
		operand()
		{
			rel8 = 0;
			rel16 = 0;
			rel32 = 0;
			imm8 = 0;
			imm16 = 0;
			imm32 = 0;
			disp8 = 0;
			disp16 = 0;
			disp32 = 0;
			mul = 0;
			opmode = 0;
			flags = 0;
		}

		~operand()
		{
		}

		uint32_t flags;
		uint8_t opmode;
		std::vector<uint8_t> reg;
		uint8_t mul; // single multiplier

		uint8_t append_reg(uint8_t reg_type)
		{
			reg.push_back(reg_type);
			return reg_type;
		}

		union
		{
			uint8_t rel8;
			uint16_t rel16;
			uint32_t rel32;
		};

		union
		{
			uint8_t imm8;
			uint16_t imm16;
			uint32_t imm32;
		};

		union
		{
			uint8_t disp8;
			uint16_t disp16;
			uint32_t disp32;
		};
	};

	struct inst
	{
		char data[256];
		OP_INFO info;

		uint32_t flags;
		uint8_t bytes[MAX_INSTR_READBITS / 8];
		size_t len;
		uintptr_t address;
		std::vector<operand>operands;

		inst()
		{
			data[0] = '\0';
			info = OP_INFO();

			operands = std::vector<operand>(4);

			address = 0;
			flags = 0;
			len = 0;
		}

		~inst()
		{
			operands.clear();
		}

		operand source()
		{
			if (operands.size() <= 0) return operand();
			return operands[0];
		}

		operand destination()
		{
			if (operands.size() <= 1) return operand();
			return operands[1];
		}
	};

	extern void* current_proc;

	extern HMODULE get_base_module(HANDLE proc, std::wstring process_name);
	extern void open(HANDLE current_proc, HMODULE _module = nullptr);
	extern void open(std::wstring process_name);
	extern inst read(uintptr_t address);

	extern std::vector<EyeStep::inst> read(uintptr_t address, int count);
	extern std::vector<EyeStep::inst> read_range(uintptr_t address_from, uintptr_t address_to);

	extern uint8_t to_byte(std::string, int offset = 0);
	extern std::string to_str(uint8_t);
}

#endif

