#pragma once
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

	std::optional<uint32_t> constant;

	UInstruction() : all(0), constant() {}
};
#pragma pack(pop)

