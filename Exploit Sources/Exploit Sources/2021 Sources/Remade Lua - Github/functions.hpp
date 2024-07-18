#include "headers.hpp"
#include <Windows.h>
#include <exception>
#include <stdio.h>

__forceinline r_TValue* r_index2adr(std::uintptr_t rL, std::int32_t idx)
{
	if (idx > 0)
	{
		r_TValue* o = *reinterpret_cast<r_TValue**>(rL + offsets::base) + (idx - 1);

		if (o >= *reinterpret_cast<r_TValue**>(rL + offsets::top))
			return cast(r_TValue*, r_luaO_nilobject);
		else
			return o;
	}
	else
	{
		return addresses::r_lua_index2adr_slow(rL, idx);
	}
}

/* stack -> push */
void r_lua_pushnil(std::uintptr_t rL)
{
	r_setnilvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top));
	r_incr_top(rL);
}

void r_lua_pushthread(std::uintptr_t rL)
{
	r_setthvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), rL);
	r_incr_top(rL);
}

void r_lua_pushboolean(std::uintptr_t rL, bool b)
{
	r_setbvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), b);
	r_incr_top(rL);
}

void r_lua_pushnumber(std::uintptr_t rL, std::double_t num)
{
	r_setnvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), num);
	r_incr_top(rL);
}

void r_lua_pushvalue(std::uintptr_t rL, std::uint32_t idx)
{
	r_setobj2s(*reinterpret_cast<r_TValue**>(rL + offsets::top), r_index2adr(rL, idx));
	r_incr_top(rL);
}

void r_lua_pushinteger(std::uintptr_t rL, std::uint32_t n)
{
	r_setnvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), cast_num(n));
	r_incr_top(rL);
}

void r_lua_pushlightuserdata(std::uintptr_t rL, void* p)
{
	r_setpvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), p);
	r_incr_top(rL);
}

/* state -> to */
std::uint32_t* r_lua_tothread(std::uintptr_t rL, std::uint32_t idx)
{
	r_StkId o = r_index2adr(rL, idx);
	if (o->tt == R_LUA_TTHREAD)
		return &(o)->value.gc->th;
	return NULL;
}

std::double_t r_lua_tonumber(std::uintptr_t rL, std::int32_t idx)
{
	r_TValue* top = r_index2adr(rL, idx);

	std::double_t raw_value = *reinterpret_cast<std::double_t*>(&reinterpret_cast<r_TValue*>(top)->value);
	return r_xor_number(raw_value);
}

std::uint32_t r_lua_tointeger(std::uintptr_t rL, std::uint32_t idx)
{
	r_TValue* top = r_index2adr(rL, idx);

	const std::double_t raw_value = *reinterpret_cast<std::double_t*>(&(reinterpret_cast<r_TValue*>(top))->value);
	const auto result = static_cast<std::int32_t>(floor(r_xor_number(raw_value)));
	return result;
}

bool r_lua_toboolean(std::uintptr_t rL, std::uint32_t idx)
{
	r_TValue* top = r_index2adr(rL, idx);
	return reinterpret_cast<r_TValue*>(top)->value.b;
}

const char* r_lua_tolstring(std::uintptr_t rL, std::uint32_t idx, std::size_t* len)
{
	r_StkId o = r_index2adr(rL, idx);
	if (!r_ttisstring(o))
	{
		if (!addresses::r_luaV_tostring(rL, o))
		{ /* conversion failed? */
			if (len != NULL)
				*len = 0;
			return NULL;
		}
		o = r_index2adr(rL, idx); /* previous call may reallocate the stack */
	}
	if (len != NULL)
		*len = obfuscations::deobf_tstring_len(reinterpret_cast<std::uintptr_t>(o->value.gc));
	return r_getstr(o->value.gc);
}

/* luaL */
r_lua_Number r_luaL_checknumber(std::uintptr_t rL, std::uint32_t narg) {
	r_lua_Number d = r_lua_tonumber(rL, narg);
	if (d == 0)  /* avoid extra test when d is not 0 */
		printf("bruh moment in checknumber\n");
	return d;
}

std::uint32_t r_lua_type(std::uintptr_t rL, std::uint32_t idx)
{
	r_TValue* obj = r_index2adr(rL, idx);
	return obj == r_luaO_nilobject ? R_LUA_TNONE : obj->tt;
}

void r_luaL_checktype(std::uintptr_t rL, std::uint32_t narg, std::uint32_t t)
{
	if (r_lua_type(rL, narg) != t)
		printf("Invalid type.\n");
	// r_luaL_error(rL, "Invalid type.");
}

/* state -> other */
void r_lua_xmove(std::uintptr_t from, std::uintptr_t to, std::uint32_t n)
{
	if (from == to) return;
	*reinterpret_cast<std::uintptr_t*>(from + offsets::top) -= (n * sizeof(r_TValue));

	for (auto i = 0; i < n; i++)
	{
		r_setobj2s(*reinterpret_cast<r_TValue**>(to + offsets::top), *reinterpret_cast<r_TValue**>(from + offsets::top) + i);
		r_incr_top(to);
	}
	return;
}

void r_lua_replace(std::uintptr_t rL, std::uint32_t idx)
{
	auto o = r_index2adr(rL, idx);

	if (idx == R_LUA_ENVIRONINDEX)
	{
		//	r_luaL_error(rL, "no calling environment");
		printf("no calling environment\n"); // use r_luaL_error if you want
	}

	r_setobj2s(reinterpret_cast<r_TValue*>(o), reinterpret_cast<r_TValue*>(r_index2adr(rL, -1)));
	r_decr_top(rL);
}

void r_lua_remove(std::uintptr_t rL, std::uint32_t idx)
{
	auto p = r_index2adr(rL, idx);

	while (p < *reinterpret_cast<r_TValue**>(rL + offsets::top))
	{
		r_setobj2s(reinterpret_cast<r_TValue*>(p), reinterpret_cast<r_TValue*>(p + sizeof(r_TValue)));
		p += sizeof(r_TValue);
	}

	r_decr_top(rL);
}


void r_lua_insert(std::uintptr_t rL, std::uint32_t idx)
{
	auto p = r_index2adr(rL, idx);
	for (auto q = *reinterpret_cast<r_TValue**>(rL + offsets::top); q > p; q -= sizeof(r_TValue))
	{
		r_setobj2s(reinterpret_cast<r_TValue*>(q), reinterpret_cast<r_TValue*>(q - sizeof(r_TValue)));
	}

	r_setobj2s(reinterpret_cast<r_TValue*>(p), *reinterpret_cast<r_TValue**>(rL + offsets::top));
}

std::uint32_t r_lua_gettop(std::uintptr_t rL)
{
	return cast_int(*reinterpret_cast<r_TValue**>(rL + offsets::top) - *reinterpret_cast<r_TValue**>(rL + offsets::base));
}

void r_lua_settop(std::uintptr_t rL, std::uint32_t idx)
{
	r_TValue** top = reinterpret_cast<r_TValue**>(rL + offsets::top);
	r_TValue** base = reinterpret_cast<r_TValue**>(rL + offsets::base);
	if (idx >= 0)
	{
		while (*top < *base + idx)
		{
			r_setnilvalue(*top);
			*top++;
		}
		*top = *base + idx;
	}
	else
		*top += idx + 1;
}

void r_lua_pushraw(std::uintptr_t rL, std::uintptr_t obj, int tt)
{
	r_setrawobj2s(rL, obj, tt);
	r_incr_top(rL);
}

std::uint32_t r_lua_getmetatable(std::uintptr_t rL, std::uint32_t idx)
{
	const r_TValue* obj;
	auto mt = 0;
	std::uintptr_t res;
	obj = r_index2adr(rL, idx);

	switch (r_ttype(obj))
	{
	case R_LUA_TTABLE:
	{
		std::uintptr_t v5 = *(DWORD*)obj + 24;
		mt = *(_DWORD*)v5 + 12;
		break;
	}
	case R_LUA_TUSERDATA:
	{
		std::uintptr_t v6 = *(DWORD*)obj + 12;
		mt = v6 ^ *(DWORD*)v6;
		break;
	}
	default:
	{
		mt = *(DWORD*)(4 * *(DWORD*)(obj + 12) + 1304 - (rL + 20) + *(DWORD*)(rL + 20));
		break;
	}
	}
	if (mt == NULL)
		res = 0;
	else
	{
		r_lua_pushraw(rL, mt, R_LUA_TTABLE);
		res = 1;
	}
	return res;
}

std::uint32_t r_lua_setmetatable(std::uintptr_t rL, std::int32_t objindex) // didnt check if this actually works, will remove this comment when i confirm it does
{
	const r_TValue* obj;
	auto mt = 0;
	obj = r_index2adr(rL, objindex);

	switch (r_ttype(obj))
	{
	case R_LUA_TTABLE:
	{
		std::uintptr_t v5 = *(DWORD*)obj + 24;
		*(_DWORD*)v5 + 12 == mt;
		break;
	}
	case R_LUA_TUSERDATA:
	{
		std::uintptr_t v6 = *(DWORD*)obj + 12;
		v6^* (_DWORD*)v6 == mt;
		break;
	}
	default:
	{
		*(DWORD*)(4 * *(DWORD*)(obj + 12) + 1304 - (rL + 20) + *(DWORD*)(rL + 20)) = mt;
		break;
	}
	}
	r_decr_top(rL);
	return 1;
}

std::uint32_t r_lua_yield(std::uintptr_t rL, std::uint32_t nresults) {

	if (*reinterpret_cast<std::uintptr_t*>(rL + offsets::l_nccalls) > *reinterpret_cast<std::uintptr_t*>(rL + offsets::l_baseccalls))
		printf("attempt to yield across metamethod/C-call boundary\n");
	// r_luaL_error(rL, "attempt to yield across metamethod/C-call boundary");

	*reinterpret_cast<std::uintptr_t*>(rL + offsets::base) = *reinterpret_cast<std::uintptr_t*>(rL + offsets::top) - 16 * nresults;
	*reinterpret_cast<std::uintptr_t*>(rL + offsets::l_status) = R_LUA_YIELD;
	return -1;
}

void r_luaC_link(std::uintptr_t rL, std::uint32_t o, r_lu_byte tt)
{
	const auto g = r_G(rL); // getglobalstate
	*reinterpret_cast<std::uint32_t*>(o) = *reinterpret_cast<std::uint32_t*>(g + offsets::g_next);
	*reinterpret_cast<std::uint32_t*>(g + offsets::g_rootgc) = o;
	*reinterpret_cast<r_lu_byte*>(o + offsets::g_marked) = *reinterpret_cast<r_lu_byte*>(g + offsets::g_currentwhite) & 3;
	*reinterpret_cast<r_lu_byte*>(o + offsets::g_ttype) = tt;
}

void* r_luaM_realloc_(std::uintptr_t rL, std::size_t, osize, std::size_t nsize, std::uint8_t memcat)
{
	auto GL = r_G(rL);
	void* result;

	result = (std::uintptr_t*)(*(std::uint32_t(__cdecl**)(std::uint32_t, const std::uintptr_t, const std::uintptr_t, const std::uintptr_t, std::uint32_t))(GL + offsets::g_frealloc))(rL, *reinterpret_cast<const std::uintptr_t*>(GL + 16), 0, 0, nsize);

	*reinterpret_cast<std::size_t*>(GL + offsets::g_totalbytes) = (*reinterpret_cast<size_t*>(GL + offsets::g_totalbytes) - osize) + nsize;
	*reinterpret_cast<std::uintptr_t*>(GL + 4 * memcat + 200) += nsize - osize;

	return result;
}

std::uintptr_t r_luaF_newCclosure(const std::uintptr_t rL, const std::uint32_t nelems, const std::uintptr_t e)
{
	const auto c = reinterpret_cast<std::int32_t>(r_luaM_realloc_(rL, 40, *reinterpret_cast<BYTE*>(rL + offsets::l_activememcat))); // meme
	r_luaC_link(rL, c, R_LUA_TFUNCTION);
	*reinterpret_cast<std::uint8_t*>(c + closure_isc) = 1;
	*reinterpret_cast<std::uintptr_t*>(c + closure_env) = e;
	*reinterpret_cast<std::uint8_t*>(c + closure_nupvalues) = nelems;
	*reinterpret_cast<std::uint8_t*>(c + closure_stacksize) = 20;
	*reinterpret_cast<std::uint8_t*>(c + closure_preload) = 0;
	*reinterpret_cast<std::uintptr_t*>(c + c_closure_f) = -(c + c_closure_f);
	*reinterpret_cast<std::uintptr_t*>(c + c_closure_cont) = (c + c_closure_cont);
	*reinterpret_cast<std::uintptr_t*>(c + c_closure_debugname) = (c + c_closure_debugname);
	return c;
}

void r_lua_pushcclosure(const std::uintptr_t rL, const std::uintptr_t fn, std::uint32_t nup)
{
	auto cl = r_luaF_newCclosure(rL, nup, *reinterpret_cast<const std::uintptr_t*>(r_index2adr(rL, -10001)));

	const auto nupvalue = *reinterpret_cast<r_TValue**>(rL + lua_state_top) -= nup;
	const auto upvalue = reinterpret_cast<void*>((cl + c_closure_upvals) + (16u * nup));

	while (nup--)
		*reinterpret_cast<double*>(upvalue) = *reinterpret_cast<const double*>(nupvalue);

	r_setval(rL, R_LUA_TFUNCTION, cl);
	r_incr_top(rL);
	return;
}

std::uint32_t* r_luaH_new(const std::uintptr_t rL)
{
	auto t = reinterpret_cast<std::uintptr_t*>(r_luaM_realloc_(rL, 36u, *reinterpret_cast<BYTE*>(rL + offsets::l_activememcat)));
	r_luaC_link(rL, reinterpret_cast<std::uintptr_t>(t), R_LUA_TTABLE);

	*reinterpret_cast<std::uintptr_t*>(t + offsets::t_metatable) = *reinterpret_cast<std::uintptr_t*>(rL + 12); // requires metatable obfuscation
	*reinterpret_cast<std::uint8_t*>(t + offsets::t_flags) = 0;
	*reinterpret_cast<std::uintptr_t*>(t + offsets::t_array_) = 0;
	*reinterpret_cast<std::uintptr_t*>(t + offsets::t_sizearray) = 0;
	*reinterpret_cast<std::uint8_t*>(t + offsets::t_lsizenode) = 0;
	*reinterpret_cast<std::uint8_t*>(t + offsets::t_readonly) = 0;
	*reinterpret_cast<std::uint8_t*>(t + offsets::t_safeenv) = 0;
	*reinterpret_cast<std::uint8_t*>(t + offsets::t_nodemask8) = 0;

	return t;
	// side note: we will never use 2nd and 3rd arg so we just completely remove the use of em
}

void r_lua_createtable(std::uintptr_t rL)
{
	r_sethvalue(*reinterpret_cast<r_TValue**>(rL + offsets::top), r_luaH_new(rL));
	r_incr_top(rL);
	return;
}

/* NEWTHREAD */

void stack_init(const std::uintptr_t rL, const std::uintptr_t L1)
{
	std::uint32_t v89 = L1;
	std::uint32_t v72 = rL;

	auto v94 = *(_DWORD*)(v72 + 20) - (v72 + 20);
	std::uint32_t v369 = v94;
	std::uint32_t 	v376 = *(_BYTE*)(v89 + 6);

	std::uint32_t v90 = v369;
	std::uint32_t v91 = v376;
	*(_DWORD*)(v369 + 56) += 112;
	*(_DWORD*)(v90 + 4 * v91 + 200) += 112;
	std::uint32_t v92 = *(_DWORD*)(v72 + 20) - (v72 + 20);
	*(_DWORD*)v89 = *(_DWORD*)(v92 + 24);
	RLOBYTE(v91) = *(_BYTE*)(v92 + 20);
	*(_DWORD*)(v92 + 24) = v89;
	*(_BYTE*)(v89 + 4) = v91 & 3;
	*(_BYTE*)(v89 + 5) = 8;
	RLOBYTE(v91) = *(_BYTE*)(v72 + 8);
	*(_BYTE*)(v89 + 6) = v91;
	v376 = v91;
	std::uint32_t v93 = *(_DWORD*)(v72 + 20) - (v72 + 20);
	*(_DWORD*)(v89 + 100) = 0;
	*(_DWORD*)(v89 + 20) = v89 + 20 + v93;
	*(_DWORD*)(v89 + 32) = 0;
	*(_DWORD*)(v89 + 44) = v89 + 44;
	*(_DWORD*)(v89 + 7) = 0;
	*(_DWORD*)(v89 + 48) = 0;
	*(_DWORD*)(v89 + 52) = 0;
	*(_DWORD*)(v89 + 16) = 0;
	*(_DWORD*)(v89 + 40) = 0;
	*(_DWORD*)(v89 + 104) = 0;
	*(_DWORD*)(v89 + 56) = 0;
	*(_DWORD*)(v89 + 108) = 0;
	*(_DWORD*)(v89 + 92) = 0;
	*(_BYTE*)(v89 + 8) = *(_BYTE*)(v72 + 8);
	auto v95 = (*(int(__cdecl**)(int, _DWORD, _DWORD, _DWORD, int))(v94 + 12))(v72, *(_DWORD*)(v94 + 16), 0, 0, 192);
	std::uint32_t v96 = v95;
	std::uint32_t v97 = v369;
	std::uint32_t v98 = v376;
	*(_DWORD*)(v369 + 56) += 192;
	*(_DWORD*)(v97 + 4 * v98 + 200) += 192;
	*(_DWORD*)(v89 + 36) = v96 + 168;
	*(_DWORD*)(v89 + 40) = v96;
	*(_DWORD*)(v89 + 16) = v96;
	*(_DWORD*)(v89 + 48) = 8;
	auto v99 = (*(int(__cdecl**)(int, _DWORD, _DWORD, _DWORD, int))(v369 + 12))(v72, *(_DWORD*)(v369 + 16), 0, 0, 720);
	std::uint32_t v100 = v369;
	std::uint32_t v101 = v376;
	*(_DWORD*)(v369 + 56) += 720;
	*(_DWORD*)(v100 + 4 * v101 + 200) += 720;
	std::uint32_t v102 = 0;
	*(_DWORD*)(v89 + 32) = v99;
	*(_DWORD*)(v89 + 44) = v89 + 89;
	do
	{
		*(_DWORD*)(v102 + *(_DWORD*)(v89 + 32) + 12) = 0;
		v102 += 16;
	} while (v102 < 720);
	auto v103 = *(_DWORD*)(v89 + 32);
	*(_DWORD*)(v89 + 24) = v103;
	*(_DWORD*)(v89 + 28) = v103 + 16 * (*(_DWORD*)(v89 + 44) - (v89 + 44) - 6);
	**(_DWORD**)(v89 + 16) = v103;
	*(_DWORD*)(*(_DWORD*)(v89 + 24) + 12) = 0;
	*(_DWORD*)(v89 + 24) += 16;
	*(_DWORD*)(*(_DWORD*)(v89 + 16) + 4) = *(_DWORD*)(v89 + 24);
	auto v104 = *(_DWORD*)(v89 + 16);
	*(_DWORD*)(v89 + 12) = *(_DWORD*)(v104 + 4);
	*(_DWORD*)(v104 + 12) = *(_DWORD*)(v89 + 24) + 320;
	*(_DWORD*)(v89 + 80) = *(_DWORD*)(v72 + 80);
	*(_BYTE*)(v89 + 10) = *(_BYTE*)(v72 + 10);
	auto v105 = *(_DWORD*)(v72 + 24);
	*(_DWORD*)v105 = v89;
	*(_DWORD*)(v105 + 12) = 8;
	*(_DWORD*)(v72 + 24) += 16;
	auto v106 = *(_DWORD*)(*(_DWORD*)(v72 + 20) - (v72 + 20) + 2004);
	if (v106)
		((void(__cdecl*)(int, int))v106)(v72, v89);
}

std::uintptr_t r_luaE_newthread(const std::uintptr_t rL)
{
	std::uintptr_t L1 = celi_getstate(r_luaM_realloc_(rL, 0, celi_statesize(112), 0));
	r_luaC_link(rL, L1, R_LUA_TTHREAD);
	stack_init(rL, L1);
	celi_getspace(L1);
	return L1;
}

std::uintptr_t r_lua_newthread(const std::uintptr_t rL)
{
	std::uintptr_t L1 = r_luaE_newthread(rL);
	r_setthvalue(*(r_TValue**)(rL + offsets::top), L1);
	r_incr_top(rL);
	return L1;
}
