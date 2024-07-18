#include <Windows.h>
#include <vector>

using namespace std;



#define lua_pop(lua_State,n) lua_settop(lua_State,-(n)-1)
#define lua_pushcfunction(lua_State,f) lua_pushcclosure(lua_State,f,0)
#define lua_setglobal(lua_State,k) lua_setfield(lua_State,-10002,k)
#define lua_getglobal(lua_State,k) lua_getfield(lua_State,-10002,k)
#define lua_tostring(lua_State,k) lua_tolstring(lua_State,(k), NULL)


typedef int* (__cdecl *printIdentity)(int lua_State);
typedef int* (__cdecl *GetContext)();
typedef int(__cdecl *luaL_NewState)();
typedef void* (__cdecl *lua_GetField)(int lua_State, int idx, const char *k);
typedef int(__cdecl *lua_PCall)(int lua_State, int nargs, int nresults, int errfunc);
typedef void(__cdecl *lua_PushString)(int lua_State, const char *s);
typedef const char *(__cdecl *lua_ToLString)(int lua_State, int idx, size_t *len);
typedef void* (__cdecl *lua_PushValue)(int lua_State, int idx);
typedef void* (__cdecl *lua_LoadFile)(int lua_State, const char *filePath);
typedef const char* (__cdecl *lua_LoadString)(int lua_State, int a2, const char * LuaCode);
typedef int(__cdecl *lua_GetTop)(int lua_State);
typedef void* (__cdecl *lua_SetTop)(int lua_State, int n);
typedef void* (__cdecl *lua_SetField)(int lua_State, int idx, const char *k);
typedef void* (__cdecl *lua_PushCClosure)(int lua_State, int fn(), int n);
typedef void* (__cdecl *lua_Remove)(int lua_State, int idx);
typedef int(__cdecl *Print)();





static luaL_NewState luaL_newstate;
static lua_GetField lua_getfield;
static lua_PCall lua_pcall;
static lua_PushString lua_pushstring;
static lua_ToLString lua_tolstring;
static lua_PushValue lua_pushvalue;
static lua_LoadFile lua_loadfile;
static lua_LoadString lua_loadstring;
static lua_GetTop lua_gettop;
static lua_SetTop lua_settop;
static lua_SetField lua_setfield;
static lua_PushCClosure lua_pushcclosure;
static lua_Remove lua_remove;
static Print print;
static GetContext context;
static printIdentity printID;


int luaState = 0; // bow down to the mighty LuaState!