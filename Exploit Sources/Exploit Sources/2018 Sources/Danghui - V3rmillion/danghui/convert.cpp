
#include <Windows.h>
#include <emmintrin.h>
#include "globals.hpp"
#include "convert.hpp"
#include "wrap.hpp"
#include "Lua/lstate.h"
#include "Libraries/VMP/VMProtectSDK.h"

void* r_luaS_newlstr(unsigned int rL, const char* str, size_t l)
{
	typedef void*(__cdecl* _newlstr)(unsigned int, const char*, size_t);
	_newlstr newlstr = (_newlstr)(DH_NEWLSTR);
	return newlstr(rL, str, l);
}

lua_Number r_luaA_signnumber(unsigned int rL, lua_Number from)
{
	lua_Number* Exchg = cast(lua_Number*, DH_XORCONS);
	__m128d xmmKey = _mm_load_pd(Exchg);
	__m128d xmmData = _mm_load_sd(&from);
	__m128d xmmResult = _mm_xor_pd(xmmData, xmmKey);
	return _mm_cvtsd_f64(xmmResult);
}

int danghui_convert(lua_State* L, unsigned int rL, TValue* src, r_TValue* dest)
{
	VMProtectBeginMutation("convertingLOLE");
	switch (src->tt)
	{
		case LUA_TNIL:
			dest->tt = R_LUA_TNIL;
			return TRUE;
		case LUA_TBOOLEAN:
			dest->tt = R_LUA_TBOOLEAN;
			dest->value.b = src->value.b;
			return TRUE;
		case LUA_TNUMBER:
			dest->tt = R_LUA_TNUMBER;
			dest->value.n = r_luaA_signnumber(rL, src->value.n);
			return TRUE;
		case LUA_TSTRING:
		{
			TString* ts = cast(TString*, src->value.gc);
			dest->tt = R_LUA_TSTRING;
			dest->value.p = r_luaS_newlstr(rL, getstr(ts), strlen(getstr(ts)));
			return TRUE;
		}
		case LUA_TLIGHTUSERDATA:
			dest->tt = R_LUA_TLIGHTUSERDATA;
			dest->value.p = src->value.p;
			return TRUE;
		case LUA_TUSERDATA:
		{
			r_TValue* ts = cast(r_TValue*, rawuvalue(src) + 1);
			dest->tt = R_LUA_TUSERDATA;
			dest->value = ts->value;
			return TRUE;
		}
		case LUA_TFUNCTION:
		{
			Closure* ts = cast(Closure*, src->value.gc);
			if (ts->l.isC)
			{
				if (ts->c.upvalue && ts->c.upvalue[0].tt == LUA_TLIGHTUSERDATA)
				{
					dest->tt = R_LUA_TFUNCTION;
					dest->value.p = ts->c.upvalue[0].value.p;
					return TRUE;
				}
			}
			return FALSE;
		}
		default: return FALSE;
	}
	return FALSE;
	VMProtectEnd();
 }
