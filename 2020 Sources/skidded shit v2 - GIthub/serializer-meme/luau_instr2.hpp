
/*
*
*	SYNAPSE X
*	File.:	RbxLuauConversion.cpp
*	Desc.:	Lua -> Luau conversion
*
*/

#pragma once
#include <cstdint>
#include <vector>
#include <windows.h>
#include <sstream>
extern "C" {
#include "lua/lapi.h"
#include "lua/lopcodes.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lobject.h"
#include "lua/lstate.h"
}

template<typename int_t = uint64_t>
size_t encodeVarint(int_t value, uint8_t* output) {
	size_t outputSize = 0;
	//While more than 7 bits of data are left, occupy the last output byte
	// and set the next byte flag
	while (value > 127) {
		//|128: Set the next byte flag
		output[outputSize] = ((uint8_t)(value & 127)) | 128;
		//Remove the seven bits we just wrote
		value >>= 7;
		outputSize++;
	}
	output[outputSize++] = ((uint8_t)value) & 127;

	return outputSize;
}

static void writeVarInt(std::stringstream& ss, uint64_t value) {
	UCHAR buf[8] = { 0 };
	size_t sz_var_out = encodeVarint<uint64_t>(value, buf);
	ss.write(reinterpret_cast<const char*>
		(buf), sz_var_out
	);
}

static void writeWord(std::stringstream& ss, DWORD value) {
	ss.write(reinterpret_cast<const char*>
		(&value), sizeof(value));
}

static void writeDouble(std::stringstream& ss, double value) {
	ss.write(reinterpret_cast<const char*>
		(&value), sizeof(value));
}

static void writeByte(std::stringstream& ss, unsigned char value)
{
	ss.write(reinterpret_cast<const char*>
		(&value), sizeof(value));
}

void writeProto(lua_State* L, std::stringstream& ss, Proto* p)
{
	std::vector<uint8_t> stringTable;
	int TableCounter = 0;
	std::vector<lua_TValue*> kConstant;

	std::vector<TString*> constantStrings;
	kConstant.clear();

	/* Serializing the actual strings of the table size  */

	for (int i = 0; i < p->sizek; i++)
	{
		TValue* k = &p->k[i];
		if (k->tt == LUA_TSTRING) /* Set Value type to TString  */
		{
			TableCounter++;
			kConstant.push_back(k);
		}
		if (k->tt == LUA_TNUMBER)
		{
			writeByte(ss, nvalue(k));
		}
		if (k->tt == LUA_TBOOLEAN)
		{
			writeByte(ss, bvalue(k));
		}
		if (k->tt == LUA_TNIL)
		{
			writeByte(ss, nvalue(k));
		}
	}
	writeVarInt(ss, TableCounter); /* Write string table size */
	for (int i = 0; i < constantStrings.size(); i++)
	{
		std::string Value = getstr(&kConstant[i]->value.gc->ts);
		writeWord(ss, Value.length());
		writeByte(ss, Value[i]);
	}

	for (int i = 0; i < p->sizecode; i++)
	{
		writeVarInt(ss, p->code[i]);
	}

	for (int i = 0; i < p->sizep; i++)
	{
		writeProto(L, ss, p->p[i]);
	}

	int LastLineDefined = 0;

	for (int i = 0; i < p->sizelineinfo; i++)
	{
		writeWord(ss, p->lineinfo[i] - LastLineDefined);
		LastLineDefined = p->lineinfo[i];
	}

	for (int i = 0; i < p->sizelocvars; i++)
	{
		const struct LocVar* l = &p->locvars[i];
		writeWord(ss, l->startpc);
		writeWord(ss, l->endpc);
	}
}

namespace syn
{
	uint8_t LuaU_MagicMul = 1;
}

#pragma pack(push, 1)
union LuauInstruction
{
	struct
	{
		uint8_t OpCode;
		uint8_t A;
		union
		{
			struct
			{
				uint8_t B;
				uint8_t C;
			};
			uint16_t Bx;
			int16_t sBx;
		};
	};
	uint32_t Value;

	[[nodiscard]] uint32_t GetAx() const { return Value >> 8; }
	[[nodiscard]] int32_t GetsAx() const { return (int32_t)GetAx(); }
	void SetAx(const uint32_t value) { Value = Value & 0xFF | value << 8; }
	void SetsAx(const int32_t value) { SetAx((uint32_t)value); }

	explicit LuauInstruction()
	{
		Value = 0;
	}

	explicit LuauInstruction(const Instruction C)
	{
		Value = C;
	}

	void SetOpCode(const uint8_t ROpCode)
	{
		OpCode = (uint8_t)((uint8_t)ROpCode * syn::LuaU_MagicMul);
	}

	explicit operator Instruction() const
	{
		return Value;
	}

	explicit operator DWORD() const
	{
		return Value;
	}
};
static_assert(sizeof(LuauInstruction) == sizeof(uint32_t), "LuauInstruction size mismatch");
#pragma pack(pop)


class OneWayLuauTranslator
{
private:
	Proto* CurrentProto = nullptr;
	enum LuauOp
	{
		OP_NOP = 0x0,
		OP_MUL = 0x9,
		OP_NTEST = 0xE,
		OP_TFORPREP = 0x17,
		OP_SETGLOBAL = 0x18,
		OP_LEN = 0x1C,
		OP_SUB = 0x26,
		OP_TEST = 0x2B,
		OP_SETTABLEK = 0x30,
		OP_GETGLOBAL = 0x35,
		OP_UNM = 0x39,
		OP_ADD = 0x43,
		OP_GETTABLEK = 0x4D,
		OP_MOVE = 0x52,
		OP_NOT = 0x56,
		OP_NLT = 0x60,
		OP_JMP = 0x65,
		OP_SETTABLE = 0x6A,
		OP_LONGJMP = 0x69,
		OP_LOADK = 0x6F,
		OP_CONCAT = 0x73,
		OP_NLE = 0x7D,
		OP_RETURN = 0x82,
		OP_GETTABLE = 0x87,
		OP_FORLOOP = 0x8B,
		OP_LOADNUMBER = 0x8C,
		OP_NEQ = 0x9A,
		OP_CALL = 0x9F,
		OP_CLEARSTACK = 0xA3,
		OP_FORPREP = 0xA8,
		OP_LOADBOOL = 0xA9,
		OP_POW = 0xB2,
		OP_LT = 0xB7,
		OP_SELF = 0xBC,
		OP_CLOSE = 0xC1,
		OP_SETLIST = 0xC5,
		OP_LOADNIL = 0xC6,
		OP_MOD = 0xCF,
		OP_LE = 0xD4,
		OP_SETUPVAL = 0xDE,
		OP_CLOSURE = 0xD9,
		OP_VARARG = 0xDD,
		OP_DIV = 0xEC,
		OP_EQ = 0xF1,
		OP_TFORLOOP = 0xFA,
		OP_GETUPVAL = 0xFB,
		OP_NEWTABLE = 0xFF
	};
public:
	__declspec(noinline) std::vector<LuauInstruction> Convert(Instruction* Instrs, const size_t Size) const
	{
		struct RelocationData
		{
			int OriginalPc;
			int TranslatedPc;
			Instruction OriginalInstr;
			Instruction TranslatedInstr;
		};

		auto VanillaInstrs = std::vector<Instruction>(Instrs, Instrs + Size);
		auto PcTranslation = (int*)malloc(Size * 4);
		auto PcRelocationNeeded = std::vector<RelocationData>();

		//Stage 1 - Relocations.
		

		//Stage 2 - Translation.
		auto LuaUInstrs = std::vector<LuauInstruction>();

		LuauInstruction InitInstr(0);

		InitInstr.SetOpCode(LuauOp::OP_CLEARSTACK);
		InitInstr.A = CurrentProto->is_vararg;
		LuaUInstrs.push_back(InitInstr);

		for (size_t i = 0; i < VanillaInstrs.size(); i++)
		{
			LuauInstruction LuaUInstr(0);
			const auto Instr = VanillaInstrs[i];
			const auto Opc = GET_OPCODE(Instr);

			printf("LUA_OP: %s\n", luaP_opnames[Opc]);
			
			//Explicitly not - 1 as it will be the next instruction pushed.
			PcTranslation[i] = (int)LuaUInstrs.size();

			if (Opc == OpCode::OP_MOVE)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_MOVE);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_LOADK)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_Bx(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_LOADBOOL)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_LOADBOOL);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);
				LuaUInstr.C = GETARG_C(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_LOADNIL)
			{
				//LuaU's OP_LOADNIL only does a single register... we need to do duplications (facepalm)

				auto A = GETARG_A(Instr);
				auto B = GETARG_B(Instr);

				LuaUInstr.SetOpCode(LuauOp::OP_LOADNIL);

				while (B >= A)
				{
					LuaUInstr.A = B--;
					LuaUInstrs.push_back(LuaUInstr);
				}
			}

			else if (Opc == OpCode::OP_GETUPVAL)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_GETUPVAL);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_GETGLOBAL)
			{
				//"OP_GETGLOBAL" was replaced by "OP_GETENV" in LuaU, practically does the exact same thing - has a psuedoinstruction instead of the Bx though. (cuz we really need more psuedoinstructions!)

				LuaUInstr.SetOpCode(LuauOp::OP_GETGLOBAL);
				LuaUInstr.A = GETARG_A(Instr);

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(GETARG_Bx(Instr));
			}

			else if (Opc == OpCode::OP_GETTABLE)
			{
				//"OP_GETTABLE" is split up into 3 different versions in LuaU - OP_GETTABLE, OP_GETTABLEK, and OP_GETTABLEN. OP_GETTABLE and OP_GETTABLEK are the only instructions actually relevant here.

				if (ISK(GETARG_C(Instr)))
				{
					//Use OP_GETTABLEK.

					LuaUInstr.SetOpCode(LuauOp::OP_GETTABLEK);
					LuaUInstr.A = GETARG_A(Instr);
					LuaUInstr.B = GETARG_B(Instr);

					LuaUInstrs.push_back(LuaUInstr);
					LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
				}
				else
				{
					//Use OP_GETTABLE.

					LuaUInstr.SetOpCode(LuauOp::OP_GETTABLE);
					LuaUInstr.A = GETARG_A(Instr);
					LuaUInstr.B = GETARG_B(Instr);
					LuaUInstr.C = GETARG_C(Instr);

					LuaUInstrs.push_back(LuaUInstr);
				}
			}

			else if (Opc == OpCode::OP_SETGLOBAL)
			{
				//The same routine as OP_GETGLOBAL can be done here, just with OP_SETENV instead.

				LuaUInstr.SetOpCode(LuauOp::OP_SETGLOBAL);
				LuaUInstr.A = GETARG_A(Instr);

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(GETARG_Bx(Instr));
			}

			else if (Opc == OpCode::OP_SETUPVAL)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_SETUPVAL);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_SETTABLE)
			{
				//OP_SETTABLE can use A as either a constant or a register - LuaU only supports registers. A temp instruction is needed.

				auto RealSet = GETARG_C(Instr);
				if (ISK(RealSet))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = RealSet - 256;

					LuaUInstrs.push_back(LuaUInstr);

					RealSet = 254;
				}

				if (ISK(GETARG_B(Instr)))
				{
					//Use OP_SETTABLEK.

					LuaUInstr.SetOpCode(LuauOp::OP_SETTABLEK);
					LuaUInstr.A = RealSet;
					LuaUInstr.B = GETARG_A(Instr);
					LuaUInstr.C = 0;

					LuaUInstrs.push_back(LuaUInstr);
					LuaUInstrs.emplace_back(GETARG_B(Instr) - 256);
				}
				else
				{
					//Use OP_SETTABLEV.

					LuaUInstr.SetOpCode(LuauOp::OP_SETTABLE);
					LuaUInstr.A = RealSet;
					LuaUInstr.B = GETARG_A(Instr);
					LuaUInstr.C = GETARG_B(Instr);

					LuaUInstrs.push_back(LuaUInstr);
				}
			}

			else if (Opc == OpCode::OP_NEWTABLE)
			{
				//More useless psuedoinstructions!

				LuaUInstr.SetOpCode(LuauOp::OP_NEWTABLE);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_C(Instr);

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(GETARG_B(Instr));
			}

			else if (Opc == OpCode::OP_SELF)
			{
				//We don't need to relocate here, its done before this code.
				if (ISK(GETARG_C(Instr)))
				{
					//We can use regular OP_SELF.

					LuaUInstr.SetOpCode(LuauOp::OP_SELF);
					LuaUInstr.A = GETARG_A(Instr);
					LuaUInstr.B = GETARG_B(Instr);

					LuaUInstrs.push_back(LuaUInstr);
					LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
				}
				else
				{
					//TODO: Synapse assumption - Register bound OP_SELF's are preceded by a OP_LOADK.

					const auto RInstr = LuaUInstrs.at(LuaUInstrs.size() - 1);
					const auto RealConst = RInstr.Bx;

					LuaUInstrs.pop_back();

					LuaUInstr.SetOpCode(LuauOp::OP_SELF);
					LuaUInstr.A = GETARG_A(Instr);
					LuaUInstr.B = GETARG_B(Instr);

					LuaUInstrs.push_back(LuaUInstr);
					LuaUInstrs.emplace_back(RealConst);
				}
			}

			else if (Opc == OpCode::OP_ADD)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_ADDK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_ADD);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_SUB)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_SUBK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_SUB);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_MUL)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_MULK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_MUL);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_DIV)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_DIVK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_DIV);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_MOD)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_MODK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_MOD);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_POW)
			{
				auto CmpB = GETARG_B(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
					LuaUInstr.A = 254;
					LuaUInstr.Bx = GETARG_B(Instr) - 256;

					LuaUInstrs.push_back(LuaUInstr);

					CmpB = 254;
				}

				LuaUInstr.A = GETARG_A(Instr);

				/*if (ISK(GETARG_C(Instr)))
				{
					LuaUInstr.SetOpCode(LuauOp::OP_POWK);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr) - 256;
				}
				else
				{*/
					LuaUInstr.SetOpCode(LuauOp::OP_POW);
					LuaUInstr.B = CmpB;
					LuaUInstr.C = GETARG_C(Instr);
				//}

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_UNM)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_UNM);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_NOT)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_NOT);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_LEN)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_LEN);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_CONCAT)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_CONCAT);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);
				LuaUInstr.C = GETARG_C(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_JMP)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_JMP);
				LuaUInstr.sBx = 0;

				LuaUInstrs.push_back(LuaUInstr);

				//Add relocation.
				RelocationData Relocation{};
				Relocation.OriginalPc = i;
				Relocation.TranslatedPc = LuaUInstrs.size() - 1;
				Relocation.OriginalInstr = (Instruction)Instr;
				Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
				PcRelocationNeeded.push_back(Relocation);
			}

			else if (Opc == OpCode::OP_EQ)
			{
				uint8_t RegB = GETARG_B(Instr);
				uint8_t RegC = GETARG_C(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 254;
					Temp.Bx = RegB;

					LuaUInstrs.push_back(Temp);

					RegB = 254;
				}

				if (ISK(GETARG_C(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 255;
					Temp.Bx = RegC;

					LuaUInstrs.push_back(Temp);

					RegC = 255;
				}

				if (GETARG_A(Instr))
					LuaUInstr.SetOpCode(LuauOp::OP_EQ);
				else
					LuaUInstr.SetOpCode(LuauOp::OP_NEQ);

				LuaUInstr.A = RegB;

				/* Always paired with a JMP in the vanilla compiler */
				LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(RegC);
			}

			else if (Opc == OpCode::OP_LT)
			{
				uint8_t RegB = GETARG_B(Instr);
				uint8_t RegC = GETARG_C(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 254;
					Temp.Bx = RegB;

					LuaUInstrs.push_back(Temp);

					RegB = 254;
				}

				if (ISK(GETARG_C(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 255;
					Temp.Bx = RegC;

					LuaUInstrs.push_back(Temp);

					RegC = 255;
				}

				if (GETARG_A(Instr))
					LuaUInstr.SetOpCode(LuauOp::OP_LT);
				/*else
					LuaUInstr.SetOpCode(LuauOp::OP_GT);*/

				LuaUInstr.A = RegB;

				/* Always paired with a JMP in the vanilla compiler */
				LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;
				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(RegC);
			}

			else if (Opc == OpCode::OP_LE)
			{
				uint8_t RegB = GETARG_B(Instr);
				uint8_t RegC = GETARG_C(Instr);

				if (ISK(GETARG_B(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 254;
					Temp.Bx = RegB;

					LuaUInstrs.push_back(Temp);

					RegB = 254;
				}

				if (ISK(GETARG_C(Instr)))
				{
					LuauInstruction Temp(0);
					Temp.SetOpCode(LuauOp::OP_LOADK);
					Temp.A = 255;
					Temp.Bx = RegC;

					LuaUInstrs.push_back(Temp);

					RegC = 255;
				}

				if (GETARG_A(Instr))
					LuaUInstr.SetOpCode(LuauOp::OP_LE);
				/*else
					LuaUInstr.SetOpCode(LuauOp::OP_GE);*/

				LuaUInstr.A = RegB;

				/* Always paired with a JMP in the vanilla compiler */
				LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(RegC);
			}

			else if (Opc == OpCode::OP_TEST)
			{
				if (GETARG_C(Instr))
					LuaUInstr.SetOpCode(LuauOp::OP_TEST); // TEST1 is either OP_NTEST or OP_TEST (unsure)
				else
					LuaUInstr.SetOpCode(LuauOp::OP_NTEST);

				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = 1;

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_TESTSET)
			{
				/*
					Reimplementation is required for this instruction - the built in instructions are way too limited.

					We can use this combo to recreate it:
					[1] test0/test1 <b> 2
					[2] move <a> <b>
					[3] jmp <value> --> From the original TESTSET assumption.
					[4] <next instr>
				*/

				if (GETARG_C(Instr))
					LuaUInstr.SetOpCode(LuauOp::OP_TEST); // switch test and ntest if this doesnt work
				else
					LuaUInstr.SetOpCode(LuauOp::OP_NTEST);

				LuaUInstr.A = GETARG_B(Instr);
				LuaUInstr.B = 2;

				LuaUInstrs.push_back(LuaUInstr);

				LuaUInstr.SetOpCode(LuauOp::OP_MOVE);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_CALL || Opc == OpCode::OP_TAILCALL)
			{
				//Tailcalls don't exist within LuaU. We don't need to deal with the psuedo-instructions either, they are handled fine by the regular loop.

				LuaUInstr.SetOpCode(LuauOp::OP_CALL);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);
				LuaUInstr.C = GETARG_C(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_RETURN)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_RETURN);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);
				LuaUInstr.C = GETARG_C(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_FORLOOP)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_FORLOOP);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.sBx = 0;

				LuaUInstrs.push_back(LuaUInstr);

				//Add relocation.
				RelocationData Relocation{};
				Relocation.OriginalPc = i;
				Relocation.TranslatedPc = LuaUInstrs.size() - 1;
				Relocation.OriginalInstr = (Instruction)Instr;
				Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
				PcRelocationNeeded.push_back(Relocation);
			}

			else if (Opc == OpCode::OP_FORPREP)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_FORPREP);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.sBx = 0;

				LuaUInstrs.push_back(LuaUInstr);

				//Add relocation.
				RelocationData Relocation{};
				Relocation.OriginalPc = i;
				Relocation.TranslatedPc = LuaUInstrs.size() - 1;
				Relocation.OriginalInstr = (Instruction)Instr;
				Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
				PcRelocationNeeded.push_back(Relocation);
			}

			else if (Opc == OpCode::OP_TFORLOOP)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_TFORLOOP);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.sBx = 1;

				LuaUInstrs.push_back(LuaUInstr);
				LuaUInstrs.emplace_back(GETARG_C(Instr));
			}

			else if (Opc == OpCode::OP_SETLIST)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_SETLIST);
				LuaUInstr.B = GETARG_A(Instr) + 1; /* Stack start (tbl + 1)  */
				LuaUInstr.C = GETARG_B(Instr) + 1; /* Size of table (b - 1)? */
				LuaUInstrs.push_back(LuaUInstr);

				/* Extended setlist */
				int c;
				if (GETARG_C(Instr) == 0)
					c = VanillaInstrs.at(i++);
				else
					c = GETARG_C(Instr);

				LuaUInstrs.emplace_back((c - 1) * LFIELDS_PER_FLUSH); /* Table start index */
			}

			else if (Opc == OpCode::OP_CLOSE)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_CLOSE);
				LuaUInstr.A = GETARG_A(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_CLOSURE)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_CLOSURE);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.Bx = GETARG_Bx(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}

			else if (Opc == OpCode::OP_VARARG)
			{
				LuaUInstr.SetOpCode(LuauOp::OP_VARARG);
				LuaUInstr.A = GETARG_A(Instr);
				LuaUInstr.B = GETARG_B(Instr);

				LuaUInstrs.push_back(LuaUInstr);
			}
		}

		//Stage 3 - Resolve secondary relocations.
		for (auto& Data : PcRelocationNeeded)
		{
			/*
				We resolve relocations by re-calculating the sBx offset after conversion is complete.

				We keep a table of the PC Vanilla -> PC LuaU offsets (PcTranslation) along with a 'needs-relocation' table (PcRelocationNeeded).

				We can then calculate the actual sBx parameter by getting the actual target instruction (from the PcTranslation table), then rebase the instruction off of that.
			 */

			const auto Target = PcTranslation[Data.OriginalPc + GETARG_sBx(Data.OriginalInstr) + 1];
			const auto Sbx = Target - Data.TranslatedPc - 1;

			if (Sbx > 32768 || Sbx < -32768)
			{
				throw std::exception("todo: large sBx for translation not supported yet");
			}
			else
			{
				LuauInstruction RInstr(Data.TranslatedInstr);
				RInstr.sBx = Sbx;

				LuaUInstrs[Data.TranslatedPc] = RInstr;
			}
		}

		//Free PcTranslation table.
		free(PcTranslation);

		return LuaUInstrs;
	}

	OneWayLuauTranslator(Proto* p)
	{
		CurrentProto = p;
	}
};