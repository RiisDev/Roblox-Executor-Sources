#pragma once
//getfield
unsigned long getSkId = SKID(0x7307e0);
//settop
unsigned long SkIdtop = SKID(0x7328d0);
//pushstring
unsigned long pushSkId = SKID(0x731b60);
//pushvalue
unsigned long SkIdvalue = SKID(0x731c20);
//call
unsigned long SkIds = SKID(0x730180);
//setfield
unsigned long setSkId = SKID(0x7324f0);
//pushnumber
unsigned long hOwMaNySkIdS = SKID(0x731ad0);
//contextlevel/identity
unsigned long SKIDLEVEL = SKID(0x5a7260);
//ScriptContextVFTable
unsigned long SkIdTab = SKID(0x6b898e);
//getmetatable
unsigned long DOGGO = SKID(0x730ad0);
//pcall
unsigned long pSkId = SKID(0x7315a0);
//rawrjz
unsigned long miaujz = SKID(0x729f17);

int Skidtest;
int SkidState;
int skid = 220;

void SKIDD() {
	using namespace std;
	DWORD SkIdT = *(DWORD*)(SkIdTab + 0x2);
	Skidtest = SkId::ScanforSkid((char*)&SkIdT);
	//luastate
	SkidState = Skidtest + skid - *(DWORD*)(Skidtest + skid);
}