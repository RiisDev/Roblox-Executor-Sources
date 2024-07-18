#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet")

#pragma once

#include "stdafx.h"
#include "SkId5.h"
#include "Skid6.h"
#include "SkId4.h"
#include "SkId3.h"
#include "SkId2.h"
#include "SkId1.h"

using namespace std;
//Command pipe
DWORD WINAPI SkidPipe(PVOID lvpParameter)
{
	HANDLE hSkid;
	char SkIdEr[1024];
	DWORD SkIdRead;
	hSkid = CreateNamedPipe(TEXT("\\\\.\\pipe\\IceSkidCmd"),//IceSkidCmd is the name of the command pipe
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hSkid != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hSkid, NULL) != FALSE)
		{
			while (ReadFile(hSkid, SkIdEr, sizeof(SkIdEr) - 1, &SkIdRead, NULL) != FALSE)
			{
				SkIdEr[SkIdRead] = '\0';
				try {
					ExecuteSkId(SkIdEr);
				}
				catch (std::exception e) {
					MessageBox(NULL, e.what(), "SKID ICE - Uh Oh", MB_OK);
				}
				catch (...) {
					MessageBox(NULL, "An Unhandled Error Has Occured!", "SKID ICE- Uh Oh", MB_OK);
					cout << "ERROR: An Unexpected Error Has Occured!" << endl;
				}
			}
		}
		DisconnectNamedPipe(hSkid);
	}
}
//Lua C pipe
DWORD WINAPI SkidCPipe(PVOID lvpParameter)
{
	HANDLE hSkid;
	char SkIdEr[1024];
	DWORD SkIdRead;
	hSkid = CreateNamedPipe(TEXT("\\\\.\\pipe\\IceSkidC"),//IceSkidC is the name of the lua c pipe
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hSkid != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hSkid, NULL) != FALSE)
		{
			while (ReadFile(hSkid, SkIdEr, sizeof(SkIdEr) - 1, &SkIdRead, NULL) != FALSE)
			{
				SkIdEr[SkIdRead] = '\0';
				try {
					SkIdC(SkIdEr);
				}
				catch (std::exception e) {
					MessageBox(NULL, e.what(), "SKID ICE - Uh Oh", MB_OK);
				}
				catch (...) {
					MessageBox(NULL, "An Unhandled Error Has Occured!", "SKID ICE- Uh Oh", MB_OK);
					cout << "ERROR: An Unexpected Error Has Occured!" << endl;
				}
			}
		}
		DisconnectNamedPipe(hSkid);
	}
}

int SKid() {
	CreateThread(NULL, 0, SkidPipe, NULL, 0, NULL);//Create new thread for command pipe
	CreateThread(NULL, 0, SkidCPipe, NULL, 0, NULL);//Create new thread for lua c pipe
	//Auth(); //uncomment this function to enable the whitelist duh
	SKID();//Scan lua state
	MessageBox(NULL, "Ice for SKIDS has loaded Correctly!\nSKID now you can use Ice!\nUpdated and improved by rakion99\nIce Made by Josh()", "Ice-Skid Version", MB_OK | MB_TOPMOST);//Messagebox so we can know when scan finished
	return 0;
}