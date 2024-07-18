#pragma once
#define CURL_STATICLIB

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <stdio.h>
#include <windows.h>

#include "../curl/curl.h"
#include <random>

#pragma comment(lib,"../curl/libcurl_a.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Wldap32.lib")

inline void init_http_lib()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t write_memory_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

inline const char* get(const char* _curl)
{
	CURL* curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(1);
	chunk.size = 0;

	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, _curl);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&chunk));
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		return nullptr;
	}

	curl_easy_cleanup(curl_handle);

	return chunk.memory;
}
