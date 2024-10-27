#pragma once
#include <cstdint>
#include <queue> 
#include "Bypasses\\RetCheck.hpp"
#include "Globals.hpp"

namespace Fluxus {
	void Init();
	DWORD ReturnDataModel();
	DWORD SetState(DWORD);
	DWORD FindChildOfInstance(DWORD, const char*);
	int ASLR(int);
	DWORD ReturnCallCheck();
	
	namespace Offset {
		const auto Readonly = 6;
		const auto Namecall = 100;
		const auto ModuleLocked = 224;
		const auto NetworkedDDMS = 1152;
		const auto DataModelInstance = 12;
		const auto EmptySpace = 108;
		const auto TOP = 24;
		const auto RootGC = 28;
		const auto GCTT = 4;
		const auto K = 12;
		const auto SizeK = 76;
		const auto P = 20;
		const auto SizeP = 68;

		const auto TrustCheck = ASLR(0x23A304C);  // https://cdn.discordapp.com/attachments/743872274259771504/747181404873556009/unknown.png
	}

	class RBX {
		DWORD State;
	public:
		void IncrTop(std::uintptr_t State);
		int Deserialize(const char* v2, const char* v3, size_t v4);
		void Spawn();
		void Settop(int);
		void Pop(int);
		int NewThread();
		void Pushlstring(const char*, size_t);
		void Pushstring(const char*);
		void Getfield(int, const char*);
		void Setfield(int, const char*);
		void GetGlobal(const char*);
		void SetGlobal(const char*);
		bool ToBoolean(int);
		char* Tolstring(int, size_t*);
		void PushValue(int);
		void ECall(int, int);
		int PCall(int, int, int);
		double ToNumber(int, int);
		int Type(int);
		const char* TypeName(int);
		int NewMetaTable(const char*);
		void RawPushCClosure(int, int, int, int);
		void PushCClosure(int a1(DWORD),int a2);
		void PushCFunction(int a1(DWORD));
		void PushNumber(double);
		void CreateTable(int, int);
		void Settable(int);
		void PushRawObject(uintptr_t, TValue*);
		int ToUserData(int);
		void PushBoolean(bool);
		int GetMetaTable(int);
		bool SetMetaTable(int);
		void XMove(DWORD, DWORD);
		void Error(const char*);
		std::uintptr_t ToPointer(int);
		void Setreadonly(int, bool);
		void CheckArg(DWORD,DWORD);
		void CheckArg(DWORD);
		void SandboxThread();
		void Replace(int);
		void Insert(int);
		int RYield(int);
		int Gettop();
		void Remove(DWORD);
		int Next(DWORD);
		bool IsCFunction(DWORD);
		std::uintptr_t NewLClosure(int, int, std::uintptr_t);
		int GetInfo(const char*, rlua_Debug*);
		TValue* Index2Adr(DWORD);
		void Settabss(const char* i, const char* v);
		void Settabsi(const char* i, int v);
		void Settabsb(const char* i, bool v);
		void Treatstackoption(const char* fname);
		int Idkwtf(const char*,int,size_t);
		void FireClickDetector(DWORD, float, DWORD);
		int GetCleanState();
		int SetCleanState(DWORD);
		const char* CFunctionName(DWORD,bool);
		DWORD GetGlobalState();
		DWORD GetProto(DWORD);
		DWORD ProtoDeobf(DWORD,DWORD);
		RBX(DWORD);
	};
}


const auto R_LUA_TNIL = 0;
const auto R_LUA_TLIGHTUSERDATA = 4;
const auto R_LUA_TNUMBER = 3;
const auto R_LUA_TBOOLEAN = 1;
const auto R_LUA_TSTRING = 5;
const auto R_LUA_TTHREAD = 8;
const auto R_LUA_TFUNCTION = 7;
const auto R_LUA_TTABLE = 6;
const auto R_LUA_TUSERDATA = 9;
const auto R_LUA_TPROTO = 0xA;
const auto R_LUA_TUPVALUE = 0xB;

