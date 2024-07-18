#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <WinInet.h>
#pragma comment(lib, "wininet")

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

inline std::string download_url(const std::string& url) {
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