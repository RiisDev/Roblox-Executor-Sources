#include "BasicMemory.h"
#include "SignatureScanner.h"

#ifndef guard
#define guard

unsigned long location_scriptcontextvftable = *(DWORD*)(SigScan("\xC7\x03\x00\x00\x00\x00\xC7\x43\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\xC7\x43\x00\x00\x00\x00\x00\xF0\xFF\x05", "xx????xx?????xx?????xx?????xxx") + 0x2);
unsigned long location_RETFUNC = ASLR(0x15F0140);
unsigned long location_RETFLAG1 = ASLR(0x15F0170);
unsigned long location_RETFLAG2 = ASLR(0x15F353C);
unsigned long location_RawrJZ = ASLR(0x74BED7);
unsigned long location_RawrFlag = ASLR(0x15F0170);
unsigned long location_getfield = ASLR(0x7527A0);
/*Name: lua_getfield | String: __type | Location: 3 lines below string*/
unsigned long location_setfield = ASLR(0x7544B0); //
/*Name: lua_setfield | String: _VERSION | Location: 3 lines below string*/
unsigned long location_pcall = ASLR(0x753590);
/*Name: lua_pcall | String: *** Value not found *** | Location: 4 lines above string*/
unsigned long location_pushvalue = ASLR(0x753C10);
/*Name: lua_pushvalue | String: name conflict for module '%s' | Location: 9 lines under string*/
unsigned long location_pushnumber = ASLR(0x753AC0);
/*Name: lua_pushnumber | String: huge | Location: 1 line above string (Press CTRL+T to search again)*/
unsigned long location_pushstring = ASLR(0x753B50);
/*Name: lua_pushstring | String: __namecall | Location: 2 lines under string*/
unsigned long location_pushcclosure = ASLR(0x7536E0);
unsigned long location_next = ASLR(0x756AC0);
unsigned long location_tolstring = ASLR(0x7581B0);
unsigned long location_identity = ASLR(0x5AEAC0);
/*Name: Identity | String: Current identity is %d | Location: 2 lines above string*/

#endif