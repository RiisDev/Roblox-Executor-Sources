/* ATHENA RLUA HEADER */

// Written by Tactical BFG.


// Notice: code is really sloppy and spaghetti code is very present, i'm not a pretty coder, I just write stuff that gets shit done and it gets shit done.

// Also: These addresses & calling conventions will not be updated! (These are from 26/4/18 if you need a Roblox version)

#include "Main.h"
#include "checc.h"

extern "C" {
  #include "Lua/lobject.h"
  #include "Lua/lstate.h"
  #include "Lua/lua.hpp"
}

/* STRUCTURES */
typedef int(rstate);


union Rvalue {
  void *gc;
  const void *p;
  double num;
  int b;
};

struct Rtvalue {
  Rvalue value;
  int tt;
};

/* LUA TYPES */

#define RLUA_TNONE		      (-1)
#define RLUA_TNIL                     0
#define RLUA_TNUMBER                  2
#define RLUA_TBOOLEAN                 3
#define RLUA_TSTRING                  4
#define RLUA_TLIGHTUSERDATA	      1
#define RLUA_TTABLE                   7
#define RLUA_TUSERDATA		      8
#define RLUA_TFUNCTION		      6
#define RLUA_TPROTO                   9
#define RLUA_TTHREAD                  5
#define RLUA_TUPVALUE                 10

/* LUA STACK STUFF */
#define Rluatop     8;
#define Rluabase    12;


#define rlua_inctop(r) *(DWORD*)(r + Rluatop) += sizeof(Rtvalue)

/* ACTUAL RLUA YAY */


namespace lua {


	typedef void(__stdcall *Lua_getfield)(rstate state, int index, const char *what);
	Lua_getfield getfield = (Lua_getfield)Retcheck::unprotect(aslr(0x73F8D0));

	typedef void(__cdecl *Lua_setfield)(rstate state, signed int index, const char *k);
	Lua_setfield asetfield = (Lua_setfield)Retcheck::unprotect(aslr(0x741640));

	typedef void(__stdcall *Lua_pushvalue)(rstate state, int index);
	Lua_pushvalue pushvalue = (Lua_pushvalue)Retcheck::unprotect(aslr(0x740D30));

	typedef void(__stdcall *Lua_pushstring)(rstate state, const char *s);
	Lua_pushstring pushstring = (Lua_pushstring)Retcheck::unprotect(aslr(0x740C70));  //9/23

	typedef const char*(__cdecl *Lua_ToLString)(rstate lst, int idx, size_t *len);
	Lua_ToLString toLstring = (Lua_ToLString)ReturnCheck::UnProtect((BYTE*)aslr(0x741C50));



	typedef void(__fastcall *Lua_settop)(rstate state, int index);
	Lua_settop settop = (Lua_settop)Retcheck::unprotect(aslr(0x741A10));


	typedef int(__cdecl *lpushbool)(rstate r, bool value);
	lpushbool pushboolean = (lpushbool)Retcheck::unprotect(aslr(0x740770));

	typedef bool(__cdecl *tobool)(rstate state, int what);
	tobool toboolean = (tobool)(aslr(0x741B90)); //9/27

	typedef int(__cdecl *pnil)(rstate state);
	pnil pushnil = (pnil)Retcheck::unprotect(aslr(0x740B70));
	 
	typedef int(__cdecl *ins)(rstate r, int a1);
	ins insert = (ins)Retcheck::unprotect(aslr(0x73DF50));
	
	typedef signed int(__cdecl *typ)(rstate state, int jef);
	typ type = (typ)(aslr(0x742090));

	typedef int(__cdecl *nt)(int globgogabgalabstate);
	nt newthread = (nt)Retcheck::unprotect(aslr(0x740380)); //11/15

	typedef int(__cdecl *nxt) (rstate state, int index);
	nxt next = (nxt)Retcheck::unprotect(aslr(0x740530));

	typedef int(__cdecl *lcll) (rstate state, int args, int retn);
	lcll call = (lcll)Retcheck::unprotect(aslr(0x73f2d0));
	
	typedef int(__stdcall *gmt)(rstate r, int a2);
	gmt getmetatable = (gmt)Retcheck::unprotect(aslr(0x73FBA0));

	typedef int(__cdecl *smt)(rstate r, int a2);
	smt setmetatable = (smt)Retcheck::unprotect(aslr(0x7417F0));
	
	typedef signed int(__cdecl *lref)(rstate r, unsigned int func);
	lref ref = (lref)ReturnCheck::UnProtect((BYTE*)aslr(0x73A8B0));

	typedef int(__cdecl *ton)(rstate r, int idx);
	ton tonumber = (ton)aslr(0x741F30);
	
	typedef int(__cdecl *rg)(rstate r, int a2, int a3);
	rg rawgeti = (rg)Retcheck::unprotect(aslr(0x740FE0));
	
	typedef int(__cdecl *ct)(rstate r, int table1, int table2);
	ct createtable = (ct)ReturnCheck::UnProtect((BYTE*)aslr(0x73F580));

	typedef int(__fastcall *i2a)(rstate r, unsigned int idx);
	i2a index2adr = (i2a)aslr(0x737320);

	typedef void*(__cdecl *ncc)(rstate r, int hi, void *gay);
	ncc newcclosure = (ncc)aslr(0x739240);

	typedef int(__cdecl *rpushlight)(DWORD, void *gae);
	rpushlight pushlightUD = (rpushlight)Retcheck::unprotect(aslr(0x73EB10));

	auto toUD = (void*(__cdecl*)(DWORD, int))Retcheck::unprotect(aslr(0x7401B0));

	typedef int(__cdecl *rpushnumber)(rstate r, int num);
	rpushnumber pushnumber = (rpushnumber)Retcheck::unprotect(aslr(0x73ECA0));


	typedef int(__cdecl *newud)(rstate r, int a);
	newud newuserdata = (newud)Retcheck::unprotect(aslr(0x73E520));

	typedef int(__cdecl *top)(rstate r, int idx);
	top topointer = (top)Retcheck::unprotect(aslr(0x741FB0));
	
}

#define lpop(l, n) lua::settop(l, -(n)-1)

/* REWRITTEN FUNCTIONS BECAUSE ADDRESSES ARE LEGIT USELESS SINCE THESE ONLY USE RLUA TYPES */
int checktype(lua_State *Ls, int narg, int t) {
	if (lua::type(L, narg) != t) {
		return 1;
	}
	else {
		return 0;
	}
}

int gettop(int l) {
	return (*(DWORD *)(l + Rluatop) - *(DWORD *)(l + Rluabase)) >> 4;
}

// deadass setfield bypass skidded from cyber aka hexus aka idk
void setfield(int L, int idx, const char *k) {
		lua::pushvalue(L, idx);
		if (lua::getmetatable(L, -1)) {
			lua::getfield(L, -1, "__newindex");
			lua::pushvalue(L, -3);
			lua::pushstring(L, k);
			lua::pushvalue(L, -6);
			lua::call(L, 3, 1);
			lpop(L, 3);
		}
		else {
			lpop(L, 1);
			lua::asetfield(L, idx, k);
		};
};


/* CUSTOM FUNCTIONS (pls dont kill me for releasing these, i expect convex to change readonly's pointer soon but who knows) */

int ReadOnly(lua_State *Ls) {
  bool Is = lua_toboolean(Ls, -1);
  
  DWORD InstanceData = lua::index2adr(L, -1);
  if (!Is) {
    *(BYTE*)(*(DWORD*)InstanceData + 6) = 0;
  } else {
    *(BYTE*)(*(DWORD*)InstanceData + 6) = 1; // tbh this 1 was an assumption but who even sets it back to readonly after disabling it, its not like memcheck :megalul:
  }
  
  return 0;
} // Lua ex: setreadonly(game, false)

int Dbg_Getmetatable(lua_State *Ls) {
  wrapper:PushRbx(Ls, -1);
  if (!lua::getmetatable(L, -1)) 
    lua::pushnil(L);
   wrapper:PushVanilla(Ls, -1);
  
  return 1;
}

int Dbg_Setmetatable(lua_State *Ls) {
  wrapper:PushRbx(Ls, -1);
  lua_pushboolean(Ls, lua::setmetatable(L, -1));
   wrapper:PushVanilla(Ls, -1);
  
  return 1;
}

int ReworkHttpGet(lua_State *Ls) {
  bool NobodyCares = lua_toboolean(Ls, -1); // we all deadass know its gonna be true but still
  std::string LuaUrl = lua_tostring(Ls, -2);
  
  lua_pushstring(Ls, getURL(LuaUrl).c_str());
  return 1;
}

int ReworkHttpPost(lua_State *Ls) {
  string top = lua_tostring(Ls, -2);
	string meme = "http://tacticalbfg.com/postto.php?ws=" +top+"&post="+lua_tostring(Ls, -1);
  // I just use my own HTTP request handler. I just happened to post the postto source in here for you to freely use.
	lua_pushstring(Ls, getURL(meme.c_str()).c_str()); // get request because the server posts it ;)
  return 1;
}


