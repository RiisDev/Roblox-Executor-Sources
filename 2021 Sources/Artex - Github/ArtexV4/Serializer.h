#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Translator.h"
#include "GeneralUpdates.hpp"
#include <fstream>
#include <sstream>

extern "C" {
	// lua core
#include "Lua\lua.h"
#include "Lua\lauxlib.h"
#include "Lua\lualib.h"
	// lua internals
#include "Lua\lobject.h"
#include "Lua\lstate.h"
#include "Lua\lfunc.h"
#include "Lua\lopcodes.h"
	// lparser.c modified for roblox's op_closure change
}

class VectorStream
{
public:


	std::ostringstream ss;


	std::string str() {
		return ss.str();
	}
	void writeByte(unsigned char value)
	{

		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	void writeInt(int value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	void writeDouble(double value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	void writeCompressedInt(unsigned int value)
	{
		do
		{
			int v = value & 0x7F;
			value >>= 7;

			if (value)
				v |= 0x80;

			writeByte(v);
		} while (value);
	}
	void writeString(std::string value)
	{
		writeCompressedInt(value.length());
		for (int i = 0; i < value.length(); i++) {
			writeByte(value[i]);
		}
	}

};

static void writeProto(VectorStream& vs, std::unordered_map<TString*, int>& stringTable, std::unordered_map<Proto*, int>& protoTable, Proto* p);

class StringTable
{
	VectorStream& vs;

	size_t stringCount = 1;
	std::unordered_map<TString*, int> stringMap;
	std::vector<TString*> strings;

	void getProtoStrings(Proto* p)
	{
		for (int i = 0; i < p->sizek; i++)
		{
			if (p->k[i].tt == LUA_TSTRING)
				insert(&p->k[i].value.gc->ts);
		}

		for (int i = 0; i < p->sizep; i++)
			getProtoStrings(p->p[i]);
	}

public:
	void insert(TString* ts)
	{
		if (stringMap.find(ts) != stringMap.end())
			return;
		stringMap[ts] = stringCount++;
		strings.push_back(ts);
	}

	std::unordered_map<TString*, int> write(Proto* p)
	{
		getProtoStrings(p);

		vs.writeCompressedInt(strings.size());
		for (TString* ts : strings)
			vs.writeString(std::string(getstr(ts), ts->tsv.len));

		return stringMap;
	}

	StringTable(VectorStream& vs) : vs(vs) {}
};

class ProtoTable
{
	VectorStream& vs;

	size_t protoCount = 0;
	std::unordered_map<Proto*, int> protoMap;
	std::vector<Proto*> protos;

	void addProto(Proto* p)
	{
		for (int i = 0; i < p->sizep; i++)
			addProto(p->p[i]);

		protoMap[p] = protoCount++;
		protos.push_back(p);
	}

public:
	std::unordered_map<Proto*, int> write(Proto* p, std::unordered_map<TString*, int>& stringTable)
	{
		addProto(p);

		vs.writeCompressedInt(protos.size());
		for (Proto* np : protos)
			writeProto(vs, stringTable, protoMap, np);

		return protoMap;
	}

	ProtoTable(VectorStream& vs) : vs(vs) {}
};

enum ConstantType
{
	ConstantNil,
	ConstantBoolean,
	ConstantNumber,
	ConstantString
};

void writeProto(VectorStream& vs, std::unordered_map<TString*, int>& stringTable, std::unordered_map<Proto*, int>& protoTable, Proto* p)
{
	std::vector<int> new_lineinfo;
	auto instructions = push_instrs(p, new_lineinfo);


	vs.writeByte(p->maxstacksize);
	vs.writeByte(p->numparams);
	vs.writeByte(p->nups);
	vs.writeByte(p->is_vararg);

	vs.writeCompressedInt(instructions.size());
	for (size_t i = 0; i < instructions.size(); ++i)
	{
		vs.writeInt((Instruction)instructions[i]);
	}

	vs.writeCompressedInt(p->sizek);
	for (int i = 0; i < p->sizek; ++i)
	{
		const TValue* o = &p->k[i];

		switch (ttype(o))
		{
		case LUA_TNIL:
			vs.writeByte(ConstantNil);
			break;

		case LUA_TBOOLEAN:
			vs.writeByte(ConstantBoolean);
			vs.writeByte(bvalue(o));
			break;

		case LUA_TNUMBER:
			vs.writeByte(ConstantNumber);
			vs.writeDouble(nvalue(o));
			break;

		case LUA_TSTRING:
			vs.writeByte(ConstantString);
			vs.writeCompressedInt(stringTable[rawtsvalue(o)]);
			break;
		}
	}
	vs.writeCompressedInt(p->sizep);
	for (int i = 0; i < p->sizep; ++i)
	{
		vs.writeCompressedInt(protoTable[p->p[i]]);
	}

	vs.writeByte(false);
	vs.writeByte(0);
	vs.writeByte(false);
}

static LClosure* compile(lua_State* L, const std::string& src) {
	luaL_loadstring(L, src.c_str());
	LClosure* cl = (LClosure*)(L->top - 1)->value.gc;
	return cl;
}

static std::string serialize(lua_State* L, const std::string& source)
{
	VectorStream vs;

	const LClosure* cl = compile(L, source);
	if (cl == nullptr)
	{
		vs.writeByte(0);
		vs.writeString(lua_tostring(L, -1));
		return vs.str();
	}

	vs.writeCompressedInt(1);

	/* Write string table */
	StringTable st(vs);
	std::unordered_map<TString*, int> stringTable = st.write(cl->p);

	/* Write proto table */
	ProtoTable pt(vs);
	std::unordered_map<Proto*, int> protoTable = pt.write(cl->p, stringTable);

	/* Entry point in proto table */
	vs.writeCompressedInt(protoTable[cl->p]);

	return vs.str();
}

VOID execute_script(int rl, lua_State* l, std::string source) {
	rl = RBX::r_lua_newthread(rl);
	if (luaL_loadstring(l, source.c_str())) {
		printf("Error: %s\n", lua_tostring(l, -1));
		lua_pop(l, 1);
	}

	else {
		auto bc = serialize(l, source.c_str());
		auto results = ((int(__cdecl*)(int, const char*, const char*, size_t))aslr(0x151c3e0))(rl, "", bc.c_str(),bc.size());
		printf("Successful: %d\n", results);
		RBX::spawn(rl);
		lua_pop(l, 1);
	}
}