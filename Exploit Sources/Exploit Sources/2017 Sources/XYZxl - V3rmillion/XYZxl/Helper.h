#pragma once
#define HELPER_H 1

#include <Windows.h>
#include <stdio.h>
#include <WinInet.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

// athread

class athread {
private:
	HANDLE thread;
	int threadID;

public:
	athread(void* func, void* args);
	int wait();
	HANDLE getThread();
	int getExitCode();
	int getThreadId();
};

athread::athread(void* func, void* args) {
	thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)func, args, 0, 0);
}

HANDLE athread::getThread() {
	return thread;
}

int athread::getExitCode() {
	int exitCode = 0;
	if (thread) {
		GetExitCodeThread(thread, (LPDWORD)&exitCode);
		return exitCode;
	}

	return exitCode;
}

int athread::getThreadId() {
	return threadID;
}

int athread::wait() {
	DWORD exitCode;

	if (thread) {
		DWORD result;

		do {
			result = WaitForSingleObject(thread, 0);

			if (result == WAIT_OBJECT_0) {
				GetExitCodeThread(thread, &exitCode);
				break;
			}

		} while (1);
	}

	return exitCode;
}