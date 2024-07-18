// wloder.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "rbxlua.h"
#include "RbxL.h"
#include "scan.h"
#include "Memcheck.h"
#include "detours.h"
#include "lua.hpp"
#include "LuaT.h"
extern "C" {
#include "lauxlib.h"
#include "luaconf.h"
#include "lua.h"
}
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "lua51.lib")

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
    return _iob;
}

int rbxlua_State;

typedef void(__cdecl *retcheckf)();
retcheckf retcheck = (retcheckf)0x5436C0;

typedef int(*instancenew)(int);
instancenew InstanceNew = (instancenew)0x74F970;

int ScriptContext;

std::string Input() {
    std::string wotthefuck;
    getline(std::cin, wotthefuck);
    return wotthefuck;
}

void patchedret() {
    return;
}

void PushInstance(lua_State *L, DWORD addy) {
    lua_newtable(L);
    lua_pushnumber(L, addy);
    lua_setfield(L, -2, "__address");

    lua_getglobal(L, "robject");
    lua_setmetatable(L, -2);
}

static int CreateInstance(lua_State *L) {
    int Args = lua_gettop(L);
    if (Args == 1) {
        const char* Class = lua_tostring(L, -1);
        rbxlua_pushstring(rbxlua_State, Class);
        InstanceNew(rbxlua_State);
        int u = (int)rbxlua_touserdata(rbxlua_State, -1);
        PushInstance(L, u);
        return 1;
    }
    else if (Args == 2) {
        lua_getfield(L, -1, "__address");
        int AddressLoc = lua_tonumber(L, -1);

        const char* Class = lua_tostring(L, -3);
        rbxlua_pushstring(rbxlua_State, Class);

        int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
        *userdata = AddressLoc;
        *(userdata + 1) = 0;
        rbxlua_getfield(rbxlua_State, -10000, "Object");
        rbxlua_setmetatable(rbxlua_State, -2);

        InstanceNew(rbxlua_State);

        int u = (int)rbxlua_touserdata(rbxlua_State, -1);
        PushInstance(L, u);
        return 1;
    }
    else {
        return 0;
    }
}

static int Print(lua_State *L) {
    rbxlua_getfield(rbxlua_State, LUA_GLOBALSINDEX, "print");
    int ArgCount = 0;
    for (int x = lua_gettop(L); x != 0; x -= 1) {
        switch (lua_type(L, x * -1)) {
        case LUA_TNIL: {
            rbxlua_pushnil(rbxlua_State);
            break;
        }
        case LUA_TNUMBER: {
            double no = lua_tonumber(L, x * -1);
            rbxlua_pushnumber(rbxlua_State, no);
            break;
        }
        case LUA_TSTRING: {
            const char* str = lua_tostring(L, x * -1);
            rbxlua_pushstring(rbxlua_State, str);
            break;
        }
        case LUA_TTABLE: {
            lua_getfield(L, -1, "__address");
            if (!lua_isnoneornil(L, -1)) {
                int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
                *userdata = lua_tonumber(L, x * -1);
                *(userdata + 1) = 0;
                rbxlua_getfield(rbxlua_State, -10000, "Object");
                rbxlua_setmetatable(rbxlua_State, -2);
            }
            else {
                lua_pop(L, 1);
                rbxlua_pushstring(rbxlua_State, "table: #NULL");
            }
            break;
        }
        default:
        {
            const char* str = lua_tostring(L, x * -1);
            rbxlua_pushstring(rbxlua_State, str);
            break;
        }
        }
        ArgCount += 1;
    }
    rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
    return 0;
}

static int Warn(lua_State *L) {
    rbxlua_getfield(rbxlua_State, LUA_GLOBALSINDEX, "warn");
    int ArgCount = 0;
    for (int x = lua_gettop(L); x != 0; x -= 1) {
        switch (lua_type(L, x * -1)) {
        case LUA_TNIL: {
            rbxlua_pushnil(rbxlua_State);
            break;
        }
        case LUA_TNUMBER: {
            double no = lua_tonumber(L, x * -1);
            rbxlua_pushnumber(rbxlua_State, no);
            break;
        }
        case LUA_TSTRING: {
            const char* str = lua_tostring(L, x * -1);
            rbxlua_pushstring(rbxlua_State, str);
            break;
        }
        case LUA_TTABLE: {
            lua_getfield(L, -1, "__address");
            if (!lua_isnoneornil(L, -1)) {
                int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
                *userdata = lua_tonumber(L, x * -1);
                *(userdata + 1) = 0;
                rbxlua_getfield(rbxlua_State, -10000, "Object");
                rbxlua_setmetatable(rbxlua_State, -2);
            }
            else {
                lua_pop(L, 1);
                rbxlua_pushstring(rbxlua_State, "table: #NULL");
            }
            break;
        }
        default:
        {
            const char* str = lua_tostring(L, x * -1);
            rbxlua_pushstring(rbxlua_State, str);
            break;
        }
        }
        ArgCount += 1;
    }
    rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
    return 0;
}

static int PrintIdentity(lua_State *L) {
    rbxlua_getfield(rbxlua_State, LUA_GLOBALSINDEX, "printidentity");
    int ArgCount = 0;
    if (lua_gettop(L) != 0) {
        for (int x = lua_gettop(L); x != 0; x -= 1) {
            switch (lua_type(L, x * -1)) {
            case LUA_TNIL: {
                rbxlua_pushnil(rbxlua_State);
                break;
            }
            case LUA_TNUMBER: {
                double no = lua_tonumber(L, x * -1);
                rbxlua_pushnumber(rbxlua_State, no);
                break;
            }
            case LUA_TSTRING: {
                const char* str = lua_tostring(L, x * -1);
                rbxlua_pushstring(rbxlua_State, str);
                break;
            }
            case LUA_TTABLE: {
                lua_getfield(L, -1, "__address");
                if (!lua_isnoneornil(L, -1)) {
                    int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
                    *userdata = lua_tonumber(L, x * -1);
                    *(userdata + 1) = 0;
                    rbxlua_getfield(rbxlua_State, -10000, "Object");
                    rbxlua_setmetatable(rbxlua_State, -2);
                }
                else {
                    lua_pop(L, 1);
                    rbxlua_pushstring(rbxlua_State, "table: #NULL");
                }
                break;
            }
            default:
            {
                const char* str = lua_tostring(L, x * -1);
                rbxlua_pushstring(rbxlua_State, str);
                break;
            }
            }
            ArgCount += 1;
        }
    }
    rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
    return 0;
}

static int GetChildren(lua_State* L) {
    lua_getfield(L, -1, "__address");
    int Instance = lua_tonumber(L, -1);
    lua_pop(L, 2);
    lua_newtable(L);
    DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
    DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
    for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
    {
        PushInstance(L, *(int*)i);
        lua_setfield(L, -2, std::to_string(i).c_str());
    }
    return 1;
}

static int LuaFindFirstChild(lua_State* L) {
    const char* Name = lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "__address");
    int rInstance = lua_tonumber(L, -1);
    lua_pop(L, 2);
    int Instance = FindFirstChild(rInstance, Name);
    if (Instance != 0) {
        PushInstance(L, Instance);
        return 1;
    }
    else {
        return 0;
    }
}

static int CallMethod(lua_State* L) {
    int ArgCount,fArgs = 0;
    try {
        for (int x = lua_gettop(L); x != 0; x -= 1) {
            switch (lua_type(L, x * -1)) {
            case LUA_TNIL: {
                rbxlua_pushnil(rbxlua_State);
                break;
            }
            case LUA_TNUMBER: {
                double no = lua_tonumber(L, x * -1);
                rbxlua_pushnumber(rbxlua_State, no);
                break;
            }
            case LUA_TSTRING: {
                const char* str = lua_tostring(L, x * -1);
                rbxlua_pushstring(rbxlua_State, str);
                break;
            }
            case LUA_TTABLE: {
                lua_getfield(L, -1, "__address");
                if (!lua_isnoneornil(L, -1)) {
                    int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
                    *userdata = lua_tonumber(L, x * -1);
                    *(userdata + 1) = 0;
                    rbxlua_getfield(rbxlua_State, -10000, "Object");
                    rbxlua_setmetatable(rbxlua_State, -2);
                    lua_pop(L, 1);
                }
                else {
                    lua_pop(L, 1);
                    rbxlua_pushstring(rbxlua_State, "table: #NULL");
                }
                break;
            }
            case LUA_TFUNCTION: {
                break;
            }
            default:
            {
                const char* str = lua_tostring(L, x * -1);
                rbxlua_pushstring(rbxlua_State, str);
                break;
            }
            }
            ArgCount += 1;
        }
        rbxlua_pcall(rbxlua_State, ArgCount, LUA_MULTRET, 0);
        for (int x = rbxlua_gettop(rbxlua_State); x != 0; x -= 1) {
            switch (rbxlua_type(rbxlua_State, x * -1)) {
            case RLUA_TNIL: {
                lua_pushnil(L);
                break;
            }
            case RLUA_TNUMBER: {
                const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
                lua_pushnumber(L, ::atof(str));
                break;
            }
            case RLUA_TBOOLEAN: {
                bool _bool = rbxlua_toboolean(rbxlua_State, x * -1);
                lua_pushboolean(L, _bool);
                break;
            }
            case RLUA_TSTRING: {
                const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
                lua_pushstring(L, str);
                break;
            }
            case RLUA_TUSERDATA: {
                int u = (int)rbxlua_touserdata(rbxlua_State, -1);
                PushInstance(L, u);
                break;
            }
            }
            fArgs += 1;
        }
    }
    catch (std::exception e) {
        std::string err = "C ERROR: ";
        err += e.what();
        std::cout << err.c_str() << std::endl;
    }
    return fArgs;
}

static int Index(lua_State* L) {
        std::string Key = lua_tostring(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "__address");
        int AddressLoc = lua_tonumber(L, -1);
        lua_pop(L, 2);

        int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
        *userdata = AddressLoc;
        *(userdata + 1) = 0;
        rbxlua_getfield(rbxlua_State, -10000, "Object");
        rbxlua_setmetatable(rbxlua_State, -2);

        if (Key == "GetChildren" || Key == "getChildren" || Key == "children") {
            lua_pushcfunction(L, GetChildren);
            return 1;
        }
        else if (Key == "FindFirstChild" || Key == "findFirstChild") {
            lua_pushcfunction(L, LuaFindFirstChild);
            return 1;
        }
        else if (Key == "Parent") {
            PushInstance(L, GetParent(AddressLoc));
            return 1;
        }
        else {
            rbxlua_getfield(rbxlua_State, -1, Key.c_str());
            int a = rbxlua_type(rbxlua_State, -1);
            switch (rbxlua_type(rbxlua_State, -1)) {
            case LUA_TNIL: {
                return 0;
            }
            case 2:
            case LUA_TNUMBER: {
                const char* str = rbxlua_tolstring(rbxlua_State, -1, NULL);
                lua_pushnumber(L, ::atof(str));
                return 1;
            }
            case LUA_TSTRING: {
                const char* str = rbxlua_tolstring(rbxlua_State, -1, NULL);
                lua_pushstring(L, str);
                return 1;
            }
            case 6: {
                lua_pushcfunction(L, CallMethod);
                return 1;
            }
            case 8: {
                int instance = FindFirstChild(AddressLoc, Key);
                PushInstance(L, instance);
                return 1;
            }
            }
        }
}

static int NewIndex(lua_State* L) {
    int Type = lua_type(L, -1);

    std::string Key = lua_tostring(L, -2);

    lua_getfield(L, -3, "__address");
    int AddressLoc = lua_tonumber(L, -1);
    lua_pop(L, 1);

    int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
    *userdata = AddressLoc;
    *(userdata + 1) = 0;
    rbxlua_getfield(rbxlua_State, -10000, "Object");
    rbxlua_setmetatable(rbxlua_State, -2);

    switch (Type) {
    case LUA_TNUMBER: {
        double no = lua_tonumber(L, -1);
        rbxlua_pushnumber(rbxlua_State, no);
        rbxlua_setfield(rbxlua_State, -2, Key.c_str());
        return 0;
    }
    case LUA_TSTRING: {
        const char* str = lua_tostring(L, -1);
        rbxlua_pushstring(rbxlua_State, str);
        rbxlua_setfield(rbxlua_State, -2, Key.c_str());
        return 0;
    }
    case LUA_TBOOLEAN: {
        bool a = lua_toboolean(L, -1);
        if (a) {
            rbxlua_pushnumber(rbxlua_State, 1);
        }
        else {
            rbxlua_pushnil(rbxlua_State);
        }
        rbxlua_setfield(rbxlua_State, -2, Key.c_str());
        return 0;
    }
    case LUA_TTABLE: {
        lua_getfield(L, -1, "__address");
        if (!lua_isnil(L, -1)) {
            int inst = lua_tonumber(L, -1);
            lua_pop(L, 1);
            int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
            *userdata = inst;
            *(userdata + 1) = 0;
            rbxlua_getfield(rbxlua_State, -10000, "Object");
            rbxlua_setmetatable(rbxlua_State, -2);
            rbxlua_setfield(rbxlua_State, -2, Key.c_str());
        }
        else {
            lua_pop(L, 1);
        }
        return 0;
    }
    }
}

static int SpawnThread(lua_State *L) {
    lua_State* nL = lua_newthread(L);
}

static int Tick(lua_State *L) {
    lua_pushnumber(L, std::time(0));
    return 1;
}

void Init(lua_State* L) {
    lua_newtable(L);
    lua_pushcfunction(L, Index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, NewIndex);
    lua_setfield(L, -2, "__newindex");
    lua_setglobal(L, "robject");

    PushInstance(L, DataModel);
    lua_setglobal(L, "game");

    PushInstance(L, Workspace);
    lua_setglobal(L, "workspace");

    PushInstance(L, Workspace);
    lua_setglobal(L, "Workspace");

    lua_pushcfunction(L, Print);
    lua_setglobal(L, "print");

    lua_pushcfunction(L, Warn);
    lua_setglobal(L, "warn");

    lua_pushcfunction(L, PrintIdentity);
    lua_setglobal(L, "printidentity");

    lua_newtable(L);
    lua_pushcfunction(L, CreateInstance);
    lua_setfield(L, -2, "new");
    lua_setglobal(L, "Instance");

    lua_pushcfunction(L, Tick);
    lua_setglobal(L, "tick");

}

void SetContext() {
    while (true) {
        *(DWORD*)0x163AF38 = 7;
        Sleep(100);
    }
}

void Main() {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetContext, 0, 0, 0);
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    SetConsoleTitleA("wl0der | by Louka @ V3rmillion");
    Memcheck_Disable();
    ScriptContextVftable = 0xF754CC;
    ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
    DataModel = GetParent(ScriptContext);
    Workspace = FindFirstClass(DataModel, "Workspace");
    Players = FindFirstClass(DataModel, "Players");
    Lighting = FindFirstClass(DataModel, "Lighting");
    PlayerName = GetName(GetLocalPlayer(Players));
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)retcheck, (PBYTE)patchedret);
    DetourTransactionCommit();
    std::cout << "wl0der | by Louka @ V3rmillion\n\n";

    rbxlua_State = rbxlua_openstate(ScriptContext);

    while (true) {
        std::string i = Input();
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        Init(L);
        try {
            int err = luaL_dostring(L, i.c_str());
            if (err) {
                const char* errmsg = lua_tostring(L, -1);
                std::string err = "ERROR: ";
                err += errmsg;
                std::cout << err.c_str() << std::endl;
                lua_pop(L, 1);
            }
        }
        catch (std::exception e) {
            std::string err = "C ERROR: ";
            err += e.what();
            std::cout << err.c_str() << std::endl;
        }
        lua_close(L);
    }
}