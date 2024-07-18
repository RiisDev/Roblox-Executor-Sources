#include "helper.h"

athread::athread(void* func, void* args) {
	thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func, args, NULL, (LPDWORD)&threadID);
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

void GetFile(const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle("SGExploit V2.dll"), buffer, bfSize);
	char* pathEnd = strrchr(buffer, '\\');
	strcpy(pathEnd + 1, fileName);
}

long int GetFileSize(FILE* ifile) {
	long int fsize = 0;
	long int fpos = ftell(ifile);
	fseek(ifile, 0, SEEK_END);
	fsize = ftell(ifile);
	fseek(ifile, fpos, SEEK_SET);

	return fsize;
}

std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrl(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			return rtn;
		}
	}
	InternetCloseHandle(interwebs);
	return rtn;
}

std::vector<std::string> GetArguments(std::string input) {
	std::vector<std::string> rtn;

	if (input[0] == ' ') {
		input = input.substr(1);
	}

	BYTE size = input.size();
	DWORD pos1 = 0;

	for (int i = 0; i < size; ++i) {
		if (input[i] == ' ') {
			rtn.push_back(input.substr(pos1, i - pos1));
			pos1 = i + 1;
		}
		else if (i == size - 1) {
			rtn.push_back(input.substr(pos1, i - pos1 + 1));
			pos1 = i + 1;
		}
	}

	return rtn;
}
 
