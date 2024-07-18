#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>

using namespace std;

std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

string replaceAll(string subject, const string& search,
    const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool endsWithCaseInsensitive(std::string mainStr, std::string toMatch)
{
    auto it = toMatch.begin();
    return mainStr.size() >= toMatch.size() &&
        std::all_of(std::next(mainStr.begin(), mainStr.size() - toMatch.size()), mainStr.end(), [&it](const char& c) {
        return ::tolower(c) == ::tolower(*(it++));
            });


}

std::string protectfilename(const char* filename) {
    std::string workstr = filename;

    for (int idx = 0; idx < workstr.length(); idx++) {
        if (!isalnum(workstr[idx]) && workstr[idx] != '-' && workstr[idx] != '_' && workstr[idx] != '.' && workstr[idx] != ' ' && workstr[idx] != '\0') {
            workstr[idx] = '_';
        }
    }

    return workstr;
}

std::size_t callback(
    const char* in,
    std::size_t size,
    std::size_t num,
    std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

HWND RobloxActive;

void IsTainedBypass()
{
    auto TaintBypass = GetModuleHandleA("WINTRUST.dll");

    DWORD CheckIsTainted = (DWORD)GetProcAddress(TaintBypass, "WinVerifyTrust");

    BYTE BypassTaintOPCodes[9] = { 85, 137, 229, 49, 192, 93, 194, 12, 0 };
    DWORD OldProtect, NewProtect;

    VirtualProtect((LPVOID)CheckIsTainted, sizeof(BypassTaintOPCodes), PAGE_EXECUTE_READWRITE, &OldProtect);
    for (int i = 0; i < 9; i++)
    {
        *(BYTE*)(CheckIsTainted + i) = BypassTaintOPCodes[i];
    }
    VirtualProtect((LPVOID)CheckIsTainted, sizeof(BypassTaintOPCodes), OldProtect, &NewProtect);
}

namespace GianTools
{
    std::string ReadWebsite(const char* URL)
    {
        CURL* curl = curl_easy_init();

        // Set remote URL.
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        // Don't bother trying IPv6, which would increase DNS resolution time.
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

        // Don't wait forever, time out after 10 seconds.
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);

        // Follow HTTP redirects if necessary.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Response information.
        int httpCode(0);
        std::unique_ptr<std::string> httpData(new std::string());

        // Hook up data handling function.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

        // Hook up data container (will be passed as the last parameter to the
        // callback handling function).  Can be any pointer type, since it will
        // internally be passed as a void pointer.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

        // Run our HTTP GET command, capture the HTTP response code, and clean up.
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if (httpCode == 200)
        {
            return *httpData;
        }
        else
        {
            "";
        }
    }
}

//const char* workspacepath() {
//    char path[MAX_PATH];
//    HMODULE hm = NULL;
//
//    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&protectfilename, &hm);
//    GetModuleFileName(hm, path, sizeof(path));
//
//    std::string workstr = path;
//    replace(workstr, "BytecodeConversion.dll", "workspace\\");
//
//    return workstr.c_str();
//}

