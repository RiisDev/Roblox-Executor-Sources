/*
  [#] Decompiled Source Code 'Cryztal' wrapper
  [#] Credits to 'UnKnown'!
*/

#include "main.h"


#define RawUnprotected 0x72D3F7

namespace Retcheck {

  DWORD unprotect(DWORD addr)

  {

    BYTE * tAddr = (BYTE * ) addr;

    do {

      tAddr += 0x10;

    } while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

    DWORD funcSz = tAddr - (BYTE * ) addr;

    PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (nFunc == NULL)

      return addr;

    memcpy(nFunc, (BYTE * ) addr, funcSz);

    DWORD pos = (DWORD) nFunc;

    BOOL valid = false;

    do {

      if ( * (BYTE * ) pos == 0x72 && * (BYTE * )(pos + 0x2) == 0xA1 && * (BYTE * )(pos + 0x7) == 0x8B) {

        memcpy((void * ) pos, "\xEB", 1);

        DWORD cNFunc = (DWORD) nFunc;

        do {

          if ( * (BYTE * ) cNFunc == 0xE8)

          {

            DWORD tFunc = addr + (cNFunc - (DWORD) nFunc);

            DWORD oFunc = (tFunc + * (DWORD * )(tFunc + 1)) + 5;

            if (oFunc % 16 == 0)

            {

              DWORD rebFAddr = oFunc - cNFunc - 5;

              *(DWORD * )(cNFunc + 1) = rebFAddr;

            }

            cNFunc += 5;

          } else

            cNFunc += 1;

        } while (cNFunc - (DWORD) nFunc < funcSz);

        valid = true;

      }

      pos += 1;

    } while (pos < (DWORD) nFunc + funcSz);

    if (!valid) {

      VirtualFree(nFunc, funcSz, MEM_RELEASE);

      return addr;

    }

    return (DWORD) nFunc;

  }

}#include "main.h"


void open_lua_libs(lua_State * Ls) {

  lua_newtable(Ls);

  lua_pushcfunctioN(Ls, print);

  lua_setglobal(Ls, "print");

  lua_settop(Ls, 0);

  lua_newtable(Ls);

  lua_pushcfunctioN(Ls, warn);

  lua_setglobal(Ls, "warn");

  lua_settop(Ls, 0);

  lua_newtable(Ls);

  lua_pushcfunctioN(Ls, error);

  lua_setglobal(Ls, "error");

  lua_settop(Ls, 0);

  lua_newtable(Ls);

  lua_pushcfunctioN(Ls, pi);

  lua_setglobal(Ls, "printidentity");

  lua_settop(Ls, 0);

  lua_newtable(Ls);

  lua_pushcfunctioN(Ls, newinstance);

  lua_setfield(Ls, -2, "new");

  lua_setglobal(Ls, "Instance");

  lua_settop(Ls, 0);

}

void WrapLua(std::string source) {

  lua_State * luaS = luaL_newstate();

  lua_State * LuaS = lua_newthread(luaS);

  *(char * )(L - 0x14) = 6;

  luaL_openlibs(LuaS);

  open_lua_libs(LuaS);

  luaL_dostring(LuaS, source.c_str());

  lua_settop(LuaS, 0);

  lua_close(LuaS);

  settop(L, 0);

}#include <iostream>

#include <Windows.h>


#include <gay.h>


#include "Lua.h"


#include "lauxlib.h"


#include "lua2.h"


#include "luaconf.h"


#include "LuaWrapper.h"


#include "Metamethods.h"


using namespace std; // l a z y

typedef int(L);

int print(lua_State * Ls) {

  string args = lua_tolstring(Ls, -2);

  getglobal(L, "print");

  pushstring(args.c_str());

  pcall(L, 1, 0, 0);

}

int warn(lua_State * Ls) {

  string args = lua_tolstring(Ls, -2);

  getglobal(L, "warn");

  pushstring(args.c_str());

  pcall(L, 1, 0, 0);

}

int error(lua_State * Ls) {

  string args = lua_tolstring(Ls, -2);

  getglobal(L, "error");

  pushstring(args.c_str());

  pcall(L, 1, 0, 0);

}

int pi(lua_State * Ls) {

  string args = lua_tolstring(Ls, -2);

  getglobal(L, "printidentity");

  pcall(L, 0, 0, 0);

}

int newinstance(lua_State * Ls) {

  string args = lua_tolstring(Ls, -2);

  getglobal(L, "Instance");

  getfield(L, -1, "new");

  pushstring(args.c_str());

  pcall(L, 4, 1, 0);

  getglobal(L, "workspace");

  pushvalue(L, -2);

  setfield(L, -2, "Parent");

}#include "main.h"

using namespace std;

int main(int argc,
  const char * argv[]) {

  /* THREADS */

  CreateThread(0, NULL, ThreadProc, (LPVOID) L "Cryztal - Developed by Tactical BFG", NULL, NULL);

  Sleep(10);

  DWORD asdmemes;

  VirtualProtect((PVOID) & FreeConsole, 1, PAGE_EXECUTE_READWRITE, & asdmemes);

  *(BYTE * )( & FreeConsole) = 0xC3;

  AllocConsole();

  std::cout << "Done!" << endl;

  getservice(L, "Players");

  luacputime = time(NULL);

  getfield(L, -1, "LocalPlayer");

  getfield(L, -1, "Name");

  string userID = toLstring(L, -1, NULL);

  myplr = userID.c_str();

  wstring playername(userID.length(), L ' ');

  copy(userID.begin(), userID.end(), playername.begin());

  system("cls");

  copy(userID.begin(), userID.end(), playername.begin());

  cout << "Welcome to Cryztal, " << myplr << "Type 'mml' to load MakerModelLua Admin."
  endl;

  return 0;

}

void HookThread(void * entrypoint)

{

  // ya its the 32 bit support thing but still

  bool MessageOn = true;

  typedef int(__stdcall * MessageBoxKernel)(HWND, LPCSTR, LPCSTR, UINT);

  MessageBoxKernel MsgBoxK = (MessageBoxKernel) GetProcAddress(GetModuleHandleA("user32"), "MessageBoxA");

  DWORD size = MAX_PATH;

  HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());

  LPVOID mem = VirtualAllocEx(proc, NULL, size, MEM_COMMIT, PAGE_READWRITE);

  char pathbuff[MAX_PATH];

  GetModuleFileNameA(NULL, pathbuff, MAX_PATH);

  WriteProcessMemory(proc, mem, pathbuff, strlen(pathbuff), NULL);

  CreateRemoteThread(proc, NULL, NULL, (LPTHREAD_START_ROUTINE) entrypoint, mem, NULL, NULL);

  if (MessageOn == true)

  {

    VirtualFreeEx(proc, mem, size, MEM_FREE);

  } else

  {

    VirtualFreeEx(proc, mem, size, MEM_FREE);

  }

}

BOOL APIENTRY DllMain(HMODULE hModule,

  DWORD ul_reason_for_call,

  LPVOID lpReserved

)

{

  switch (ul_reason_for_call)

  {

  case DLL_PROCESS_ATTACH:

    inj_hModule = hModule;

    HookThread(main);

  case DLL_THREAD_ATTACH:

  case DLL_THREAD_DETACH:

  case DLL_PROCESS_DETACH:

    break;

  }

  return TRUE;

}
