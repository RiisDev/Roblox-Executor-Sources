#pragma once
#define WIN32_LEAN_AND_MEAN
#include "../commons.h"
#include "xorstr.h"
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/modes.h>
#include <intrin.h>
#include <iphlpapi.h>

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

class Security {
private:
    inline void get_file(const char* dllName, const char* fileName, char* buffer,
        int bfSize)
    {
        if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
            char* pathEnd = strrchr(buffer, '\\');
            strcpy(pathEnd + 1, fileName);
        } else {
            *buffer = 0;
        }
    }
    inline std::string replace_all(std::string subject, const std::string& search,
        const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

public:
    char* version = "2.9.0";
    bool authenticated = true;

public:
    inline std::string download_url(const char* URL)
    {
        std::string result {};
        HINTERNET handle = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT,
            NULL, NULL, NULL);
        HINTERNET url;
        if (handle) {
            url = InternetOpenUrlA(handle, URL, NULL, NULL, NULL, NULL);
            if (url) {
                char buffer[2000];
                DWORD bytesRead;
                do {
                    InternetReadFile(url, buffer, 2000, &bytesRead);
                    result.append(buffer, bytesRead);
                    memset(buffer, 0, 2000);
                } while (bytesRead);
                InternetCloseHandle(handle);
                InternetCloseHandle(url);
                return replace_all(result, "|n", "\r\n");
            }
        }
        InternetCloseHandle(handle);
        return replace_all(result, "|n", "\r\n");
    }

    inline bool check_version()
    {
        return true;
    }

    inline bool authenticate(char* username, char* password)
    {
        return true;
    }
};

extern Security security;