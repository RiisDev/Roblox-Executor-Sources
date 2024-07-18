#pragma once

#include "RevBase.hpp"
#include "../Lua/lobject.h"
#include "..//Lua/lstate.h"
#define LUAVM_SHUFFLE2(sep,a0,a1) a1 sep a0
#define LUAVM_SHUFFLE3(sep,a0,a1,a2) a1 sep a2 sep a0
#define LUAVM_SHUFFLE4(sep,a0,a1,a2,a3) a3 sep a1 sep a0 sep a2
#define LUAVM_SHUFFLE5(sep,a0,a1,a2,a3,a4) a4 sep a0 sep a2 sep a1 sep a3
#define LUAVM_SHUFFLE6(sep,a0,a1,a2,a3,a4,a5) a3 sep a5 sep a2 sep a0 sep a1 sep a4
#define LUAVM_SHUFFLE7(sep,a0,a1,a2,a3,a4,a5,a6) a2 sep a3 sep a0 sep a4 sep a6 sep a1 sep a5
#define LUAVM_SHUFFLE8(sep,a0,a1,a2,a3,a4,a5,a6,a7) a7 sep a0 sep a5 sep a6 sep a3 sep a1 sep a2 sep a4
#define LUAVM_SHUFFLE9(sep,a0,a1,a2,a3,a4,a5,a6,a7,a8) a2 sep a6 sep a4 sep a7 sep a1 sep a8 sep a0 sep a3 sep a5

#define lint_deptr(x) *(int*)x + x
#define lint_enptr(x, y) *(int*)x = (int)y - x
#define rev_luaM_malloc(L,t) rev_luaM_realloc_(L, NULL, 0, (t))
#define rev_luaM_new(L,t) cast(t*, rev_luaM_malloc(L, sizeof(t)))

typedef TValue r_TValue;

template <typename T> class LuaVMValue
{
public:
	operator const T() const
	{
		return (T)((uintptr_t)storage + reinterpret_cast<uintptr_t>(this));
	}

	void operator=(const T& value)
	{
		storage = (T)((uintptr_t)value - reinterpret_cast<uintptr_t>(this));
	}

	const T operator->() const
	{
		return operator const T();
	}

private:
	T storage;
};

typedef struct r_LClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int *p;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_CClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int* f;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_TString {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	unsigned int len;			// +08	08
	unsigned int hash;			// +12  0C
};

typedef struct r_Udata {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	int* env;					// +08	08
	unsigned int len;			// +12	0C
	int* metatable;				// +16	10
	int unk;					// +20	14
};

typedef struct r_LocVar {
	r_TString *varname;
	int startpc;  /* first point where variable is active */
	int endpc;    /* first point where variable is dead */
};

typedef struct r_Proto {
	int* next;					// +00	00
	byte marked;				// +04	04
	byte tt;					// +05	05
	byte unk_0;					// +06	06
	byte unk_1;					// +07	07
	struct r_Proto** p;			// +08	08
	r_TString* source;			// +12	0C
	struct r_LocVar *locvars;	// +16	10
	int unk_3;					// +20	14
	int* code;					// +24	18
	int sizek;					// +28	1C
	r_TValue* k;				// +32	20
	int* lineinfo;				// +36	24 ???
	int unk_5;					// +40	28
	int sizep;					// +44	2C
	int sizelocvars;			// +48	30
	int sizecode;				// +52	34
	int unk_6;					// +56	38
	int unk_7;					// +60	3C
	int sizelineinfo;			// +64	40 ???
	int unk_9;					// +68	44
	byte maxstacksize;			// +72	48
	byte numparams;				// +73	49
	byte nups;					// +74	4A
	byte is_vararg;				// +75	4B
};



void rev_luaC_link(int r_lua_State, int o, byte tt);
void* rev_luaM_realloc_(int r_lua_State, int block, int osize, int nsize);
r_Proto* rev_luaF_newproto(int r_lua_State);
