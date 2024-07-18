#pragma once
#include <cstddef>
#include <memory>
#include <string>

#include "curl/curl.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"curl/libcurl_a.lib")
#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Wldap32.lib")

std::string protectfilename(const char* filename) {
    std::string workstr = filename;
    for (int idx = 0; idx < workstr.length(); idx++) {
        if (!isalnum(workstr[idx]) && workstr[idx] != '-' && workstr[idx] != '_' && workstr[idx] != '.' && workstr[idx] != ' ' && workstr[idx] != '\0') {
            workstr[idx] = '_';
        }
    }
    return workstr;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

const char* workspacepath() {
    char path[MAX_PATH];
    HMODULE hm = NULL;

    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&protectfilename, &hm);
    GetModuleFileName(hm, path, sizeof(path));

    std::string workstr = path;
    replace(workstr, "csapi.dll", "\\");

    return workstr.c_str();
}

namespace
{
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
}

namespace Tools
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
    const char* FormatString(const char* fmt, ...)
    {
        char buffer[2048];

        va_list _ArgList;
        __crt_va_start(_ArgList, fmt);

#pragma warning(push)
#pragma warning(disable: 4996) // Deprecation
        _vsprintf_l(buffer, fmt, NULL, _ArgList);
#pragma warning(pop)

        __crt_va_end(_ArgList);

        return buffer;
    }

}