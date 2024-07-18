// Hi
#include <Windows.h>
#include <iostream>
#include <string>

#define aslr(x) (x - 0x400000 + (DWORD)GetModuleHandle(0)) // rebase ofc.

DWORD ScriptContext;

typedef DWORD(__thiscall* ScriptPogger)(int* yes, const char* name, DWORD arg);
ScriptPogger r_grabScriptContext = (ScriptPogger)aslr(0x5DF530); // Updated fro 06/01/2021

void GetScriptContext()
{
  int use = 0;
  ScriptContext = r_grabScriptContext(&use, "ScriptContext", 0);
}

/*
to use this make sure to init "GetScriptContext", and use ScriptContext as your ScriptContext, makes a bit of sense that.

This never returned a valid lua state. 
*/

