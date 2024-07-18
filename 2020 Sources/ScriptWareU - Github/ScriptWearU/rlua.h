#pragma once
#include "retcheck.h"
#include "util.h"
#include "misc.h"
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

#include <bitset>

constexpr auto RLUA_TNIL = 0;
constexpr auto RLUA_TNONE = -1;
constexpr auto RLUA_TLIGHTUSERDATA = 3; //inside lua_touserdata's address (first if statement offset)
constexpr auto RLUA_TSTRING = 4; //tables cannot be cylinc (4th case) or lua_pushstring
constexpr auto RLUA_TNUMBER = 2; //tables cannot be cylinc or lua_pushnumber (Third case)
constexpr auto RLUA_TBOOLEAN = 1; //tables cannot be cylinc (2nd case v14 = sub_767530(a1, v8) != 0;)
constexpr auto RLUA_TTHREAD = 8; //lua_topointer (NOTE: CASES ARE SCRAMBLED, USE PROCESS OF ELIMINATION)
constexpr auto RLUA_TFUNCTION = 6; //tables cannot be cylinc (case with v69 = *(_DWORD *)(__readfsdword(0x2Cu) + 4 * TlsIndex);)
constexpr auto RLUA_TTABLE = 5; //tables cannot be cylinc or lua_createtable (The case with 'tables cannot be cyclic' string inside.)
constexpr auto RLUA_TUSERDATA = 7; //tables cannot be cylinc or lua_touserdata (second if statement offset)
constexpr auto RLUA_TPROTO = 9; //: bytecode version mismatch
constexpr auto RLUA_TUPVAL = 10;

static int script_context;
static int rL; //global state, initialized in the main function
static DWORD encode_key; //encode key, used in executescript function because its used as a parameter
static DWORD decode_key;
static int content_provider; //content provider (http://wiki.roblox.com/index.php?title=API:Class/ContentProvider) used to provide contents, referenced inside Script-Wear's internal getobjects function

#define LocalScriptGlobalState	172
#define CoreScriptGlobalState	220 // TODO: update?

#define rlua_upvalueindex(i) (-10002 - (i))

							 /* rebasing addresses for memory arrangement randomization (aslr) */
#define IdaBase 0x400000
static auto RobloxBase = (uintptr_t)GetModuleHandle(NULL);
#define RebaseAddress(x) (RobloxBase + x - IdaBase)

/* memory calls */
namespace addresses {
	/* todo: reduce this list/signature scan */

	static int lua_getfield = RebaseAddress(0x7cabd0); // updated
	static int lua_tonumber = RebaseAddress(0x7cd630); // updated
	static int lua_pushnumber = RebaseAddress(0x7cc0a0); // updated
	static int lua_newthread = RebaseAddress(0x7cb5e0); // updated
	static int lua_touserdata = RebaseAddress(0x7cd790); // updated
	static int lua_pcall = RebaseAddress(0x7cbaf0); // updated
	static int spawn = RebaseAddress(0x72a210); // updated
	static int lua_settop = RebaseAddress(0x7cd2c0); // updated
	static int lua_pushstring = RebaseAddress(0x7CC130);// updated
	static int lua_setfield = RebaseAddress(0x7CCD80); // updated
	static int InstanceNew = RebaseAddress(0x732D00);
	static int lua_pushcclosure = RebaseAddress(0x7CBC40);//updated
	static int lua_tolstring = RebaseAddress(0x7CD500);//updated
	static int lua_pushvalue = RebaseAddress(0x7CC280);// updated
	static int getmetatable = RebaseAddress(0x7CAF60);//updated
	static int index2adr = RebaseAddress(0x7C28F0);//updated

	static int luaD_call = RebaseAddress(0x7C3940);//updated
	static int luau_precall = RebaseAddress(0x8C8D60);//updated
	static int luaD_precall = RebaseAddress(0x7C3D60);//updated
	static int luaV_execute = RebaseAddress(0x7C7310);//updated
	static int lua_resume = RebaseAddress(0x7CC980);//updated

	static int getcontent = RebaseAddress(0x586770);//NOT REQUIRED (GETOBJECTS)
	static int r_loadinstances = RebaseAddress(0x5EBDD0);//NOT REQUIRED (GETOBJECTS)
	static int r_pushobject = RebaseAddress(0x74A620);//NOT REQUIRED (GETOBJECTS)

	static int xorconst = RebaseAddress(0x2078CC0);
	static int luaF_newproto = RebaseAddress(0x7C4A10);
	static int luaM_realloc_ = RebaseAddress(0x7C6100);
	static int luaS_newlstr = RebaseAddress(0x7c6730);
	static int luaF_newLclosure = RebaseAddress(0x7C4960);
	static int luaF_newupval = RebaseAddress(0x71B9D0);// :kekw:
	static int SandboxThread = RebaseAddress(0x729E90);
}

//"\x83\xEC\x00\x56\x8B\x75\x08\x57\x6A\x00\x6A\x08\x8B\xF9\x8B\xCE\x50", "xx?xxxxxxxxxxxxxx", -27

/* roblox lua structs */

#define RL_TOP     24
#define RL_BASE    12
#define RL_L_GT    80
#define RL_L_G     20
#define GET_CKEY(rL) (((rL) + 60 + *(DWORD *)((rL) + 20)) ^ *(DWORD *)((rL) + 60 + *(DWORD *)((rL) + 20)))
#define SET_CKEY(rL, v) (*(DWORD *)(*(DWORD *)((rL) + 20) + (rL) + 20 + 40) = (*(DWORD *)((rL) + 20) + (rL) + 20 + 40) ^ (v))

union r_value {
	PVOID gc;
	PVOID p;
	double n;
	int b;
};


struct r_tvalue {
	r_value value;
	int tt;
};

struct RLocVar
{
	TString* varname;
	int startpc;  /* first point where variable is active */
	int endpc;  /* first point where variable is dead */
	int unk; /* no idea why this even exists for */
};

struct __declspec(align(4)) r_proto
{
	GCObject* next;
	lu_byte marked;
	lu_byte tt;
	BYTE gap0[2];
	r_tvalue* k;
	RLocVar* locvars;
	r_proto* p;
	Instruction* code;
	int* lineinfo;
	int sizek;
	int sizeupvalues;
	int* source;
	int* upvalues;
	DWORD name;
	BYTE gap30[4];
	int sizelineinfo;
	BYTE gap38[4];
	int sizecode;
	int sizelocvars;
	BYTE gap44[8];
	int sizep;
	lu_byte nups;
	lu_byte numparams;
	lu_byte is_vararg;
	lu_byte maxstacksize;
};
/*
struct r_proto {
	CommonHeader;    // +00
	unsigned char Unk0;   // +07
	r_tvalue* k;     // +08
	int* source;    // +12
	int sizek;     // +16
	int* lineinfo;    // +20
	Instruction* code;   // +24
	int* locvars;    // +28
	r_proto* p;     // +32
	int sizeupvalues;   // +36
	int* upvalues;    // +40
	int linedefined;   // +44
	int sizelocvars;   // +48
	unsigned char Unk1[4];  // +52
	int sizep;     // +56
	int sizelineinfo;   // +60
	int sizecode;    // +64
	unsigned char Unk2[4];  // +68
	int lastlinedefined;  // +72
	byte numparams;    // +76
	byte is_vararg;    // +77
	byte nups;     // +78
	byte maxstacksize;   // +79
};
*/
struct __declspec(align(4)) rl_closure
{
	DWORD* next;
	BYTE marked;
	BYTE tt;
	BYTE isC;
	BYTE nups;
	BYTE maxstacksize;
	BYTE unk9[7];
	DWORD env;
	r_proto* p;
	DWORD unk18[4];
	DWORD upvals[1];
};



struct instance {
	int ptr;
	int ext;
};

typedef int(__cdecl* rlua_cclosure)(int L);

/* memory call info */
typedef void(__stdcall* r_lua_getfield_def)(int L, int idx, const char* k);
typedef double(__cdecl* r_lua_tonumber_def)(int L, int idx, DWORD*);
typedef int(__cdecl* r_lua_newthread_def)(int);
typedef int(__cdecl* r_spawn_def)(int);
typedef void(__fastcall* r_lua_settop_def)(int, int);
typedef void(__fastcall* r_lua_pushstring_def)(int, const char*);
typedef void(__fastcall* r_lua_setfield_def)(int, int, const char*);
typedef void(__cdecl* r_InstanceNew_def)(int); // wtf
typedef void(__fastcall* r_lua_pushcclosure_def)(DWORD, void*, int, int, int);
typedef void(__stdcall* r_lua_pushnumber_def)(int, double n);
typedef const char* (__fastcall* lua_tolstring_def)(DWORD lua_State, DWORD index, size_t* len);
typedef void(__fastcall* lua_pushvalue_def)(int luaState, int index);
typedef DWORD(__cdecl* r_getmetatable_def)(DWORD, DWORD);
typedef r_tvalue* (__cdecl* r_index2adr_def)(int, int);
typedef int(__thiscall* rgetcontentstring_def)(int, int*, std::string*, int, int, int);//wtf
typedef void(__thiscall* rloadinstances_def)(int, std::istream*, std::vector<instance>*);//wtf
typedef int(__cdecl* rpushobject_def)(int, int, int);//wtf
typedef r_proto* (__cdecl* rluaF_newproto_def)(int);
typedef VOID* (__cdecl* rluaM_realloc__def)(int, int, int, int, int);
typedef int* (__stdcall* rluaS_newlstr_def)(int, const char*, int);
typedef rl_closure* (__cdecl* rluaF_newLclosure_def)(int, int, int, int);
typedef int(__cdecl* rluaF_newupval_def)(int);
typedef void(__cdecl* rSandboxThread_def)(int, const std::bitset<16>&, const void*);
typedef int(__cdecl* rlua_touserdata_def)(int, int);
typedef int(__cdecl* rlua_pcall_def)(int, int, int, int);
typedef void(__cdecl* rluaD_call_def)(int, r_tvalue*, int);
typedef int(__cdecl* rluaD_precall_def)(int, r_tvalue*, int);
typedef void(__cdecl* rluaV_execute_def)(int, int);
typedef int(__cdecl* rlua_resume_def)(int, int);

static rlua_pcall_def r_lua_pcall = (rlua_pcall_def)Retcheck::unprotect((byte*)(addresses::lua_pcall));
static rluaD_call_def r_luaD_call = (rluaD_call_def)addresses::luaD_call;
static rluaD_precall_def r_luaD_precall = (rluaD_precall_def)Retcheck::unprotect((byte*)(addresses::luaD_precall));
static rluaV_execute_def r_luaV_execute = (rluaV_execute_def)addresses::luaV_execute;
static rlua_resume_def r_lua_resume = (rlua_resume_def)Retcheck::unprotect((byte*)(addresses::lua_resume));

static r_lua_pushnumber_def r_lua_pushnumber = (r_lua_pushnumber_def)Retcheck::unprotect((byte*)(addresses::lua_pushnumber));
static r_lua_tonumber_def r_lua_tonumber_old = (r_lua_tonumber_def)Retcheck::unprotect((byte*)(addresses::lua_tonumber));
__forceinline double r_lua_tonumber(int a1, int32_t a2)
{
	return r_lua_tonumber_old(a1, a2, nullptr);
}

static rlua_touserdata_def rlua_touserdata = (rlua_touserdata_def)Retcheck::unprotect((byte*)(addresses::lua_touserdata));

static r_lua_getfield_def r_lua_getfield = (r_lua_getfield_def)Retcheck::unprotect((byte*)(addresses::lua_getfield));
static r_lua_newthread_def r_lua_newthread = (r_lua_newthread_def)Retcheck::unprotect((byte*)(addresses::lua_newthread));
static r_spawn_def r_spawn = (r_spawn_def)addresses::spawn;
static r_lua_pushcclosure_def r_lua_pushcclosure = (r_lua_pushcclosure_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_pushvalue_def r_lua_pushvalue = (lua_pushvalue_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_tolstring_def r_lua_tolstring = (lua_tolstring_def)Retcheck::unprotect((byte*)(addresses::lua_tolstring));

static r_lua_settop_def r_lua_settop = (r_lua_settop_def)Retcheck::unprotect((byte*)(addresses::lua_settop));
static r_lua_pushstring_def r_lua_pushstring = (r_lua_pushstring_def)Retcheck::unprotect((byte*)(addresses::lua_pushstring));
static r_lua_setfield_def r_lua_setfield = (r_lua_setfield_def)Retcheck::unprotect((byte*)(addresses::lua_setfield));
static r_InstanceNew_def r_instance_new = (r_InstanceNew_def)addresses::InstanceNew;
static r_getmetatable_def r_lua_getmetatable = (r_getmetatable_def)Retcheck::unprotect((byte*)(addresses::getmetatable));
static r_index2adr_def r_lua_index2adr = (r_index2adr_def)Retcheck::unprotect((byte*)(addresses::index2adr));
static rgetcontentstring_def r_getcontent = (rgetcontentstring_def)addresses::getcontent;
static rloadinstances_def r_load_instances = (rloadinstances_def)addresses::r_loadinstances;
static rpushobject_def r_push_object = (rpushobject_def)addresses::r_pushobject;

static rluaF_newproto_def rluaF_newproto = (rluaF_newproto_def)addresses::luaF_newproto;
static rluaM_realloc__def rluaM_realloc_ = (rluaM_realloc__def)addresses::luaM_realloc_;
static rluaS_newlstr_def rluaS_newlstr = (rluaS_newlstr_def)addresses::luaS_newlstr;
static rluaF_newLclosure_def rluaF_newLclosure = (rluaF_newLclosure_def)addresses::luaF_newLclosure;
static rluaF_newupval_def rluaF_newupval = (rluaF_newupval_def)addresses::luaF_newupval;
static rSandboxThread_def r_sandbox_thread = (rSandboxThread_def)addresses::SandboxThread;

/* roblox lua macros */
#define RLUAM_MALLOC(rl, t) rluaM_realloc_(rl, 0, 0, (t), 4)
#define RLUAS_NEW(rl, str) rluaS_newlstr(rl, str, strlen(str))
#define R_API_INCR_TOP(rl) *(int*)(rl + RL_TOP) += sizeof(r_tvalue)
#define R_LUA_POP(rl, n) r_lua_settop(rl, -(n)-1)
#define R_CLEAR_STACK(rl) r_lua_settop(rl, 0)
#define RLUA_GETTOP(rl) ((*(int*)(rl + RL_TOP) - *(int*)(rl + RL_BASE)) >> 4)
#define r_lua_getglobal(rL, k)	r_lua_getfield(rL, -10002, k)
#define R_LUA_POP(rL, idx) r_lua_settop(rL, -(idx)-1)
#define r_lua_setglobal(rL, g) r_lua_setfield(rL, -10002, g)

/* random roblox util */
VOID set_encode_key(int rL, DWORD* key);
VOID set_localscript(int rL);

static int lua_jmpbackhandler(int rL)
{
	auto call_back = (DWORD)r_lua_tonumber(rL, rlua_upvalueindex(1));
	return ((int(*)(int))call_back)(rL);
}

static void rlua_pushlclosure(int rL, rl_closure* rlcl) {
	r_tvalue* top = *(r_tvalue**)(rL + RL_TOP);
	top->tt = RLUA_TFUNCTION;
	*(int*)(top) = (int)(rlcl);
	R_API_INCR_TOP(rL);
}

static void r_lua_pushboolean(int RL, bool boolean)
{
	//set top to the boolean value and ->tt to the boolean type
	int v4 = *(DWORD*)(RL + RL_TOP);
	*(DWORD*)v4 = boolean;
	*(DWORD*)(v4 + 8) = RLUA_TBOOLEAN;
	R_API_INCR_TOP(RL);
}

static double r_xor_number(double* Number) {
	int rxor = addresses::xorconst;

	PVOID OldNumber = Number;
	double NewNumber;
	__asm {
		mov edx, OldNumber
		mov eax, rxor
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd NewNumber, xmm1
	}
	return NewNumber;
}

/* custom global implementations */

static int custom_toclipboard(int rL)
{
	const char* str = r_lua_tolstring(rL, -1, 0);
	int len = strlen(str);
	HGLOBAL mem1 = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void* mem_ptr = GlobalLock(mem1);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(mem1);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, mem1);
	CloseClipboard();

	return 0;
}

static int custom_writefile(int rL) {
	const char* name = r_lua_tolstring(rL, -1, 0);
	const char* stuff = r_lua_tolstring(rL, -2, 0);

	char path[MAX_PATH];
	util::get_file(SW_DLL, "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "data\\savedscripts\\";
	file += stuff;

	std::ofstream createfile(file.c_str());

	createfile << name;

	return 0;
}

static int custom_readfile(int rbx_L) {
	const char* name = r_lua_tolstring(rbx_L, -1, 0);

	char path[MAX_PATH];
	util::get_file(SW_DLL, "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "data\\savedscripts\\";
	file += name;

	std::string line;
	std::ifstream swfile(file);

	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			r_lua_pushstring(rbx_L, line.c_str());
		}
		swfile.close();
	}

	return 0;
}

static int custom_getgenv(int RL) {
	//	r_TValue1* top = rbx_gettop(RL);

	//r_setobj(top, (r_TValue1*)(RL + 72));
	//r_api_incr_top(RL);

	return 1;
}

static int custom_getrawmetatable(int rL) {
	r_lua_getmetatable(rL, -1);

	DWORD read_only = (DWORD)r_lua_index2adr(rL, -1);
	*(byte*)(*(DWORD*)read_only + 8) = 0;
	return 1;
}