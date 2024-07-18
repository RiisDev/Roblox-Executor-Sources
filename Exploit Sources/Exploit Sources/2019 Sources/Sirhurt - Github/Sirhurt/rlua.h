/*rbx*/ #define int3breakpoint 0x11CDC4C
/*rbx*/ #define Format(address) (address - 0x00400000 + reinterpret_cast<uintptr_t>(GetModuleHandle(0)))
/*rbx*/ #define algorithm (ScriptContext + 164) + *(uint32_t*)(ScriptContext + 164)
/*rbx*/ #define globalalgorithm (ScriptContext + 220) - *(uint32_t*)(ScriptContext + 220)

//164 reg globalstate 220 corescript state

#define Unformat(a)(a + 0x400000 - (DWORD)GetModuleHandleA(0))
#define format(x)(x - 0x400000 + (DWORD)GetModuleHandle(NULL))

#define DEBUG

#if true
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(Retcheck::unprotect(Format(address)))
#else
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(address)
#endif 

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8
#define RLUA_TPROTO 9

#define rlua_pop(rL, n) rlua_settop(rL, -(n) - 1)

//auto rlua_settop = Declare(0x77A0E0, void, __cdecl, uintptr_t rL, int idx); //update
//auto rlua_gettop = Declare(0x778400, int, __cdecl, uintptr_t rL); //update
//auto rlua_getmetatable = Declare(0x7781D0, int, __stdcall, uintptr_t rL, int idx); //update
//auto r_link = Declare(0x77C4E0, void, __thiscall, uintptr_t rL, uintptr_t m_rL); //update

#define rluaS_new(L, s) (rluaS_newlstr(L, s, strlen(s)))
#define getstr(ts)      cast(const char *, (ts) + 1)
#define r_lua_tostring(rL,i)    r_lua_tolstring(rL, (i), NULL)

DWORD VMCheckAddy = format(0x1BE4E44); //the flag thats called in C Stack Overflow (83 EC 08 56 8B 75 08 C7 45 ? ? ? ? ? 8B 4D FC) //17/01 1C1080C
int rxor = Format(0x1AB5B30); //33 35 ? ? ? ? F7 C6 //14/01 5209A5 // 17/01 004B39E5
int breakpoint = Format(0x401016);

auto rlua_pcall = Declare(0x780010, int, __cdecl, uintptr_t rL, int nargs, int nresults, int errfunc);//14 0077DFA0
auto rlua_newthread = Declare(0x77FCE0, uintptr_t, __cdecl, uintptr_t rL); //14 0077DC70

auto Spawn = Declare(0x736A10, int, __cdecl, uintptr_t rL); //spawn requires 1 argument (function name) //14 765130

															//realloc aob: FF 75 18 8B 38
auto rluaM_realloc_ = Declare(0x77A5F0, void*, __cdecl, uintptr_t rL, void *block, size_t osize, size_t nsize, unsigned int modkey); //14 00778560
auto rluaS_newlstr = Declare(0x77AC70, void*, __stdcall, uintptr_t rL, const char* str, size_t l); //setfield (noise) sub_7740E0(v3, (int)a3, strlen(a3)); //14 778BE0
auto sandboxThread = Declare(0x7364D0, int, __cdecl, uintptr_t rL); //The metatable is locked, func name is addy. //17/01 sub_7642F0

																	//for pushing globals.
auto r_lua_tonumber = Declare(0x781910, double, __fastcall, int a1, signed int a2);
auto r_lua_pushcclosure = Declare(0x780160, void, __fastcall, DWORD a1, int a2, int a3);
auto r_lua_pushstring = Declare(0x7805F0, int, __stdcall, DWORD a1, const char* a2);
auto r_lua_pushnumber = Declare(0x780560, void, __thiscall, DWORD a1, double a2);
auto r_lua_setfield = Declare(0x781000, void*, __stdcall, DWORD a1, int a2, const char* a3);
auto r_lua_tolstring = Declare(0x781620, char*, __stdcall, DWORD a1, int a2, std::size_t* a3);