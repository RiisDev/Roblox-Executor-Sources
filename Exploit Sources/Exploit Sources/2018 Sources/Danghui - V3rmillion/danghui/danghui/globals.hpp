#pragma once

#pragma region Danghui Addresses
#define DH_NEWLSTR 0x10D990
#define DH_VGETTAB 0x110490
#define DH_VSETTAB 0x1106E0
#define DH_NEWCCLO 0x10B650
#define DH_LUACALL 0x111F20
#define DH_LUARITH 0x106A90
#define DH_XORCONS 0xFEE060
#define DH_NEWTHRE 0x111C20
#define DH_LUARESU 0x112BC0
#pragma endregion

unsigned int danghui_rbase();
unsigned int danghui_grabscriptcontext();
unsigned int danghui_grabstate(unsigned int scriptcontext, int idx);
unsigned int r_lua_newthread(unsigned int rL);
extern DWORD int3breakpoint;
extern DWORD DH_SCRIPTC;