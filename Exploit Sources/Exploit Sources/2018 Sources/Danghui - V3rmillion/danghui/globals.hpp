#pragma once

#define aslr(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))


#pragma region Danghui Addresses
#define DH_NEWLSTR aslr(0x72FE70)
#define DH_VGETTAB aslr(0x732E50)
#define DH_VSETTAB aslr(0x7330C0)
#define DH_NEWCCLO aslr(0x72DC10)
#define DH_LUACALL 0x111F20
#define DH_LUARITH aslr(0x728E70)
#define DH_XORCONS aslr(0x14CB820)
#define DH_NEWTHRE aslr(0x7347B0)
#define DH_LUARESU aslr(0x734AE0)
#pragma endregion
unsigned int danghui_rbase();
unsigned int danghui_grabscriptcontext();
unsigned int danghui_grabstate(unsigned int scriptcontext, int idx);
unsigned int r_lua_newthread(unsigned int rL);
extern DWORD int3breakpoint;
extern DWORD DH_SCRIPTC;