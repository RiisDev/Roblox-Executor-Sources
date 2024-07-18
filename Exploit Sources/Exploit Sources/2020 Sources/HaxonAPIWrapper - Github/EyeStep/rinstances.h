#pragma once
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include "eyestep_util.h"

using namespace eyestep;
using namespace eyestep::util;

// there will be a release soon for my Java program
// that auto-updates every vftable offset.
namespace instances {
	int replicator = 0;
	int networkclient = 0;
	int game = 0;
	int script_context = 0;
	int workspace = 0;
	int players = 0;

	namespace offsets {
		BYTE name = 40;
		BYTE parent = 52;
		BYTE children_start = 44;
		BYTE children_end = 4;
	}

	// not compatible for external applications
	std::string getClass(int i){ return std::string((const char*)(*(int(**)(void))(*(DWORD*)i + 16))()); }
	
	std::string getName(int i){ return readstring(i + offsets::name); }
	int	getParent(int i){ return readi(i + offsets::parent); }

	std::vector<int>getChildren(int i) {
		std::vector<int> children;
		int children_start = readi(i + offsets::children_start);
		int children_end = readi(children_start + offsets::children_end);
		for (int j = readi(children_start); j != children_end; j += 8)
			children.push_back(readi(j));
		return children;
	}

	int findFirstChild(int i, std::string name) {
		for (int v : getChildren(i))
			if (getName(v).find(name) != std::string::npos)
				return v;
		return 0;
	}

	int findFirstClass(int i, std::string classname) {
		for (int v : getChildren(i))
			if (getClass(v).find(classname) != std::string::npos)
				return v;
		return 0;
	}

	// automatically get all vftables
	bool fetch() {
		if (!instances::replicator) {
			printf("REPLICATOR!!!!!!!\n");
			std::vector<int> results = eyestep::util::scan("74??8B??????000085??0F85", ".x.xxx...x..", 0, 0, 1);
			if (results.size() != 0) {
				int RakPeerHook = results[0];
				BYTE count = 0;
				while (readi(RakPeerHook) != 0x57F18B56 && count <= 0xFF) RakPeerHook--, count++;
				if (count < 0xFF) {
					instances::replicator = eyestep::util::debug(RakPeerHook, eyestep::reg_32::ecx);
				}
				printf("REEEEEEEEEE!!!\n");
			}
		}

		instances::networkclient = getParent(instances::replicator);
		if (instances::networkclient) {
			instances::game = getParent(instances::networkclient);

			if (instances::game) {
				instances::script_context = findFirstChild(instances::game, "Script Context");
				return true;
			}
			else {
				printf("Failed to find Game\n");
			}
		}
		else {
			printf("Failed to find NetworkClient\n");
		}

		return false;
	}
}

// use once for the active lua state from script context
int get_lua_state(int script_context) {
	int lua_state = 0;
	//std::vector<int>result_list = eyestep::util::scan("B9000000000F45CA", "........", 0, 0, 1);
	std::vector<int>result_list = eyestep::util::scan("B9000000000F45CA", "........", 0, 0, 1);
	if (result_list.size() == 0) {
		printf("No results for lua state\n");
	} else {
		int at = result_list[0] + 8;
		BYTE data[64];
		BYTE size = 0;
		BYTE b[2];

		while (size < 64) {
			readb(at, &b, 2);
			if (b[0] == 0x56 && b[1] == 0xE8) break;
			data[size++] = readb(at++);
		}

		int func = valloc(0, 128);
		int var = func + 124;
		BYTE* bvar = convert::dwtopb(var);
		BYTE func_data1[] = { 0x55, 0x8B, 0xEC, 0x56, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF, 0x52, 0x51, 0xBA, 0x1, 0, 0, 0, 0xB9, 0, 0, 0, 0 };
		BYTE func_data2[] = { 0x8B, 0xC6, 0xA3, bvar[0], bvar[1], bvar[2], bvar[3], 0x59, 0x5A, 0x5E, 0x5D, 0xC2, 0x4, 0 };
		*(int*)(func_data1 + 5) = script_context;
		delete bvar;

		write(func, &func_data1, 21);
		write(func + 21, &data, size);
		write(func + 21 + size, &func_data2, 14);

		if (DLL_MODE) {
			__asm push 0 // so that we can also use CreateRemoteThread
			__asm call func
		} else {
			CreateRemoteThread(handle, NULL, 128, (LPTHREAD_START_ROUTINE)func, 0, 0, 0);
			WaitForSingleObject(handle, 100);
		}
		lua_state = readi(var);
		vfree(func, 128);
	}
	return lua_state;
}
