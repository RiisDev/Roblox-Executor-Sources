#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class ThreadFunctions {
public:
	HANDLE Thread;
	DWORD ID;
	std::string ThreadName;

	ThreadFunctions(HANDLE thread, std::string name, DWORD id = 0) {
		this->Thread = thread;
		this->ID = id;
		this->ThreadName = name;
	}

	ThreadFunctions() {
		this->Thread = NULL;
		this->ID = NULL;
		this->ThreadName = "";
	}

	DWORD WaitForThreadReturn() {
		WaitForSingleObject(Thread, INFINITE);
		return GetThreadStatus();
	}

	bool WaitForThreadReturn(int timeout) {
		DWORD result = WaitForSingleObject(Thread, timeout);
		while (result != WAIT_TIMEOUT) { Sleep(1); result = WaitForSingleObject(Thread, timeout); }
		return GetThreadStatus();
	}

	void Terminate() {
		TerminateThread(Thread, 0);
	}

	void Terminate(int code) {
		TerminateThread(Thread, code);
	}

	void Pause() {
		SuspendThread(Thread);
	}

	void Resume() {
		ResumeThread(Thread);
	}

	void Suspend() {
		SuspendThread(Thread);
	}

	DWORD GetThreadStatus() {
		DWORD EC;
		GetExitCodeThread(Thread, &EC);
		return EC;
	}
};

std::vector<ThreadFunctions> ThreadManagerThreads;

class ThreadManager {
public:
	static int ThreadCount;

	static void TerminateAllThreadManagerThreads() {
		for (int i = 0; i < ThreadManagerThreads.size(); i++) {
			if (ThreadManagerThreads[i].ID != GetCurrentThreadId()) {
				ThreadManagerThreads[i].Terminate();
			}
		}
	}

	static void TerminateThread(std::string thread_name) {
		for (int i = 0; i < ThreadManagerThreads.size(); i++) {
			if (ThreadManagerThreads[i].ThreadName == thread_name)
				ThreadManagerThreads[i].Terminate();
		}
	}

	static void TerminateThread(ThreadFunctions thread) {
		thread.Terminate();
	}


	static void SuspendThread(std::string thread_name) {
		for (int i = 0; i < ThreadManagerThreads.size(); i++) {
			if (ThreadManagerThreads[i].ThreadName == thread_name)
				ThreadManagerThreads[i].Suspend();
		}
	}

	static void SuspendThread(ThreadFunctions thread) {
		thread.Suspend();
	}

	static ThreadFunctions CreateNewThread(void *func, std::string name, void *argument = 0) {
		DWORD id;
		HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)func, argument, 0, &id);
		if (name == "Thread") {
			name = ("Thread#" + std::to_string(id)).c_str();
		}
		ThreadFunctions nt = ThreadFunctions(thread, name, id);
		ThreadManagerThreads.push_back(nt);
		return nt;
	}

	static ThreadFunctions CreateNewThread(LPTHREAD_START_ROUTINE func, std::string name, void *argument = 0) {
		DWORD id;
		ThreadFunctions nt;
		HANDLE thread = CreateThread(0, 0, func, argument, 0, &id);
		if (name == "Thread") {
			name = ("Thread#" + std::to_string(id));
		}
		nt = ThreadFunctions(thread, name, id);
		ThreadManagerThreads.push_back(nt);
		return nt;
	}

	static ThreadFunctions AddThread(HANDLE thread, DWORD id, std::string name) {
		ThreadFunctions nt;
		if (id == 0) {
			id = -1;
			name = ("Thread#" + std::to_string(ThreadCount++));
		}
		if (name == "Thread") {
			name = ("Thread#" + std::to_string(id));
		}
		nt = ThreadFunctions(thread, name, id);
		ThreadManagerThreads.push_back(nt);
		return nt;
	}
};