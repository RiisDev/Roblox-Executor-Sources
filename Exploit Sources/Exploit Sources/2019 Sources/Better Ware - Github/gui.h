#pragma once
#include "convert.h"

#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <WinInet.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#pragma comment(lib, "wininet.lib")


static void init_gui();
static void erase_peheaders();
static void stop_roblox_input();
static double time_took;

//sets the str argument as the label in the loading screen
void output_label(const char* str);

//outputs an RGB value string
void print_output(COLORREF col, const char * format, ...);

//sets progress bar to integer provided
void progress_bar(int val);

//returns the user name of the computer
DWORD get_pc_name();

//main init function
void initialize();

void open_main();
void auto_exec();
DWORD WINAPI main();
BOOL is_wow64();

static bool auth_success = false;
static bool auth_firsttime = false;

static const char* pass_buffer = new char[64];
static const char* user_buffer = new char[64];

static HWND output_field;
static HWND main_window;
static HWND main_load_window;
static HWND main_login_window;
static HWND script_field;
static HWND check_box;
static HWND debug_checkbox;
static HWND cmd_input_field;
static HWND execute;
static HWND clear;
static HWND open;
static HWND output;
static HWND progressbar;
static HWND submit;
static HWND username_field;
static HWND password_field;

static HWND versionlabel;
std::string get_hwid();
std::string replace_all(std::string subject, const std::string& search, const std::string& replace);
std::string urlencode(const std::string & s_src);
void to_clipboard(HWND hwnd, const std::string &s);
std::vector<std::string> split_string(const std::string &str, char delimiter);

inline std::string download_url(const std::string &url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all(rtn, "|n", "\r\n");
	return p;
}


static int read_file(const std::string& path, std::string& out, unsigned char binary) {
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
