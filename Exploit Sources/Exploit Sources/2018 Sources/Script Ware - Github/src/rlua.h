#pragma once
#include "retcheck.h"
#include "util.h"
#include "misc.h"
#include <fstream>
#include <sstream>
#include "VMProtectSDK.h"

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

static int script_context;
static int rL; //global state, initialized in the main function
static DWORD encode_key; //encode key, used in executescript function because its used as a parameter
static DWORD* decode_key;
static int content_provider; //content provider (http://wiki.roblox.com/index.php?title=API:Class/ContentProvider) used to provide contents, referenced inside script-ware's internal getobjects function

#define LocalScriptGlobalState	164
#define CoreScriptGlobalState	220

#define rlua_upvalueindex(i) (-10002 - (i))

							 /* rebasing addresses for memory arrangement randomization (aslr) */
#define IdaBase 0x400000
#define RobloxBase (int)GetModuleHandle(NULL)
#define RebaseAddress(x) (RobloxBase + x - IdaBase)

							 /* memory calls */
namespace addresses {
	/* todo: reduce this list/signature scan */

	static int scriptcontext = RebaseAddress(0x1008714);

	static int lua_getfield = RebaseAddress(0x71D170);
	static int lua_tonumber = RebaseAddress(0x71F450);
	static int lua_pushnumber = RebaseAddress(0x71E410);
	static int deserializer_flag = RebaseAddress(0x157C6B0);
	static int deserializer = RebaseAddress(0x57E930);
	static int lua_newthread = RebaseAddress(0x722460);
	static int spawn = RebaseAddress(0x6AC7B0);
	static int lua_settop = RebaseAddress(0x723AD0);
	static int lua_pushstring = RebaseAddress(0x722D50);
	static int lua_setfield = RebaseAddress(0x7236F0);
	static int InstanceNew = RebaseAddress(0x732D00);
	static int InitThread = RebaseAddress(0x6AD3E0);
	static int lua_pushcclosure = RebaseAddress(0x71E0A0);
	static int lua_tolstring = RebaseAddress(0x71F4E0);
	static int lua_pushvalue = RebaseAddress(0x735180);
	static int getmetatable = RebaseAddress(0x71D460);
	static int index2adr = RebaseAddress(0x719830);
	static int getcontent = RebaseAddress(0x586770);
	static int r_loadinstances = RebaseAddress(0x5EBDD0);
	static int r_pushobject = RebaseAddress(0x74A620);

	static int xorconst = RebaseAddress(0x14A26F0);
	static int luaF_newproto = RebaseAddress(0x71B910);
	static int luaM_realloc_ = RebaseAddress(0x71D3C0);
	static int luaS_newlstr = RebaseAddress(0x71DA50);
	static int luaF_newLclosure = RebaseAddress(0x71B8A0);
	static int luaF_newupval = RebaseAddress(0x71B9D0);
	static int SandboxThread = RebaseAddress(0x6AC5F0);
}

//"\x83\xEC\x00\x56\x8B\x75\x08\x57\x6A\x00\x6A\x08\x8B\xF9\x8B\xCE\x50", "xx?xxxxxxxxxxxxxx", -27

/* roblox lua structs */

#define RL_TOP     32
#define RL_BASE     20
#define RL_L_GT     104
#define RL_L_G     12
#define RL_L_G_CKEY    56

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

struct rl_closure {
	unsigned char Unk[15];		// +00
	r_proto* p;					// +16
	int upvals[1];				// +20
};

struct instance {
	int ptr;
	int ext;
};

static void r_lua_pushboolean(int RL, bool boolean)
{
	//set top to the boolean value and ->tt to the boolean type
	int v4 = *(DWORD*)(RL + RL_TOP);
	*(DWORD*)v4 = boolean;
	*(DWORD*)(v4 + 8) = 3;
	*(DWORD*)(RL + RL_TOP) += RL_TOP;
}

typedef int(__cdecl *rlua_cclosure)(int L);

/* memory call info */
typedef void(__cdecl *r_lua_getfield_def)(int L, int idx, const char *k);
typedef double(__cdecl *r_lua_tonumber_def)(int L, int idx);
typedef int(__cdecl* r_Deserializer_def)(int, std::string&, PCHAR, DWORD);
typedef int(__cdecl* r_lua_newthread_def)(int);
typedef void(__cdecl* r_spawn_def)(int);
typedef void(__cdecl* r_lua_settop_def)(int, int);
typedef void(__cdecl* r_lua_pushstring_def)(int, const char*);
typedef void(__cdecl* r_lua_setfield_def)(int, int, const char*);
typedef void(__cdecl* r_InstanceNew_def)(int);
typedef int(__cdecl* r_InitThread_def)(int, int, int);
typedef void(__cdecl *r_lua_pushcclosure_def)(DWORD luaState, LPVOID hi, DWORD n);
typedef void(__cdecl *r_lua_pushnumber_def)(int, double n);
typedef const char*(__cdecl *lua_tolstring_def)(DWORD lua_State, DWORD index, size_t *len);
typedef void(__cdecl *lua_pushvalue_def)(DWORD luaState, DWORD index);
typedef DWORD(__cdecl* r_getmetatable_def)(DWORD, DWORD);
typedef DWORD(__cdecl* r_index2adr_def)(DWORD, DWORD);
typedef int(__thiscall* rgetcontentstring_def)(int, int*, std::string*, int, int, int);
typedef void(__thiscall* rloadinstances_def)(int, std::istream*, std::vector<instance>*);
typedef int(__cdecl* rpushobject_def)(int, int, int);
typedef r_proto*(__cdecl* rluaF_newproto_def)(int);
typedef VOID*(__cdecl* rluaM_realloc__def)(int, int, int, int, int);
typedef int*(__cdecl* rluaS_newlstr_def)(int, const char*, int);
typedef rl_closure*(__cdecl* rluaF_newLclosure_def)(int, int, int);
typedef int(__cdecl* rluaF_newupval_def)(int);
typedef int(__cdecl* rSandboxThread_def)(int, int, int);

static r_lua_pushnumber_def r_lua_pushnumber = (r_lua_pushnumber_def)Retcheck::unprotect((byte*)(addresses::lua_pushnumber));
static r_lua_tonumber_def r_lua_tonumber = (r_lua_tonumber_def)Retcheck::unprotect((byte*)(addresses::lua_tonumber));
static r_lua_getfield_def r_lua_getfield = (r_lua_getfield_def)Retcheck::unprotect((byte*)(addresses::lua_getfield));
static r_Deserializer_def r_deserialize = (r_Deserializer_def)addresses::deserializer;
static r_lua_newthread_def r_lua_newthread = (r_lua_newthread_def)Retcheck::unprotect((byte*)(addresses::lua_newthread));
static r_spawn_def r_spawn = (r_spawn_def)addresses::spawn;
static r_lua_pushcclosure_def r_lua_pushcclosure = (r_lua_pushcclosure_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_pushvalue_def r_lua_pushvalue = (lua_pushvalue_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_tolstring_def r_lua_tolstring = (lua_tolstring_def)Retcheck::unprotect((byte*)(addresses::lua_tolstring));

static r_lua_settop_def r_lua_settop = (r_lua_settop_def)Retcheck::unprotect((byte*)(addresses::lua_settop));
static r_lua_pushstring_def r_lua_pushstring = (r_lua_pushstring_def)Retcheck::unprotect((byte*)(addresses::lua_pushstring));
static r_lua_setfield_def r_lua_setfield = (r_lua_setfield_def)Retcheck::unprotect((byte*)(addresses::lua_setfield));
static r_InstanceNew_def r_instance_new = (r_InstanceNew_def)addresses::InstanceNew;
static r_InitThread_def r_init_thread = (r_InitThread_def)addresses::InitThread;
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
#define RLUA_GETTOP(rl) (*(int*)(rl + RL_TOP) - *(int*)(rl + RL_BASE)) >> 4
#define r_lua_getglobal(rL, k)	r_lua_getfield(rL, -10002, k)
#define R_LUA_POP(rL, idx) r_lua_settop(rL, -(idx)-1)
#define r_lua_setglobal(rL, g) r_lua_setfield(rL, -10002, g)

#define RLUA_TNONE    -1
#define RLUA_TNIL    0
#define RLUA_TLIGHTUSERDATA  1
#define RLUA_TNUMBER   2
#define RLUA_TBOOLEAN   3
#define RLUA_TSTRING   4
#define RLUA_TTHREAD   5
#define RLUA_TFUNCTION   6
#define RLUA_TTABLE    7
#define RLUA_TUSERDATA   8
#define RLUA_TPROTO    9
#define RLUA_TUPVAL    10

/* random roblox util */
VOID set_encode_key(int rL, DWORD* key);
VOID set_localscript(int rL);

/* roblox instances use a special instance struct, it contains 2 members, one being the instance and 2 being padding (obfuscation), skip 4 bytes (padding) to get instance directly */
static int rlua_touserdata(int rL, int idx) {
	int result;
	int* tv = (int*)r_lua_index2adr(rL, idx);
	if (tv[2] == 1) {
		result = *tv;
	}
	else if (tv[2] == 8) {
		result = *tv + 24;
	}
	else {
		result = 0;
	}
	return *(int*)(result + 4);
}

static void r_lua_pushnil(int rL) {
	*(DWORD *)(*(DWORD *)(rL + 16) + 8) = 0;
	*(DWORD *)(rL + 16) += 16;
}

static int lua_jmpbackhandler(int rL)
{
	auto call_back = (DWORD)r_lua_tonumber(rL, rlua_upvalueindex(1));
	return ((int(*)(int))call_back)(rL);
}

static VOID rlua_pushlclosure(int rL, rl_closure* rlcl) {
	r_tvalue* top = *(r_tvalue**)(rL + RL_TOP);
	top->tt = RLUA_TFUNCTION;
	*(int*)(top) = (int)(rlcl);
	R_API_INCR_TOP(rL);
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
	void *mem_ptr = GlobalLock(mem1);
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
	const char * name = r_lua_tolstring(rbx_L, -1, 0);

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

static r_tvalue* rbx_gettop(int rL) {
	return *(r_tvalue**)(rL + 24);
}

static int custom_getgenv(int RL) {
	//	r_TValue1* top = rbx_gettop(RL);

	//r_setobj(top, (r_TValue1*)(RL + 72));
	//r_api_incr_top(RL);

	return 1;
}

static int custom_getrawmetatable(int rL) {
	r_lua_getmetatable(rL, -1);

	DWORD read_only = r_lua_index2adr(rL, -1);
	*(byte*)(*(DWORD*)read_only + 8) = 0;
	return 1;
}