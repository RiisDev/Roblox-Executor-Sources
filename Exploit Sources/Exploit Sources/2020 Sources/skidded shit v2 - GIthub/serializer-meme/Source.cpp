#include <Windows.h>
#include <iostream>
#include "luau_instr.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lobject.h"
#include "lua/lmem.h"
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

void writeInt(std::stringstream& ss, unsigned int value)
{
	ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

std::vector<LuauInstruction> LuaUConvert(lua_State* L, Proto* P)
{
	const auto Conv = OneWayLuauTranslator(P).Convert(P->code, P->sizecode);

	P->sizecode = Conv.size();
	P->code = luaM_newvector(L, Conv.size(), Instruction);

	for (auto i = 0; i < Conv.size(); i++)
		P->code[i] = (Instruction)Conv[i];

	for (auto i = 0; i < P->sizep; i++)
		LuaUConvert(L, P->p[i]);

	return Conv;
}

void write_instructions(std::stringstream &ss, std::vector<LuauInstruction> instrs)
{
	for (LuauInstruction x : instrs)
	{
		auto o = x.OpCode;
		int a = x.A;
		int b = x.B;
		int c = x.C;
		int bx = x.Bx;
		int sbx = x.sBx;
		writeByte(ss, o);
		switch (getOpMode(o))
		{
		case iABC:
			writeByte(ss, a);
			if (getBMode(o) != OpArgN) 
				writeByte(ss, ISK(b) ? (-1 - INDEXK(b)) : b);
			if (getCMode(o) != OpArgN) 
				writeByte(ss, ISK(c) ? (-1 - INDEXK(c)) : c);
			break;
		case iABx:
			if (getBMode(o) == OpArgK) {
				writeByte(ss, a);
				writeByte(ss, -1 - bx);
			}
			else {
				writeByte(ss, a);
				writeByte(ss, bx);
			}
			break;
		case iAsBx:
			if (o == OpCode::OP_JMP)
				writeByte(ss, sbx);
			else {
				writeByte(ss, a);
				writeByte(ss, sbx);
			}
			break;
		}
	}
}

void writeCompressedInt(std::stringstream& ss, unsigned int value)
{
	do
	{
		int v = value & 0x7F;
		value >>= 7;

		if (value)
			v |= 0x80;

		writeByte(ss, v);
	} while (value);
}


void write_table(std::stringstream &ss, Proto *p)
{
	std::vector<lua_TValue*> kConstant;
	kConstant.clear();

	for (int i = 0; i < p->sizek; i++)
	{
		TValue* k = &p->k[i];
		if (k->tt == LUA_TSTRING)
		{
			kConstant.push_back(k);
		}
	}
	
	writeCompressedInt(ss, kConstant.size()); /* Write string table size */
	
	for (auto &str : kConstant)
	{
		std::string Value = getstr(&str->value.gc->ts);
		std::cout << "STRING: " << Value << std::endl;
		writeCompressedInt(ss, Value.length());
		for (auto &x : Value)
			writeByte(ss, x);
	}

}




void write_proto(std::stringstream& ss, Proto* p)
{

	p->lastlinedefined = 1;

	write_table(ss, p);




	writeCompressedInt(ss, p->sizecode);
	writeCompressedInt(ss, p->sizek);
	writeCompressedInt(ss, p->sizelineinfo);
	writeCompressedInt(ss, p->sizep);
	writeCompressedInt(ss, p->sizelocvars);
	writeCompressedInt(ss, p->sizeupvalues);

	auto instrs = OneWayLuauTranslator(p).Convert(p->code, p->sizecode);

	write_instructions(ss, instrs);

	writeByte(ss, instrs.size());

	//writeByte(ss, instrs.size());



	for (int i = 0; i < instrs.size(); i++)
	{
		writeInt(ss, (Instruction)instrs[i]);
	}


	for (int i = 0; i < p->sizelineinfo; i++)
	{



		writeCompressedInt(ss, p->lineinfo[i]);

	}

	for (int i = 0; i < p->sizep; i++)
	{
		write_proto(ss, p->p[i]);
	}


	//int LastLineDefined = 0;





	for (int i = 0; i < p->sizelocvars; i++)
	{
		LocVar* l = &p->locvars[i];
		writeCompressedInt(ss, l->startpc);
		writeCompressedInt(ss, l->endpc);
	}

	for (int i = 0; i < p->sizeupvalues; i++)
	{
		ss.write(reinterpret_cast<const char*>(p->upvalues[i]), sizeof(p->upvalues[i]));
	}

}

std::string tohex(const std::string& s, bool upper = false)
{
	std::ostringstream ret;

	unsigned int c;
	for (std::string::size_type i = 0; i < s.length(); ++i)
	{
		c = (unsigned int)(unsigned char)s[i];
		ret << std::hex << std::setfill('0') <<
			std::setw(2) << (upper ? std::uppercase : std::nouppercase) << c;
	}
	return ret.str();
}


void main()
{
	auto* L = luaL_newstate();

	std::string source;
	while (std::getline(std::cin, source)) {
		const auto error = luaL_loadbuffer(L, source.c_str(), source.length(), "@coc");
		if (error == 0)
		{
			const auto* cl = static_cast<const LClosure*>(lua_topointer(L, -1));
			auto* P = cl->p;
			std::stringstream ss;
			writeByte(ss, error ? 0x0 : 0x1); //luau-bit
			write_proto(ss, P);
			std::string out = ss.str();

			std::ofstream ofs("test.bin", std::ios::binary);
			ofs << out;
			ofs.close();

			for (byte x : out)
			{
				std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)x << " ";
			}
			std::cout << std::endl;
			for (byte x : out)
			{
				std::cout << std::hex << std::setfill('_') << std::setw(2) << (char)x << " ";
			}
			
		}

	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DWORD c = 0;
		VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &c);
		*reinterpret_cast<UINT*>(FreeConsole) = 0xC3;
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, nullptr, 0, nullptr);
	}
	return 1;
}