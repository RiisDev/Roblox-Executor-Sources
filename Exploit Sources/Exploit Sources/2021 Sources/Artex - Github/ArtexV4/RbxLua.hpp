#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "Lua/llimits.h"

namespace Top
{
	static std::uintptr_t L_Top = 16;
	static std::uintptr_t L_Base = 20;

	static std::uintptr_t L_Env = 64;

}

class Roblox
{
	using lua_State = std::uintptr_t;
public:
	//state provider
	inline std::uintptr_t __cdecl newthread(std::uintptr_t r_L);
	inline std::uintptr_t __cdecl close(std::uintptr_t r_L);


	//lua functions

	//Stack 'n top functions
	inline int gettop(std::uintptr_t r_L);
	inline void settop(std::uintptr_t r_L, int value);
	inline void pushvalue(std::uintptr_t r_L, int value);
	inline void remove(std::uintptr_t r_L, int value);
	inline void insert(std::uintptr_t r_L, int value);
	inline void pop(std::uintptr_t r_L = 0);
	inline void replace(std::uintptr_t r_L, int value);
	inline void xmove(std::uintptr_t r_L, int value, int to);

	//access stack functions -> (C)
	inline DOUBLE isnumber(std::uintptr_t r_L, double isItDoubled);
	inline int isstring(std::uintptr_t r_L, int isItStringed);
	inline int iscfunction(std::uintptr_t r_L, int isItFunctioned);
	inline int isuserdata(std::uintptr_t r_L, int ud);
	inline int   r_type(std::uintptr_t r_L, int idx);
	const char* (r_typename)(std::uintptr_t L, int tp);
	int            (r_equal)(std::uintptr_t L, int idx1, int idx2);
	int            (r_rawequal)(std::uintptr_t L, int idx1, int idx2);
	int            (r_lessthan)(std::uintptr_t L, int idx1, int idx2);

	//std::uintptr_t L
	
	double (tonumber) (std::uintptr_t L, int idx);
	int (tointeger) (std::uintptr_t L, int idx);
	int             (toboolean)(std::uintptr_t L, int idx);
    const char* (tolstring)(std::uintptr_t L, int idx, size_t* len);
	size_t (objlen) (std::uintptr_t L, int idx);
	void (tocfunction) (std::uintptr_t L, int idx);
    void* (touserdata)(std::uintptr_t L, int idx);
	int* (tothread)(std::uintptr_t L, int idx);
	const void* (topointer)(std::uintptr_t L, int idx);

	//push to stack functions
	 void  (pushnil)(lua_State L);
	 void  (pushnumber)(lua_State L, double n);
	 void  (pushinteger)(lua_State L, int n);
	 void  (pushlstring)(lua_State L, const char* s, size_t l);
	 void  (pushstring)(lua_State L, const char* s);
	 const char* (pushvfstring)(lua_State L, const char* fmt,
		va_list argp);
	 const char* (lua_pushfstring)(lua_State L, const char* fmt, ...);
	 void  (pushcclosure)(lua_State L, int fn, int n, int n2, int n3);
	 void  (pushboolean)(lua_State L, int b);
	 void  (pushlightuserdata)(lua_State L, void* p);
	 int   (pushthread)(lua_State L);

	 //get functions (lua)

	  void  (gettable)(lua_State L, int idx);
	  void  (getfield)(lua_State L, int idx, const char* k);
	  void  (rawget)(lua_State L, int idx);
	  void  (rawgeti)(lua_State L, int idx, int n);
	  void  (createtable)(lua_State L, int narr, int nrec);
	  void* (newuserdata)(lua_State L, size_t sz);
	  int   (getmetatable)(lua_State L, int objindex);
	  void  (getfenv)(lua_State L, int idx);

	  //set functions 'lua'
	   void  (settable)(lua_State* L, int idx);
	   void  (setfield)(lua_State* L, int idx, const char* k);
	   void  (rawset)(lua_State* L, int idx);
	   void  (rawseti)(lua_State* L, int idx, int n);
	   int   (setmetatable)(lua_State* L, int objindex);
	   int   (setfenv)(lua_State* L, int idx);
private:
	//our private class functions
};

inline int Roblox::gettop(std::uintptr_t r_L)
{
	return cast_int(r_L + 16 - r_L + 20);
}

inline void Roblox::settop(std::uintptr_t r_L, int value)
{

}