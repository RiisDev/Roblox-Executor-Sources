#pragma once

extern "C" {
#include "ImportantLua/lua.h"
#include "ImportantLua/lualib.h"
#include "ImportantLua/lauxlib.h"
#include "ImportantLua/ldo.h"
#include "ImportantLua/lfunc.h"
#include "ImportantLua/lmem.h"
#include "ImportantLua/lopcodes.h"
#include "ImportantLua/lstring.h"
#include "ImportantLua/lundump.h"
#include "ImportantLua/lvm.h"
}

// write hackflag to default its value - bypasses shutdown from deserialize.
// this isn't a permanent solution (i hope).						 //0x1528BF8
#define RESTORE_HACKFLAG()     { int flag = (int) GetModuleHandle(0) + 0x115DC50; \
                               *(int*)(flag) = 8; }
//0x114BB3C
// on ScriptContext's resume, the identity is taken from state-0x14
#define SET_IDENTITY(thread, identity) *(char*)(thread-0x14) = identity

#define RBX_UNRESTRICTED_IDENTITY 7

/***********/
extern int loadscript;
extern int ggs;
extern int deserialize;
extern int newthread;
extern int resume;
extern int setfield;
extern int spawn;

extern int unksub1;
extern int unksub2;
extern int precallflag;
/***********/
extern int rets[4];

//#define RESTORE_PRECALL_FLAG() *(int *)((char *)precallflag--
#define RESTORE_PRECALL_FLAG() *(int *)((char *)precallflag + *(int*)precallflag) = *(int *)((char *)precallflag + *(int*)precallflag) - 1
#define deserialize(state, code, chunkname, modkey) ((int(*)(int, std::string&, const char*, unsigned int))(deserialize))(state, code, chunkname, modkey)
#define spawn(state) ((int(*)(int))(spawn))(state)

#define obf_ptr(l,v)       *(int*)((int)l)=(int)(v)-((int)l)
#define deobf_ptr(l)       *(int*)(l)+(l)

void LocateRbxDefs();
int rbx_pushcfunction(int one, int two, int three);