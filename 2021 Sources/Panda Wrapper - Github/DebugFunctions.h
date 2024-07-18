#include "Wrapper.h"
#include <Psapi.h>
#include "Rlua.h"

static int db_setmetatable(lua_State* L) {
	int t = lua_type(L, 2);
	luaL_argcheck(L, t == LUA_TNIL || t == LUA_TTABLE, 2,
		"nil or table expected");
	lua_settop(L, 2);
	lua_pushboolean(L, lua_setmetatable(L, 1));
	return 1;
}

int db_getmetatable(lua_State* L) {
	luaL_checkany(L, 1);
	if (!lua_getmetatable(L, 1)) {
		lua_pushnil(L);  /* no metatable */
	}
	return 1;
}

int db_getfenv(lua_State* L) {
	luaL_checkany(L, 1);
	lua_getfenv(L, 1);
	return 1;
}

int db_getregistry(lua_State* L) {
	rlua_pushvalue(rL, LUA_REGISTRYINDEX);
	return 1;
}

static int db_setfenv(lua_State* L) {
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_settop(L, 2);
	if (lua_setfenv(L, 1) == 0)
		luaL_error(L, LUA_QL("setfenv")
			" cannot change environment of given object");
	return 1;
}

static int auxupvalue(lua_State* L, int get) {
	const char* name;
	int n = luaL_checkint(L, 2);
	luaL_checktype(L, 1, LUA_TFUNCTION);
	if (lua_iscfunction(L, 1)) return 0;
	name = get ? lua_getupvalue(L, 1, n) : lua_setupvalue(L, 1, n);
	if (name == NULL) return 0;
	lua_pushstring(L, name);
	lua_insert(L, -(get + 1));
	return get + 1;
}

static int db_getupvalue(lua_State* L) {
	return auxupvalue(L, 1);
}

static int db_setupvalue(lua_State* L) {
	luaL_checkany(L, 3);
	return auxupvalue(L, 0);
}

static lua_State* getthread(lua_State* L, int* arg) {
	if (lua_isthread(L, 1)) {
		*arg = 1;
		return lua_tothread(L, 1);
	}
	else {
		*arg = 0;
		return L;
	}
}

static void settabss(lua_State* L, const char* i, const char* v) {
	lua_pushstring(L, v);
	lua_setfield(L, -2, i);
}

static void settabsi(lua_State* L, const char* i, int v) {
	lua_pushinteger(L, v);
	lua_setfield(L, -2, i);
}

static void treatstackoption(lua_State* L, lua_State* L1, const char* fname) {
	if (L == L1) {
		lua_pushvalue(L, -2);
		lua_remove(L, -3);
	}
	else
		lua_xmove(L1, L, 1);
	lua_setfield(L, -2, fname);
}

static int db_getinfo(lua_State* L) {
	lua_Debug ar;
	int arg;
	lua_State* L1 = getthread(L, &arg);
	const char* options = luaL_optstring(L, arg + 2, "flnSu");
	if (lua_isnumber(L, arg + 1)) {
		if (!lua_getstack(L1, (int)lua_tointeger(L, arg + 1), &ar)) {
			lua_pushnil(L);  /* level out of range */
			return 1;
		}
	}
	else if (lua_isfunction(L, arg + 1)) {
		lua_pushfstring(L, ">%s", options);
		options = lua_tostring(L, -1);
		lua_pushvalue(L, arg + 1);
		lua_xmove(L, L1, 1);
	}
	else
		return luaL_argerror(L, arg + 1, "function or level expected");
	if (!lua_getinfo(L1, options, &ar))
		return luaL_argerror(L, arg + 2, "invalid option");
	lua_createtable(L, 0, 2);
	if (strchr(options, 'S')) {
		settabss(L, "source", ar.source);
		settabss(L, "short_src", ar.short_src);
		settabsi(L, "linedefined", ar.linedefined);
		settabsi(L, "lastlinedefined", ar.lastlinedefined);
		settabss(L, "what", ar.what);
	}
	if (strchr(options, 'l'))
		settabsi(L, "currentline", ar.currentline);
	if (strchr(options, 'u'))
		settabsi(L, "nups", ar.nups);
	if (strchr(options, 'n')) {
		settabss(L, "name", ar.name);
		settabss(L, "namewhat", ar.namewhat);
	}
	if (strchr(options, 'L'))
		treatstackoption(L, L1, "activelines");
	if (strchr(options, 'f'))
		treatstackoption(L, L1, "func");
	return 1;  /* return table */
}

#define LEVELS1	12
#define LEVELS2	10

static int db_traceback(lua_State * L) {
	int level;
	int firstpart = 1;  /* still before eventual `...' */
	int arg;
	lua_State* L1 = getthread(L, &arg);
	lua_Debug ar;
	if (lua_isnumber(L, arg + 2)) {
		level = (int)lua_tointeger(L, arg + 2);
		lua_pop(L, 1);
	}
	else
		level = (L == L1) ? 1 : 0;  /* level 0 may be this own function */
	if (lua_gettop(L) == arg)
		lua_pushliteral(L, "");
	else if (!lua_isstring(L, arg + 1)) return 1;  /* message is not a string */
	else lua_pushliteral(L, "\n");
	lua_pushliteral(L, "stack traceback:");
	while (lua_getstack(L1, level++, &ar)) {
		if (level > LEVELS1 && firstpart) {
			/* no more than `LEVELS2' more levels? */
			if (!lua_getstack(L1, level + LEVELS2, &ar))
				level--;  /* keep going */
			else {
				lua_pushliteral(L, "\n\t...");  /* too many levels */
				while (lua_getstack(L1, level + LEVELS2, &ar))  /* find last levels */
					level++;
			}
			firstpart = 0;
			continue;
		}
		lua_pushliteral(L, "\n\t");
		lua_getinfo(L1, "Snl", &ar);
		lua_pushfstring(L, "%s:", ar.short_src);
		if (ar.currentline > 0)
			lua_pushfstring(L, "%d:", ar.currentline);
		if (*ar.namewhat != '\0')  /* is there a name? */
			lua_pushfstring(L, " in function " LUA_QS, ar.name);
		else {
			if (*ar.what == 'm')  /* main? */
				lua_pushfstring(L, " in main chunk");
			else if (*ar.what == 'C' || *ar.what == 't')
				lua_pushliteral(L, " ?");  /* C function or tail call */
			else
				lua_pushfstring(L, " in function <%s:%d>",
					ar.short_src, ar.linedefined);
		}
		lua_concat(L, lua_gettop(L) - arg);
	}
	lua_concat(L, lua_gettop(L) - arg);
	return 1;
}

static TValue* lua_index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

static int db_getupvalues(lua_State* L) {
	if (lua_gettop(L) < 1) return luaL_error(L, "debug.getupvalues requires 1 argument");
	int Size;
	TValue* Obj = lua_index2adr(L, 1);
	if (Obj->tt != LUA_TFUNCTION) return luaL_error(L, "#1 argument must be function");
	Closure* f = clvalue(Obj);
	if (f->c.isC) Size = f->c.nupvalues; else Size = f->l.p->sizeupvalues;
	if (Size) {
		lua_newtable(L);
		for (int i = 1; i <= Size; ++i) {
			const char* Name = lua_getupvalue(L, 1, i);
			lua_setfield(L, -2, Name);
		}
	}
	/*luaL_argcheck(L, lua_isfunction(L, 1) || lua_isnumber(L, 1), 1, "function or number expected");
	if (lua_isnumber(L, 1))
	{
		lua_Debug ar;
		if (!lua_getstack(L, lua_tointeger(L, 1), &ar))
			return luaL_argerror(L, 1, "level out of range");
		lua_getinfo(L, "f", &ar);
	}
	lua_newtable(L);
	int n = 1;
	while (const char* name = lua_getupvalue(L, -2, n))
	{
		lua_pushstring(L, name);
		lua_pushvalue(L, -2);
		lua_settable(L, -4);
		lua_pop(L, 1);
		n++;
	}
	return 1;*/
}

void RegisterDebugFunctions(lua_State* L) {
	lua_createtable(L, 0, 0); 
	{
		lua_pushcfunction(L, db_getmetatable);
		lua_setfield(L, -2, "getmetatable");
		lua_pushcfunction(L, db_setmetatable);
		lua_setfield(L, -2, "setmetatable");
		lua_pushcfunction(L, db_getregistry);
		lua_setfield(L, -2, "getregistry");
		lua_pushcfunction(L, db_getupvalue);
		lua_setfield(L, -2, "getupvalue");
		lua_pushcfunction(L, db_setupvalue);
		lua_setfield(L, -2, "setupvalue");
		lua_pushcfunction(L, db_getfenv);
		lua_setfield(L, -2, "getfenv");
		lua_pushcfunction(L, db_setfenv);
		lua_setfield(L, -2, "setfenv");
		lua_pushcfunction(L, db_getinfo);
		lua_setfield(L, -2, "getinfo");
		lua_pushcfunction(L, db_traceback);
		lua_setfield(L, -2, "traceback");
	}
	lua_setglobal(L, "debug");
}