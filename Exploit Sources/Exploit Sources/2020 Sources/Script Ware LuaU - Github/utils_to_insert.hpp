#pragma once
#include "RbxOp.hpp"

class script_ware_utils
{
public:
	static inline UInstruction generateABC(UOpCode op, int a, int b, int c)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.b = b;
		uint.c = c;
		return uint;
	}
	static inline UInstruction generateABx(UOpCode op, int a, int bx)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.bx = bx;
		return uint;
	}
	static inline UInstruction generateAsBx(UOpCode op, int a, int sbx)
	{
		UInstruction uint;
		uint.op = op;
		uint.a = a;
		uint.sbx = sbx;
		return uint;
	}
};

script_ware_utils* insertInst;