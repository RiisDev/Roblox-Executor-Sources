#pragma once
#include "Addresses.h"
#include "Main.h"

#include <fstream>
#include <sstream>


std::string GetLocalPlayerName() {
	rlua_getglobal(rL, "game");
	rlua_getfield(rL, -1, "Players");
	rlua_getfield(rL, -1, "LocalPlayer");
	rlua_getfield(rL, -1, "Name");
	return rlua_tolstring(rL, -1, NULL);
}

bool GetFilteringStatus() {
	rlua_getglobal(rL, "workspace");
	rlua_getfield(rL, -1, "FilteringEnabled");
	return rlua_toboolean(rL, -1);
}

void GetFile(const char* dllName, const char* fileName, char*   buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}

int ReadFile(const std::string& path, std::string& out, unsigned char binary) {
	std::ios::openmode mode = std::ios::in;
	if (binary)
		mode |= std::ios::binary;

	std::ifstream file(path, mode);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		out = buffer.str();
		file.close();
		return 1;
	}

	file.close();
	return 0;
}

static int loadfile(lua_State *L) {
	if (lua_isstring(L, 1)) {
		printf("ToString\n");
		const char * nameoffile = lua_tostring(L, 1);
		printf("GetFile\n");
		char path[MAX_PATH];
		GetFile("Cyber.dll", "", path, MAX_PATH);

		std::string lyonisameganoob;
		lyonisameganoob += path;
		lyonisameganoob += "scripts\\";
		lyonisameganoob += nameoffile;

		printf(lyonisameganoob.c_str());
		printf("\nPath\n");
		string line;
		ifstream myfile(lyonisameganoob);
		std::string data;
		if (ReadFile(lyonisameganoob, data, 0)) {
			Eggsecute(data);
		}
		else {
			SingletonPrint(ErrorColors::Error, "That file was not found. Please try again.");
		}
	}

	else {
		//luaL_error(L, "Loadfile expected a string.");
		return 1;
	}

	return 0;
}


