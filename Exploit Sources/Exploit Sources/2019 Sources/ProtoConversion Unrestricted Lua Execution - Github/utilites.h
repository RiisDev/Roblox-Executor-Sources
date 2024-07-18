#pragma once
#include <Windows.h>
#include <WinInet.h>
#include <string>

#pragma comment(lib, "wininet")

inline std::string download_url(const std::string &url);
std::string replace_all(std::string subject, const std::string& search, const std::string& replace);

void set_encode_key(int rl, DWORD* key);
