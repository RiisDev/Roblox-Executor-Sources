/*
util.cpp
Purpose: miscellaneous utility library

@author Austin J
@version 1.1 4/26/2016
*/

#ifndef UTIL_H
#define UTIL_H

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <TlHelp32.h>

#define DLURL_SUCCESS				0
#define DLURL_FAILED_REQUEST		1
#define DLURL_FAILED_CERT_QUERY		2
#define DLURL_FAILED_CERT_CHECK		3

// athread

namespace util {
	class athread {
	private:
		HANDLE thread;
		int threadID;

	public:
		athread(void* func, void* args);
		int terminate();
		int wait();
		HANDLE getThread();
		int getExitCode();
		int getThreadId();
		int running();
	};

	class timer {
	private:
		unsigned __int64 _starting_tick;
		unsigned __int64 _finish_tick;

	public:
		timer() { Reset(); }

		/* resets the timer */
		void Reset() {
			_starting_tick = 0;
			_finish_tick = 0;
		}

		/* starts the timer */
		void Start() {
			Reset();
			_starting_tick = GetTickCount64();
		}

		/* stops the timer and returns the elapsed time */
		double Stop() {
			_finish_tick = GetTickCount64();
			return GetElapsedTime();
		}

		/* returns elapsed time */
		double GetElapsedTime() {
			if (_starting_tick & _finish_tick)
				return (_finish_tick - _starting_tick) / 1000.0;

			return 0.0;
		}
	};

	template <typename T>
	class singleton {
	private:
		static T* _instance;
	public:
		static T* instance() {
			if (_instance)
				return _instance;

			_instance = new T;
			return _instance;
		}

		singleton(singleton const&) = delete;
		void operator=(singleton const&) = delete;
	};

	// other

	void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize);

	long int GetFileSize(FILE* ifile);

	int DownloadURL(const std::string& server, const std::string& path, const std::string& params, std::string& out, unsigned char useSSL, unsigned char dontCache, unsigned char direct);

	int ReadFile(const std::string& path, std::string& out, unsigned char binary);

	int WriteFile(const std::string& path, std::string data, unsigned char binary);

	std::vector<std::string> GetArguments(std::string input);

	int GetProcessByImageName(const char* imageName);

	std::string lowercase(std::string& input);

	std::wstring s2ws(const std::string& str);

	std::string ws2s(const std::wstring& wstr);

	std::string GetRawStringAtDelim(std::string input, int arg, char delim);

	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath);

	int GetEIP();

	void pause();

	namespace registry {
		int ReadString(HKEY key, const char* value, std::string& out);
	}
};

#endif