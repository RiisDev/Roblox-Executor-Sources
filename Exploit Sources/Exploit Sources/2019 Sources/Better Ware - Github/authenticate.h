#pragma once
#include "gui.h"
#include "VMProtectSDK.h"
#include "AES256.hpp"
#include "base64.h"
#include "misc.h"
#include "xorstring.h"
#include "xorstring.h"

bool cantaccess = false;
bool loadedscr = false;

bool is_original = false;
bool is_lite = false;
bool is_extended = false;

//#pragma comment (lib, "curl/libcurl_a_debug.lib")

DWORD authorize(const std::string &username, const std::string &password) {
	VMProtectBeginUltra("main auth");

	/* compile time string encryption, makes these strings invisible to decompilers */
	return 0;
	VMProtectEnd();
}

void init_auth() {
	EnableWindow(password_field, false);
	EnableWindow(username_field, false);
	EnableWindow(submit, false);

	int p_length = GetWindowTextLength(password_field) + 1;
	GetWindowText(password_field, (LPSTR)pass_buffer, p_length);

	int u_length = GetWindowTextLength(username_field) + 1;
	GetWindowText(username_field, (LPSTR)user_buffer, u_length);

	authorize(user_buffer, pass_buffer);
}

/* validate the folder that holds the username and encoded password */
void check_authfolder()
{
	struct stat info;

	auto pathname = (char*)"C:\\SW_AUTH";

	if (stat(pathname, &info) != 0)
	{
		cantaccess = true;
	}

	else if (info.st_mode & S_IFDIR)
	{
		loadedscr = true;
	}

	if (cantaccess)
	{
		CreateDirectory(pathname, NULL);

		if (!loadedscr)
			MessageBox(NULL, "Script-Ware was unable to aquire your username and password from its location.", "Script-Ware", MB_ICONERROR | MB_OK);
	}
}