#pragma once
#include <Windows.h>
#include "Serializer.h"

int* ScriptContext;

int DecryptPointer(int Pointer) {
	return *(int*)(Pointer)+(Pointer);
}

#define GetKey(rL)	DecryptPointer((int)DecryptPointer((int)(rL + 8)) + 28)

void Deserialize(int rL, const std::string& Code, const char* ChunkName, unsigned int ModKey) {
	typedef signed int(__cdecl* _Deserializer)(int, const std::string&, const char*, unsigned int);
	_Deserializer Deserializer = (_Deserializer)(0x584720 - 0x400000 + (DWORD)GetModuleHandle(NULL));
	Deserializer(rL, Code, ChunkName, ModKey);
}

void CompileBytecode(int rL, const char* Code) {
	lua_State* L = lua_open();
	LuaSerializer Serializer;
	RbxOpEncoder Encoder;

	std::string Bytecode = Serializer.serialize(L, std::string(Code), Encoder, GetKey(rL));
	Deserialize(rL, Bytecode, "Tervylla", 1); 
	lua_close(L);
}

void ExecuteScript(std::string Source) {
	int rL = ScriptContext[41] ^ ScriptContext[41];

	std::string Script;
	Script += "spawn(function() script=Instance.new(\"LocalScript\")\r\n";
	Script += Source;
	Script += "\r\nend)";

	*(DWORD *)(rL - 32) ^= (7 ^ (unsigned __int8)*(DWORD *)(rL - 32)) & 0x1F;
	CompileBytecode(rL, Script.c_str());
}