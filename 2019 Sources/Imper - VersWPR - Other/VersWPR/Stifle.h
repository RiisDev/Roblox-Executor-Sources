#pragma once
#include <Windows.h>
#include <Map>
#include <String.h>
#include <STDInt.h>
#include <vector>
#include <intrin.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <xstring>
#include <vector>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

void antiCheck()
{
	__try {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DebuggerCheck, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AntiDump, NULL, NULL, NULL);
	}
	_except(EXCEPTION_EXECUTE_HANDLER) {
		Sleep(1);
	}
}