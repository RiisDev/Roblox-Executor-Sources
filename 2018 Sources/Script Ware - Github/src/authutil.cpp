#include "gui.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include <sstream>
#include <ostream>
#include <istream>
#include <fstream>

std::string get_hwid()
{
	std::string ss = "Err_StringIsNu";

	UCHAR sz_filesys[255], sz_volname_buff[255];
	DWORD dw_serial;
	DWORD dw_mfl;
	DWORD dw_sys_flags;
	int error = 0;

	bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)sz_volname_buff, 255, &dw_serial, &dw_mfl, &dw_sys_flags, (LPTSTR)sz_filesys, 255);

	if (!success) {
		ss = "Err_Not_Elevated";
	}

	std::stringstream error_stream;
	error_stream << dw_serial;
	return std::string(error_stream.str());
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string urlencode(const std::string & s_src)
{
	const char dec2hex[16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *)s_src.c_str();
	const int src_len = s_src.length();
	unsigned char * const pstart = new unsigned char[src_len * 3];
	unsigned char * pend = pstart;
	const unsigned char * const src_end = pSrc + src_len;

	for (; pSrc < src_end; ++pSrc)
	{
		if (*pSrc >= 48 && *pSrc <= 57 || *pSrc >= 65 && *pSrc <= 90 || *pSrc >= 97 && *pSrc <= 122)
			*pend++ = *pSrc;
		else
		{
			// escape this char
			*pend++ = '%';
			*pend++ = dec2hex[*pSrc >> 4];
			*pend++ = dec2hex[*pSrc & 0x0F];
		}
	}

	std::string sresult((char *)pstart, (char *)pend);
	delete[] pstart;
	return sresult;
}

void to_clipboard(HWND hwnd, const std::string &s) {
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

std::vector<std::string> split_string(const std::string &str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string tok;

	while (std::getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

BOOL is_wow64()
{
	BOOL b_is_wow64 = FALSE;

	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fn_is_wow64_process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fn_is_wow64_process)
	{
		if (!fn_is_wow64_process(GetCurrentProcess(), &b_is_wow64))
		{
			return FALSE; //doesnt support some os therefore will be just assume the os is shit and unable to run script-ware nonetheless
		}
	}
	return b_is_wow64;
}