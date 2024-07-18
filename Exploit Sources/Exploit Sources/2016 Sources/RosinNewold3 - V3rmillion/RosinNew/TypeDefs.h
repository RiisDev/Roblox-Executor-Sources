#pragma once

#include <Windows.h>
#include <iostream>

namespace Rosin {
	typedef bool(__thiscall *spl)(int self);
	typedef void(__thiscall *sfs)(int self, float c);
	typedef void(__thiscall *r)(int self, DWORD* r);
	typedef void(__thiscall *s)(int self, int newparent);
	typedef void(__thiscall *clr)(int self);
	typedef void(__thiscall *ripchar)(int* self, bool nekkid);
	typedef void(__thiscall *BreakJ)(int self);
	typedef void(__thiscall *PSRR)(int self, int psr);
	//typedef void(__thiscall *PSRRR)(int* self, int* psr);
	typedef void(__thiscall *respawn)(int self);
	typedef void(__thiscall *settime)(int self, double time);
	typedef void(__thiscall *setjump)(int self, int b);

	typedef void(__thiscall *SoundIdd)(int self, std::string value); //SoundIdHax
	typedef void(__thiscall *startSound)(int self);
	typedef void(__thiscall *stopSound)(int self);

	typedef int(__thiscall *clrContent)(int self, bool bl);

	typedef int(__thiscall *haexs)(int dis, const char* test);


	typedef void(__thiscall *setIName)(int self, std::string* value);

	typedef int(__thiscall *getPlrChar)(int self);

	typedef int(__thiscall *cloneDef)(int dis, int a2, int a3);
	typedef int(__cdecl    *cloneAft)(int a1, int a2);
	typedef struct Vector3 { float x, y, z; };

	typedef struct CFrame
	{
		float rotationmatrix[9];
		Vector3 position;
	};

	typedef int(__thiscall *SetCFrameD)(int self, CFrame* cfStruct);

	typedef CFrame*(__thiscall *GetCFrameD)(void* self);

	typedef void(__thiscall *SetColorD)(int self, Vector3* color);

	typedef void(__thiscall *loadCharDef)(int self, int b, std::string unk);
	typedef void(__thiscall *ReportP)(int Self, int Player, std::string Reason);

	typedef void(__thiscall *setUdim2)(int self, int xscale, int xoffset, int yscale, int yoffset);
	
	typedef void(__thiscall *numVal)(int self, __int64* val);
}