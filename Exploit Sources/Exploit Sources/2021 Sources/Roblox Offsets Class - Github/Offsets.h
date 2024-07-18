#include <iostream>
#include <Windows.h>
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
/* Made by Kooxy 2021 Sometime in August 2021. */

class RobloxLuaTypes
{
public:
    DWORD R_LUA_TNONE;
    DWORD R_LUA_TUSERDATA;
    DWORD R_LUA_TFUNCTION;
    DWORD R_LUA_TSTRING;
    DWORD R_LUA_TBOOLEAN;
    DWORD R_LUA_TNUMBER;
    DWORD R_LUA_TTABLE;
    DWORD R_LUA_TNIL;
    DWORD R_LUA_TTHREAD;
    DWORD R_LUA_TVECTOR;
    DWORD R_LUA_TLIGHTUSERDATA;
    DWORD R_LUA_TPROTO;
    DWORD R_LUA_TUPVALUE;
    void PrintTypes();
    void SetRobloxTypes();
};


class RobloxLuaState
{
public:
    DWORD lstate_env;  
    DWORD lstate_l_gt; 
    DWORD lstate_base; 
    DWORD lstate_top; 
    DWORD lstack_last;
    DWORD lstate_ci;  
    DWORD lstate_stack;
    DWORD lstate_baseci;
    DWORD lstate_sizeci;
    DWORD lstate_savedpc;
    void SetLuaStateOffsets();
};

class RobloxCallInfo 
{
public:
    DWORD callinfo_func;// = 28;
    void SetRobloxCallInfoOffsets();

};

class RobloxGlobalStateOffsets
{
public:
    DWORD gstate_currentwhite;
    DWORD gstate_gcstate;
    DWORD gstate_gcstepmul;
    DWORD gstate_gray;
    DWORD gstate_gcthreshold;
    DWORD gstate_totalbytes;
    DWORD gstate_grayagain;
    DWORD gstate_mainthread;
    DWORD gstate_rootgc;
    DWORD gstate_sweepstrgc;
    DWORD gstate_weak;
    void SetRobloxGlobalStateOffsets();

};


class RobloxClosure
{
public:
    DWORD cl_env;
    DWORD cl_isC;
    DWORD cl_nups;
    void SetRobloxClosureOffsets();

};


class RobloxOffsets
{
public:
    DWORD GetNameCallMethod;
    DWORD networkedDM;
    DWORD Identity1;
    DWORD Identity2;
    void SetRobloxOffsets();

};

class RobloxTValue
{
public:
    DWORD luaTTValue;
    void SetRobloxTValueOffsets();
};


extern RobloxOffsets* rbx_offsets;
extern RobloxLuaTypes* T;
extern RobloxTValue* tval;
extern RobloxClosure* rbxcl;
extern RobloxGlobalStateOffsets* RbxglobalState;
extern RobloxCallInfo* rbx_callinfo;
