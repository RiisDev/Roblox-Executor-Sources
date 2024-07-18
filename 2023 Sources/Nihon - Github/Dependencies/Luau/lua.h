#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "luaconf.h"

#define LUA_MULTRET (-1)

#define LUA_REGISTRYINDEX (-LUAI_MAXCSTACK - 2000)
#define LUA_ENVIRONINDEX (-LUAI_MAXCSTACK - 2001)
#define LUA_GLOBALSINDEX (-LUAI_MAXCSTACK - 2002)
#define lua_upvalueindex(i) (LUA_GLOBALSINDEX - (i))
#define lua_ispseudo(i) ((i) <= LUA_REGISTRYINDEX)

enum lua_Status
{
    LUA_OK = 0,
    LUA_YIELD,
    LUA_ERRRUN,
    LUA_ERRSYNTAX,
    LUA_ERRMEM,
    LUA_ERRERR,
    LUA_BREAK,
};

enum lua_CoStatus
{
    LUA_CORUN = 0,
    LUA_COSUS,
    LUA_CONOR,
    LUA_COFIN,
    LUA_COERR,
};

typedef struct lua_State lua_State;

typedef int (*lua_CFunction)(lua_State* L);
typedef int (*lua_Continuation)(lua_State* L, int status);

typedef void* (*lua_Alloc)(void* ud, void* ptr, size_t osize, size_t nsize);

#define l_noret void LUA_NORETURN

#define LUA_TNONE (-1)

enum lua_Type
{
    LUA_TNIL = 0,
    LUA_TBOOLEAN = 1,

    LUA_TLIGHTUSERDATA,
    LUA_TVECTOR,
    LUA_TNUMBER,

    LUA_TSTRING,

    LUA_TUSERDATA,
    LUA_TTHREAD,
    LUA_TFUNCTION,
    LUA_TTABLE,

    LUA_TPROTO,
    LUA_TUPVAL,
    LUA_TDEADKEY,

    LUA_T_COUNT = LUA_TPROTO
};

typedef double lua_Number;

typedef int lua_Integer;

typedef unsigned lua_Unsigned;

LUA_API lua_State* lua_newstate(lua_Alloc f, void* ud);
LUA_API void lua_close(lua_State* L);
LUA_API lua_State* lua_newthread(lua_State* L);
LUA_API lua_State* lua_mainthread(lua_State* L);
LUA_API void lua_resetthread(lua_State* L);
LUA_API int lua_isthreadreset(lua_State* L);

LUA_API int lua_absindex(lua_State* L, int idx);
LUA_API int lua_gettop(lua_State* L);
LUA_API void lua_settop(lua_State* L, int idx);
LUA_API void lua_pushvalue(lua_State* L, int idx);
LUA_API void lua_remove(lua_State* L, int idx);
LUA_API void lua_insert(lua_State* L, int idx);
LUA_API void lua_replace(lua_State* L, int idx);
LUA_API int lua_checkstack(lua_State* L, int sz);
LUA_API void lua_rawcheckstack(lua_State* L, int sz);

LUA_API void lua_xmove(lua_State* from, lua_State* to, int n);
LUA_API void lua_xpush(lua_State* from, lua_State* to, int idx);

LUA_API int lua_isnumber(lua_State* L, int idx);
LUA_API int lua_isstring(lua_State* L, int idx);
LUA_API int lua_iscfunction(lua_State* L, int idx);
LUA_API int lua_isLfunction(lua_State* L, int idx);
LUA_API int lua_isuserdata(lua_State* L, int idx);
LUA_API int lua_type(lua_State* L, int idx);
LUA_API const char* lua_typename(lua_State* L, int tp);

LUA_API int lua_equal(lua_State* L, int idx1, int idx2);
LUA_API int lua_rawequal(lua_State* L, int idx1, int idx2);
LUA_API int lua_lessthan(lua_State* L, int idx1, int idx2);

LUA_API double lua_tonumberx(lua_State* L, int idx, int* isnum);
LUA_API int lua_tointegerx(lua_State* L, int idx, int* isnum);
LUA_API unsigned lua_tounsignedx(lua_State* L, int idx, int* isnum);
LUA_API const float* lua_tovector(lua_State* L, int idx);
LUA_API int lua_toboolean(lua_State* L, int idx);
LUA_API const char* lua_tolstring(lua_State* L, int idx, size_t* len);
LUA_API const char* lua_tostringatom(lua_State* L, int idx, int* atom);
LUA_API const char* lua_namecallatom(lua_State* L, int* atom);
LUA_API int lua_objlen(lua_State* L, int idx);
LUA_API lua_CFunction lua_tocfunction(lua_State* L, int idx);
LUA_API void* lua_tolightuserdata(lua_State* L, int idx);
LUA_API void* lua_touserdata(lua_State* L, int idx);
LUA_API void* lua_touserdatatagged(lua_State* L, int idx, int tag);
LUA_API int lua_userdatatag(lua_State* L, int idx);
LUA_API lua_State* lua_tothread(lua_State* L, int idx);
LUA_API const void* lua_topointer(lua_State* L, int idx);

LUA_API void lua_pushnil(lua_State* L);
LUA_API void lua_pushnumber(lua_State* L, double n);
LUA_API void lua_pushinteger(lua_State* L, int n);
LUA_API void lua_pushunsigned(lua_State* L, unsigned n);
#if LUA_VECTOR_SIZE == 4
LUA_API void lua_pushvector(lua_State* L, float x, float y, float z, float w);
#else
LUA_API void lua_pushvector(lua_State* L, float x, float y, float z);
#endif
LUA_API void lua_pushlstring(lua_State* L, const char* s, size_t l);
LUA_API void lua_pushstring(lua_State* L, const char* s);
LUA_API const char* lua_pushvfstring(lua_State* L, const char* fmt, va_list argp);
LUA_API LUA_PRINTF_ATTR(2, 3) const char* lua_pushfstringL(lua_State* L, const char* fmt, ...);
LUA_API void lua_pushcclosurek(lua_State* L, lua_CFunction fn, const char* debugname, int nup, lua_Continuation cont);
LUA_API void lua_pushboolean(lua_State* L, int b);
LUA_API int lua_pushthread(lua_State* L);

LUA_API void lua_pushlightuserdata(lua_State* L, void* p);
LUA_API void* lua_newuserdatatagged(lua_State* L, size_t sz, int tag);
LUA_API void* lua_newuserdatadtor(lua_State* L, size_t sz, void (*dtor)(void*));

LUA_API int lua_gettable(lua_State* L, int idx);
LUA_API int lua_getfield(lua_State* L, int idx, const char* k);
LUA_API int lua_rawgetfield(lua_State* L, int idx, const char* k);
LUA_API int lua_rawget(lua_State* L, int idx);
LUA_API int lua_rawgeti(lua_State* L, int idx, int n);
LUA_API void lua_createtable(lua_State* L, int narr, int nrec);

LUA_API void lua_setreadonly(lua_State* L, int idx, int enabled);
LUA_API int lua_getreadonly(lua_State* L, int idx);
LUA_API void lua_setsafeenv(lua_State* L, int idx, int enabled);

LUA_API int lua_getmetatable(lua_State* L, int objindex);
LUA_API void lua_getfenv(lua_State* L, int idx);

LUA_API void lua_settable(lua_State* L, int idx);
LUA_API void lua_setfield(lua_State* L, int idx, const char* k);
LUA_API void lua_rawsetfield(lua_State* L, int idx, const char* k);
LUA_API void lua_rawset(lua_State* L, int idx);
LUA_API void lua_rawseti(lua_State* L, int idx, int n);
LUA_API int lua_setmetatable(lua_State* L, int objindex);
LUA_API int lua_setfenv(lua_State* L, int idx);

LUA_API int luau_load(lua_State* L, const char* chunkname, const char* data, size_t size, int env);
LUA_API void lua_call(lua_State* L, int nargs, int nresults);
LUA_API int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc);

LUA_API int lua_yield(lua_State* L, int nresults);
LUA_API int lua_break(lua_State* L);
LUA_API int lua_resume(lua_State* L, lua_State* from, int narg);
LUA_API int lua_resumeerror(lua_State* L, lua_State* from);
LUA_API int lua_status(lua_State* L);
LUA_API int lua_isyieldable(lua_State* L);
LUA_API void* lua_getthreaddata(lua_State* L);
LUA_API void lua_setthreaddata(lua_State* L, void* data);
LUA_API int lua_costatus(lua_State* L, lua_State* co);

enum lua_GCOp
{

    LUA_GCSTOP,
    LUA_GCRESTART,

    LUA_GCCOLLECT,

    LUA_GCCOUNT,
    LUA_GCCOUNTB,

    LUA_GCISRUNNING,

    LUA_GCSTEP,

    LUA_GCSETGOAL,
    LUA_GCSETSTEPMUL,
    LUA_GCSETSTEPSIZE,
};

LUA_API int lua_gc(lua_State* L, int what, int data);

LUA_API void lua_setmemcat(lua_State* L, int category);
LUA_API size_t lua_totalbytes(lua_State* L, int category);

LUA_API l_noret lua_error(lua_State* L);

LUA_API int lua_next(lua_State* L, int idx);
LUA_API int lua_rawiter(lua_State* L, int idx, int iter);

LUA_API void lua_concat(lua_State* L, int n);

LUA_API uintptr_t lua_encodepointer(lua_State* L, uintptr_t p);

LUA_API double lua_clock();

LUA_API void lua_setuserdatatag(lua_State* L, int idx, int tag);

typedef void (*lua_Destructor)(lua_State* L, void* userdata);

LUA_API void lua_setuserdatadtor(lua_State* L, int tag, lua_Destructor dtor);
LUA_API lua_Destructor lua_getuserdatadtor(lua_State* L, int tag);

LUA_API void lua_clonefunction(lua_State* L, int idx);
LUA_API void lua_clonecfunction(lua_State* L, int idx);

LUA_API void lua_cleartable(lua_State* L, int idx);

#define LUA_NOREF -1
#define LUA_REFNIL 0

LUA_API int lua_ref(lua_State* L, int idx);
LUA_API void lua_unref(lua_State* L, int ref);

#define lua_getref(L, ref) lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))

#define lua_tonumber(L, i) lua_tonumberx(L, i, NULL)
#define lua_tointeger(L, i) lua_tointegerx(L, i, NULL)
#define lua_tounsigned(L, i) lua_tounsignedx(L, i, NULL)

#define lua_pop(L, n) lua_settop(L, -(n)-1)

#define lua_newtable(L) lua_createtable(L, 0, 0)
#define lua_newuserdata(L, s) lua_newuserdatatagged(L, s, 0)

#define lua_strlen(L, i) lua_objlen(L, (i))

#define lua_isfunction(L, n) (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L, n) (lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L, n) (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L, n) (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L, n) (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isvector(L, n) (lua_type(L, (n)) == LUA_TVECTOR)
#define lua_isthread(L, n) (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L, n) (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n) (lua_type(L, (n)) <= LUA_TNIL)

#define lua_pushliteral(L, s) lua_pushlstring(L, "" s, (sizeof(s) / sizeof(char)) - 1)
#define lua_pushcfunction(L, fn, debugname) lua_pushcclosurek(L, fn, debugname, 0, NULL)
#define lua_pushcclosure(L, fn, debugname, nup) lua_pushcclosurek(L, fn, debugname, nup, NULL)

#define lua_setglobal(L, s) lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L, s) lua_getfield(L, LUA_GLOBALSINDEX, (s))

#define lua_tostring(L, i) lua_tolstring(L, (i), NULL)

#define lua_pushfstring(L, fmt, ...) lua_pushfstringL(L, fmt, ##__VA_ARGS__)

typedef struct lua_Debug lua_Debug;

typedef void (*lua_Hook)(lua_State* L, lua_Debug* ar);

LUA_API int lua_stackdepth(lua_State* L);
LUA_API int lua_getinfo(lua_State* L, int level, const char* what, lua_Debug* ar);
LUA_API int lua_getargument(lua_State* L, int level, int n);
LUA_API const char* lua_getlocal(lua_State* L, int level, int n);
LUA_API const char* lua_setlocal(lua_State* L, int level, int n);
LUA_API const char* lua_getupvalue(lua_State* L, int funcindex, int n);
LUA_API const char* lua_setupvalue(lua_State* L, int funcindex, int n);

LUA_API void lua_singlestep(lua_State* L, int enabled);
LUA_API int lua_breakpoint(lua_State* L, int funcindex, int line, int enabled);

typedef void (*lua_Coverage)(void* context, const char* function, int linedefined, int depth, const int* hits, size_t size);

LUA_API void lua_getcoverage(lua_State* L, int funcindex, void* context, lua_Coverage callback);

LUA_API const char* lua_debugtrace(lua_State* L);

struct lua_Debug
{
    const char* name;
    const char* what;
    const char* source;
    const char* short_src;
    int linedefined;
    int currentline;
    unsigned char nupvals;
    unsigned char nparams;
    char isvararg;
    void* userdata;

    char ssbuf[LUA_IDSIZE];
};

struct lua_Callbacks
{
    void* userdata;

    void (*interrupt)(lua_State* L, int gc);
    void (*panic)(lua_State* L, int errcode);

    void (*userthread)(lua_State* LP, lua_State* L);
    int16_t(*useratom)(const char* s, size_t l);

    void (*debugbreak)(lua_State* L, lua_Debug* ar);
    void (*debugstep)(lua_State* L, lua_Debug* ar);
    void (*debuginterrupt)(lua_State* L, lua_Debug* ar);
    void (*debugprotectederror)(lua_State* L);
};
typedef struct lua_Callbacks lua_Callbacks;

LUA_API lua_Callbacks* lua_callbacks(lua_State* L);