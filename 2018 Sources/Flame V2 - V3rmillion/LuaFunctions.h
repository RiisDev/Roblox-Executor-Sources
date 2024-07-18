#pragma once
#include "Data.h"

/*
int Chatted(int L) {

	CallCheck();
	const char* message = (const char*)ToString(L, -2);
	std::string chatMsg = message;

	if (chatMsg.substr(0, 1) == Prefix) {
		chatMsg = chatMsg.substr(1, std::string::npos);
		RunCommand(chatMsg);
	}
	return 0;
}

void TP_Function(int N) {
	CallCheck();
	try {
	}
	catch (...) {
		EmptyStack(L);
	}
}

void Destory_Function(int N) {
		CallCheck();
		try {
		GetService(L, "Players");
		GetField(L, -1, "LocalPlayer");
		GetField(L, -1, "GetMouse");
		PushValue(L, -2);
		Pcall(L, 1, 1, 0);
		GetField(L, -1, "Target");
		GetField(L, -1, "Remove");
		PushValue(L, -2);
		Pcall(L, 1, 0, 0);
		}
		catch (...) {
			EmptyStack(L);
		}
}


*/

static std::string CheckFiltering(bool status)
{
	return status ? "Filtering is enabled." : "Filtering is disabled.";
}

inline void NotifyRaw(std::string Text) {
	try {
		GetGlobal(L, "game");
		GetField(L, -1, "GetService");
		PushValue(L, -2);
		PushString(L, "CoreGui");
		Pcall(L, 2, 1, 0);
		GetField(L, -1, "RobloxGui");
		GetField(L, -1, "SendNotification");
		GetField(L, -1, "Fire");
		PushValue(L, -2);
		PushString(L, "Flame");
		PushString(L, Text.c_str());
		PushString(L, "");//rbxassetid://766147453
		PushNumber(L, 5);
		Pcall(L, 5, 0, 0);
	}
	catch (...) {
		EmptyStack(L);
	}
}