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

	static int scriptcontext = RebaseAddress(0x189F5C8);//updt

	static int lua_getfield = RebaseAddress(0x07F1A50);//updt
	static int lua_tonumber = RebaseAddress(0x07F40F0);//updt
	static int lua_pushnumber = RebaseAddress(0x07F2D30);//updt
	static int deserializer_flag = RebaseAddress(0x157C6B0);
	static int deserializer = RebaseAddress(0x57E930);
	static int lua_newthread = RebaseAddress(0x07F24F0);//updt
	static int spawn = RebaseAddress(0x7D96F0);
	static int lua_settop = RebaseAddress(0x07F3C80); //updt
	static int lua_pushstring = RebaseAddress(0x07F2DC0);//updt
	static int lua_setfield = RebaseAddress(0x07F37E0);//udpt
	static int InstanceNew = RebaseAddress(0x732D00);
	static int InitThread = RebaseAddress(0x7D91B0);//updt
	static int lua_pushcclosure = RebaseAddress(0x07F2950);//updt
	static int lua_tolstring = RebaseAddress(0x07F3E10);
	static int lua_pushvalue = RebaseAddress(0x735180);
	static int getmetatable = RebaseAddress(0x71D460);
	static int index2adr = RebaseAddress(0x719830);
	static int getcontent = RebaseAddress(0x586770);
	static int r_loadinstances = RebaseAddress(0x5EBDD0);
	static int r_pushobject = RebaseAddress(0x74A620);

	static int xorconst = RebaseAddress(0x1BC2EF0);//updt
	static int luaM_realloc_ = RebaseAddress(0x07ECDF0);//updt 0x007ECDF0
	static int luaS_newlstr = RebaseAddress(0x07ED470);//updt
	static int SandboxThread = RebaseAddress(0x7D9590);//updt
}

//"\x83\xEC\x00\x56\x8B\x75\x08\x57\x6A\x00\x6A\x08\x8B\xF9\x8B\xCE\x50", "xx?xxxxxxxxxxxxxx", -27

/* roblox lua structs */

#define RL_TOP     24
#define RL_BASE     12
#define RL_L_GT     96
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
typedef double(__fastcall *r_lua_tonumber_def)(int L, int idx);
typedef int(__cdecl* r_Deserializer_def)(int, std::string&, PCHAR, DWORD);
typedef int(__cdecl* r_lua_newthread_def)(int);
typedef void(__cdecl* r_spawn_def)(int);
typedef void(__cdecl* r_lua_settop_def)(int, int);
typedef void(__cdecl* r_lua_pushstring_def)(int, const char*);
typedef void(__fastcall* r_lua_setfield_def)(int, int, const char*);
typedef void(__cdecl* r_InstanceNew_def)(int);
typedef int(__cdecl* r_InitThread_def)(int);
typedef void(__stdcall *r_lua_pushcclosure_def)(DWORD luaState, int hi, DWORD n);
typedef void(__cdecl *r_lua_pushnumber_def)(int, double n);
typedef const char*(__stdcall *lua_tolstring_def)(DWORD lua_State, DWORD index, size_t *len);
typedef void(__cdecl *lua_pushvalue_def)(DWORD luaState, DWORD index);
typedef DWORD(__cdecl* r_getmetatable_def)(DWORD, DWORD);
typedef DWORD(__cdecl* r_index2adr_def)(DWORD, DWORD);
typedef int(__thiscall* rgetcontentstring_def)(int, int*, std::string*, int, int, int);
typedef void(__thiscall* rloadinstances_def)(int, std::istream*, std::vector<instance>*);
typedef int(__cdecl* rpushobject_def)(int, int, int);
typedef r_proto*(__cdecl* rluaF_newproto_def)(int);
typedef VOID*(__cdecl* rluaM_realloc__def)(int, int, int, int, int);
typedef int*(__stdcall* rluaS_newlstr_def)(int, const char*, int);
typedef rl_closure*(__cdecl* rluaF_newLclosure_def)(int, int, int);
typedef int(__cdecl* rluaF_newupval_def)(int);
typedef int(__cdecl* rSandboxThread_def)(int, int, int);

static DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/*  Calcualte the size of the function.

		In theory this will run until it hits the next
		functions prolog. It assumes all calls are aligned to
		16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/*  Check for the return check with the sig:
			72 ?? A1 ?? ?? ?? ?? 8B

			If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/*  Check if the current byte is a call, if it is,
					calculate the new relative call(s).

					*(->E8 + 1) = originalFunction - nextInstruction

					oFuncPos - Position of call in original function
						= originalFunction + (->E8 - newFunction)

					oFuncAddr - Original call location
						= oFuncPos + rel32Offset + sizeof(call)

					relativeAddr - New relative address
						= oFuncAddr - ->E8 - sizeof(call)

					Since we are not using a disassembler we assume
					that if we hit a E8 byte which is properly aligned
					it is a relative call.
					For a small amount of compensation I skip the location
					of the call, since it is possible to have the byte
					E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						/* Don't check rel32 */
						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, 0, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

static r_lua_pushnumber_def r_lua_pushnumber = (r_lua_pushnumber_def)Retcheck::unprotect((byte*)(addresses::lua_pushnumber));
static r_lua_tonumber_def r_lua_tonumber = (r_lua_tonumber_def)Retcheck::unprotect((byte*)(addresses::lua_tonumber));
static r_lua_getfield_def r_lua_getfield = (r_lua_getfield_def)Retcheck::unprotect((byte*)(addresses::lua_getfield));
static r_Deserializer_def r_deserialize = (r_Deserializer_def)addresses::deserializer;
static r_lua_newthread_def r_lua_newthread = (r_lua_newthread_def)unprotect(addresses::lua_newthread);
static r_spawn_def r_spawn = (r_spawn_def)addresses::spawn;
static r_lua_pushcclosure_def r_lua_pushcclosure = (r_lua_pushcclosure_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_pushvalue_def r_lua_pushvalue = (lua_pushvalue_def)Retcheck::unprotect((byte*)(addresses::lua_pushcclosure));
static lua_tolstring_def r_lua_tolstring = (lua_tolstring_def)Retcheck::unprotect((byte*)(addresses::lua_tolstring));

static r_lua_settop_def r_lua_settop = (r_lua_settop_def)unprotect(addresses::lua_settop);
static r_lua_pushstring_def r_lua_pushstring = (r_lua_pushstring_def)Retcheck::unprotect((byte*)(addresses::lua_pushstring));
static r_lua_setfield_def r_lua_setfield = (r_lua_setfield_def)Retcheck::unprotect((byte*)(addresses::lua_setfield));
static r_InstanceNew_def r_instance_new = (r_InstanceNew_def)addresses::InstanceNew;
static r_InitThread_def r_init_thread = (r_InitThread_def)unprotect(addresses::InitThread);
static r_getmetatable_def r_lua_getmetatable = (r_getmetatable_def)Retcheck::unprotect((byte*)(addresses::getmetatable));
static r_index2adr_def r_lua_index2adr = (r_index2adr_def)Retcheck::unprotect((byte*)(addresses::index2adr));
static rgetcontentstring_def r_getcontent = (rgetcontentstring_def)addresses::getcontent;
static rloadinstances_def r_load_instances = (rloadinstances_def)addresses::r_loadinstances;
static rpushobject_def r_push_object = (rpushobject_def)addresses::r_pushobject;


static rluaM_realloc__def rluaM_realloc_ = (rluaM_realloc__def)addresses::luaM_realloc_;
static rluaS_newlstr_def rluaS_newlstr = (rluaS_newlstr_def)addresses::luaS_newlstr;
static rSandboxThread_def r_sandbox_thread = (rSandboxThread_def)addresses::SandboxThread;

/* roblox lua macros */
#define RLUAM_MALLOC(rl, t) rluaM_realloc_(rl, 0, 0, (t), 3)
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
	//printf("callback\n");
	auto call_back = (DWORD)r_lua_tonumber(rL, rlua_upvalueindex(1));
	return ((int(*)(int))call_back)(rL);
}

static VOID rlua_pushlclosure(int rL, DWORD rlcl) {
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

static void rluaC_link(uintptr_t rL, int o, lu_byte tt)
{
	uintptr_t g = (rL + 20) + *(uint32_t*)(rL + 20); // thread->l_G
	lu_byte currentwhite = *(lu_byte*)(g + 20); // g->currentwhite
	//printf("cwhite\n");
	*(uint32_t*)o = *(uint32_t*)(g + 28); // o->gch.next
	*(uint32_t*)(g + 28) = o; // g->rootgc
	*(lu_byte*)(o + 4) = currentwhite & 3; // o->gch.marked
	*(lu_byte*)(o + 5) = tt; // o->gch.tt
}
static DWORD VMCheckAddy = RebaseAddress(0x1E95B44);
static DWORD rluaF_newproto(DWORD rL)
{
	int v6 = (int)rluaM_realloc_(rL, 0, 0, 80, 6);
	rluaC_link(rL, v6, RLUA_TPROTO);
	//printf("v6\n");
	*(DWORD *)(v6 + 12) = -(v6 + 12);
	*(DWORD *)(v6 + 28) = 0;
	*(DWORD *)(v6 + 48) = 0;
	*(DWORD *)(v6 + 32) = -(v6 + 32);
	*(DWORD *)(v6 + 72) = 0;
	*(DWORD *)(v6 + 52) = 0;
	*(DWORD *)(v6 + 40) = 0;
	*(DWORD *)(v6 + 76) = 0;
	*(DWORD *)(v6 + 36) = -(v6 + 36);
	*(DWORD *)(v6 + 24) = -(v6 + 24);
	*(DWORD *)(v6 + 44) = 0;
	*(DWORD *)(v6 + 8) = -(v6 + 8);
	*(DWORD *)(v6 + 64) = 0;
	*(DWORD *)(v6 + 60) = 0;
	*(DWORD *)(v6 + 16) = -(v6 + 16);
	*(BYTE *)(v6 + 79) = 0;
	*(BYTE *)(v6 + 78) = 0;
	*(BYTE *)(v6 + 76) = 0;
	*(BYTE*)(v6 + 77) = 0;
	*(DWORD*)(v6 + 56) = 0;
	return v6;
}

static int rluaF_newLclosure(unsigned int rL, signed int nelems, int e)
{
	int c = (int)rluaM_realloc_(rL, 0, 0, 4 * nelems + 20, 4);
	rluaC_link(rL, c, RLUA_TFUNCTION);
	*(int*)(c + 12) = e;
	*(byte*)(c + 7) = nelems;
	if (nelems)
	{
		int v304 = c + 4 * nelems + 20;
		do
		{
			*(int*)(v304 - 4) = 0;
			v304 -= 4;
			--nelems;
		} while (nelems);
	}
	return c;
}