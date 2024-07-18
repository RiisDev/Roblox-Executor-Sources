#pragma once
#include <vector>


#define adr(x) (x - 0x400000 + (DWORD)GetModuleHandle(0))
#define ScriptContextAddress                            adr(0x0119E874)

#define ScriptContextWAddress							ScriptContextAddress - (DWORD)GetModuleHandle(0)
#define CallCheckAddress                                adr(0x155ADEC)
#define HackFlagAddress                                    adr(0x115DC50)
#define RReturnCheckFunction_address                    adr(0x49BB50)
#define RReturnFlag1_address                            adr(0x1563EE8)
#define RReturnFlag2_address                            adr(0x155DC2C)
#define callcheck() *(int*)((char*)CallCheckAddress + *(DWORD*)CallCheckAddress) = 0;
#define rblua_pushcfunction(L,f)	pushcclosure(L, (f), 0)