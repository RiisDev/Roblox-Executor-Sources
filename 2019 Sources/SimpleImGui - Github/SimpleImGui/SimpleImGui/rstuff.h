#pragma once
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

typedef int(__cdecl *RPrint)(int, const char *, ...);
RPrint r_Print = (RPrint)x(0x4e0dc0);//Roblox print address

void RobloxPrint(const char * type, const char *message)//Some gay shit as a simple test
{
	if (type == "info")
	{
		r_Print(1, message);
	}
	else if (type == "warn")
	{
		r_Print(2, message);
	}
	else if (type == "error")
	{
		r_Print(3, message);
	}
	else if (type == "print")
	{
		r_Print(0, message);
	}
}