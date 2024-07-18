#pragma once
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <unordered_map>

extern "C" {
#include "../../lua/lobject.h"
#include "../../lua/lopcodes.h"
#include "../../lua/lstring.h"
#include "../../lua/lua.h"
#include "../../lua/lualib.h"
#include "../../lua/lauxlib.h"
}

enum class r_opcode : uint8_t {
	op_loadk = 0x6F,
	op_loadint = 0x8C,
	op_ugetglobal = 0x35,
	op_call = 0x9F,
	op_self = 0xBC,
	op_return = 0x82,
	op_initvararg = 0xA3,
	op_gettablek = 0x4D,
	op_closure = 0xD9,
	op_clupval = 0x12,
	op_move = 0x52,
	op_getupval = 0xFB,
	op_setupval = 0xDE,
	op_settable = 0x6A,
	op_gettable = 0x87,
	op_loadbool = 0xA9,
	op_jmp = 0x65,
	op_loadnil = 0xC6,
	op_setglobal = 0x18,
	op_tforloop = 0x6E,
	op_forprep = 0xA8,
	op_forloop = 0x8B,
	op_len = 0x1C,
	op_mul = 0x09,
	op_pow = 0xB2,
	op_div = 0xEC,
	op_add = 0x43,
	op_sub = 0x26,
	op_mod = 0xCF,
	op_eq = 0xF1,
	op_neq = 0x9A,
	op_lt = 0xB7,
	op_gt = 0x60, //0x7D
	op_le = 0xD4,
	op_ge = 0x7D,
	op_concat = 0x73,
	op_not = 0x56,
	op_unm = 0x39,
	op_test = 0x0E,
	op_ntest = 0x2B,
	op_close = 0xC1,
	op_setlist = 0xC5,
	op_newtable = 0xFF,
	op_vararg = 0xDD,
	op_longjmp = 0x69
};

#pragma pack(push, 1)
union r_instruction {
	struct {
		r_opcode Opcode;
		union {
			struct {
				uint8_t a;
				union {
					struct {
						uint8_t b, c;
					};
					uint16_t Bx;
					int16_t sBx;
				};
			};
			signed int sAx : 24;
		};
	};
	uint32_t Value;
	r_instruction() {}
	r_instruction(r_opcode Opcode, uint8_t a, uint8_t b)
		: Opcode(Opcode), a(a), b(b) {}
	r_instruction(r_opcode Opcode, uint8_t a, uint8_t b, uint8_t c)
		: Opcode(Opcode), a(a), b(b), c(c) {}
	r_instruction(r_opcode Opcode, uint8_t a, uint16_t Bx)
		: Opcode(Opcode), a(a), Bx(Bx) {}
	r_instruction(r_opcode Opcode, uint8_t a, int16_t sBx)
		: Opcode(Opcode), a(a), sBx(sBx) {}
	r_instruction(r_opcode Opcode, int sAx)
		: Opcode(Opcode), sAx(sAx) {}
	r_instruction(uint16_t psuedoinstruction)
		: Value(psuedoinstruction) {}
};
#pragma pack(pop)

struct relocation_index {
	int original_index;
	int luau_index;
	int shift;
	Instruction original_instruction;
	relocation_index(int original_index, int luau_index, Instruction original_instruction, int shift) : original_index(original_index), luau_index(luau_index), original_instruction(original_instruction), shift(shift)
	{}
};

namespace hyperion::conversion
{
	void adjust_lines(std::vector<int>& line_info, int lineinfo, std::size_t new_instructions_amount);

	void convert_instructions(std::vector<r_instruction>& instruction_vector, std::vector<int>& line_info, Proto* proto);
}