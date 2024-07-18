#ifndef HELPER_H
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

// other

void GetFile(const char* fileName, char* buffer, int bfSize);

long int GetFileSize(FILE* ifile);

std::string DownloadURL(const char* URL);

std::vector<std::string> GetArguments(std::string input);

#endif
