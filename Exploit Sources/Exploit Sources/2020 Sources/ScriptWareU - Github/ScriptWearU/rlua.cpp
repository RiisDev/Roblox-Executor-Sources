#include "rlua.h"

__forceinline uint32_t ModInverse(uint32_t a)
{
	uint32_t x;
	x = 3 * a ^ 2; // 5 bits
	x *= 2 - a * x; // 10
	x *= 2 - a * x; // 20
	x *= 2 - a * x; // 40 -- 32 low bits
	return x;
}

void set_encode_key(int rL, DWORD* key) { // string "local", decompile for decode key
	char* v9 = (char *)GET_CKEY(rL);

	*key = ModInverse((DWORD)v9);
	// *key = Inverse(/*(DWORD)&v9[-*(int32_t*)v9]*/ (DWORD)v9, 4294967296);
}

void set_localscript(int rL) {
	r_lua_pushstring(rL, "LocalScript");
	r_instance_new(rL);
	r_tvalue localscript = *(*(r_tvalue**)(rL + RL_TOP) - 1);
	R_LUA_POP(rL, 2);
	**(r_tvalue**)(rL + RL_TOP) = localscript;
	R_API_INCR_TOP(rL);

	r_lua_setglobal(rL, "script");
}