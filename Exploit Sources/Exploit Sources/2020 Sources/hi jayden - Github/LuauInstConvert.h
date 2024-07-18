#pragma once
#include <Windows.h>
#include <vector>


extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#include "lua/lobject.h"
#include "lua/lstring.h"
#include "lua/lopcodes.h"
#include "lua/lfunc.h"
#include "lua/ldo.h"
}

#include <cstdint>
#include <optional>

enum class UOpCode : uint8_t
{
	NOP = 0x0,
	MUL = 0x9,
	NTEST = 0xE,
	TFORPREP = 0x17,
	SETGLOBAL = 0x18,
	LEN = 0x1C,
	SUB = 0x26,
	TEST = 0x2B,
	SETTABLEK = 0x30,
	GETGLOBAL = 0x35,
	UNM = 0x39,
	ADD = 0x43,
	GETTABLEK = 0x4D,
	MOVE = 0x52,
	NOT = 0x56,
	NLT = 0x60,
	JMP = 0x65,
	SETTABLE = 0x6A,
	LONGJMP = 0x69,
	LOADK = 0x6F,
	CONCAT = 0x73,
	NLE = 0x7D,
	RETURN = 0x82,
	GETTABLE = 0x87,
	FORLOOP = 0x8B,
	LOADNUMBER = 0x8C,
	NEQ = 0x9A,
	CALL = 0x9F,
	CLEARSTACK = 0xA3,
	FORPREP = 0xA8,
	LOADBOOL = 0xA9,
	POW = 0xB2,
	LT = 0xB7,
	SELF = 0xBC,
	CLOSE = 0xC1,
	SETLIST = 0xC5,
	LOADNIL = 0xC6,
	MOD = 0xCF,
	LE = 0xD4,
	SETUPVAL = 0xDE,
	CLOSURE = 0xD9,
	VARARG = 0xDD,
	DIV = 0xEC,
	EQ = 0xF1,
	TFORLOOP = 0xFA,
	GETUPVAL = 0xFB,
	NEWTABLE = 0xFF
};

#pragma pack(push)
#pragma pack(1)
class UInstruction
{
public:
	union
	{
		struct
		{
			UOpCode op;
			struct
			{
				uint8_t a;

				union
				{
					struct
					{
						uint8_t b;
						uint8_t c;
					};

					uint16_t bx;
					int16_t sbx;
				};
			};
		};

		uint32_t all;
	};

	explicit UInstruction()
	{
		all = 0;
	}

	explicit UInstruction(const Instruction C)
	{
		all = C;
	}


	explicit operator Instruction() const
	{
		return all;
	}

	explicit operator DWORD() const
	{
		return all;
	}
};
#pragma pack(pop)



class script_ware_utils
{
public:
	static inline Instruction generateABC(UOpCode op, int a, int b, int c)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.b = b;
		uint.c = c;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateABx(UOpCode op, int a, int bx)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.bx = bx;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateAsBx(UOpCode op, int a, int sbx)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.sbx = sbx;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateAU(UOpCode op, int a)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateAB(UOpCode op, int a, int b)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.b = b;
		return (Instruction)(UInstruction)uint;
	}
	////////////////////////////////////////////////////
	static inline Instruction generateABC(int a, int b, int c)
	{
		UInstruction uint;
		uint.a = a;
		uint.b = b;
		uint.c = c;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateABx(int a, int bx)
	{
		UInstruction uint;
		uint.a = a;
		uint.bx = bx;
		return (Instruction)(UInstruction)uint;
	}
	static inline Instruction generateAsBx(int a, int sbx)
	{
		UInstruction uint;
		uint.a = a;
		uint.sbx = sbx;
		return (Instruction)(UInstruction)uint;
	}
};

script_ware_utils* insertInst;



namespace script_ware
{
	class Instructions
	{
	public:
		static __forceinline std::vector<UInstruction> ConvertStart(Instruction* Instructions, size_t Size)
		{
			auto VanillaInstrs = std::vector<Instruction>(Instructions, Instructions + Size);
			auto LuaUInstrs = std::vector<UInstruction>();


			struct RelocationData
			{
				int OriginalPc;
				int TranslatedPc;
				Instruction OriginalInstr;
				Instruction TranslatedInstr;
			};
			auto PcTranslation = (int*)malloc(Size * 4);
			auto PcRelocationNeeded = std::vector<RelocationData>();


			int rinst = 0;

		
			for (auto inster = 0; inster < VanillaInstrs.size(); inster++)
			{
				const auto Instr = VanillaInstrs[inster];

				const auto Opc = GET_OPCODE(Instr);

		

				PcTranslation[inster] = (int)LuaUInstrs.size();


				printf("LUA_OP: %s\n", luaP_opnames[Opc]);

				switch (Opc)
				{
				case OP_MOVE:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::MOVE, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;

				case OP_LOADK:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_LOADBOOL:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::LOADBOOL, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				case OP_LOADNIL:
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::LOADNIL, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				case OP_GETUPVAL:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::GETUPVAL, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_GETGLOBAL:
					LuaUInstrs.emplace_back(insertInst->generateAU(UOpCode::GETGLOBAL, GETARG_A(Instr)));//, GETARG_Bx(Instr)));
					LuaUInstrs.emplace_back(GETARG_Bx(Instr));
					break;
				case OP_GETTABLE:
					if (ISK(GETARG_C(Instr)))
					{
						std::cout << "instr inst : " << std::hex << Instr << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::GETTABLEK, GETARG_A(Instr), GETARG_B(Instr)));
						LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::GETTABLE, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					}
					break;
				case OP_SETGLOBAL:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::SETGLOBAL, GETARG_A(Instr), GETARG_Bx(Instr)));
					LuaUInstrs.emplace_back(GETARG_Bx(Instr));
					break;
				case OP_SETUPVAL:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::SETUPVAL, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_SETTABLE:
				{
					auto RealC = GETARG_C(Instr);
					if (ISK(RealC))
					{
						std::cout << "isk is on c" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_C(Instr) - 256)); //GETARG_Bx(Instr)));
						RealC = 254;
					}
					if (ISK(GETARG_B(Instr)))
					{
						std::cout << "instr is on isk b\n";
						LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::SETTABLEK, RealC, GETARG_A(Instr)));
						LuaUInstrs.emplace_back(GETARG_B(Instr) - 256);
					}
					else
					{
						std::cout << "else, a, b, c" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::SETTABLE, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					}
					break;
				}
				case OP_NEWTABLE:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::NEWTABLE, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				case OP_SELF:
					if (ISK(GETARG_C(Instr)))
					{
						LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::SELF, GETARG_A(Instr), GETARG_B(Instr)));
						LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
					}
					else
					{
						const auto RInstr = LuaUInstrs.at(LuaUInstrs.size() - 1);
						const auto RealConst = RInstr.bx;

						LuaUInstrs.pop_back();
						LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::SELF, GETARG_A(Instr), GETARG_B(Instr)));
						LuaUInstrs.emplace_back(RealConst);
					}
					break;
				case OP_ADD:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::ADD, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_SUB:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::SUB, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_MUL:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::MUL, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_DIV:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::DIV, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_MOD:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::MOD, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_POW:
				{
					auto RealB = GETARG_B(Instr);
					if (ISK(RealB))
					{
						std::cout << "Isk real b" << std::endl;
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, GETARG_B(Instr)) - 256);
						RealB = 254;
					}
					else
					{
						LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::POW, GETARG_A(Instr), RealB, GETARG_C(Instr)));
					}
					break;
				}
				case OP_UNM:
				{
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::UNM, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				}

				case OP_NOT:
				{
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::NOT, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				}

				case OP_LEN:
				{
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::LEN, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				}

				case OP_CONCAT:
				{
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::CONCAT, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				}

				case OP_JMP:
				{
					LuaUInstrs.emplace_back(insertInst->generateAsBx(UOpCode::JMP, GETARG_A(Instr), GETARG_sBx(Instr)));
					//Add relocation.
					RelocationData Relocation{};
					Relocation.OriginalPc = inster;
					Relocation.TranslatedPc = LuaUInstrs.size() - 1;
					Relocation.OriginalInstr = (Instruction)Instr;
					Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
					PcRelocationNeeded.push_back(Relocation);
					break;
				}

				case OP_EQ:
				{
					auto regb = GETARG_B(Instr);
					auto regc = GETARG_C(Instr);
					if (ISK(regb))
					{
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 254, regb));
						regb = 254;
					}
					if (ISK(GETARG_C(Instr)))
					{
						LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, 255, regc));
						regc = 255;
					}

					if (GETARG_A(Instr))
					{
						UInstruction instr(0);
						instr.op = static_cast<UOpCode>(UOpCode::EQ);
						LuaUInstrs.emplace_back(instr);
					}
					else
					{
						UInstruction instr(0);
						instr.op = static_cast<UOpCode>(UOpCode::NEQ);
						LuaUInstrs.emplace_back(instr);
					}

					UInstruction a_instr(0);
					a_instr.a = regb;
					a_instr.sbx = GETARG_sBx(VanillaInstrs[++inster]) + 1;
					LuaUInstrs.emplace_back(a_instr);
					break;
				}

				case OP_CALL:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::CALL, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				case OP_RETURN:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::RETURN, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				case OP_FORLOOP: {
					LuaUInstrs.emplace_back(insertInst->generateAsBx(UOpCode::FORLOOP, GETARG_A(Instr), GETARG_sBx(Instr)));
					//Add relocation.
					RelocationData Relocation{};
					Relocation.OriginalPc = inster;
					Relocation.TranslatedPc = LuaUInstrs.size() - 1;
					Relocation.OriginalInstr = (Instruction)Instr;
					Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
					PcRelocationNeeded.push_back(Relocation);
					break;
				}
				case OP_FORPREP:
				{
					LuaUInstrs.emplace_back(insertInst->generateAsBx(UOpCode::FORPREP, GETARG_A(Instr), GETARG_sBx(Instr)));
					//Add relocation.
					RelocationData Relocation{};
					Relocation.OriginalPc = inster;
					Relocation.TranslatedPc = LuaUInstrs.size() - 1;
					Relocation.OriginalInstr = (Instruction)Instr;
					Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
					PcRelocationNeeded.push_back(Relocation);
					break;
				}
				case OP_TFORLOOP:
				{
					LuaUInstrs.emplace_back(insertInst->generateAU(UOpCode::TFORLOOP, GETARG_A(Instr))); //GETARG_sBx(Instr)));
					LuaUInstrs.emplace_back(GETARG_C(Instr));
					break;
				}

				case OP_SETLIST:
				{
			
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::SETLIST, GETARG_A(Instr) + 1, GETARG_B(Instr) + 1));
				
					int c;
					if (GETARG_C(Instr) == 0)
						c = VanillaInstrs.at(inster++);
					else
						c = GETARG_C(Instr);

					LuaUInstrs.emplace_back((c - 1)* LFIELDS_PER_FLUSH);

					break;
				}

				case OP_CLOSE:
				{
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::CLOSE, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				}

				case OP_CLOSURE:
				{
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::CLOSURE, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				}

				case OP_VARARG:
				{
					LuaUInstrs.emplace_back(insertInst->generateAB(UOpCode::VARARG, GETARG_A(Instr), GETARG_B(Instr)));
					break;
				}

				for (auto& Data : PcRelocationNeeded)
				{
					const auto Target = PcTranslation[Data.OriginalPc + GETARG_sBx(Data.OriginalInstr) + 1];
					const auto Sbx = Target - Data.TranslatedPc - 1;

					UInstruction RInstr(Data.TranslatedInstr);
					RInstr.sbx = Sbx;

					LuaUInstrs[Data.TranslatedPc] = RInstr;
				}

				free(PcTranslation);

				return LuaUInstrs;
				}
			}
		}
	};
}

script_ware::Instructions StartConvert;