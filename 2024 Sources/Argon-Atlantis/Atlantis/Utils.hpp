#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <map>

#include "XorStr.hpp"

#include <ShlObj.h>
#include <Wininet.h>
#include <TlHelp32.h>
#include <queue>

#pragma comment(lib, "wininet.lib")

#include "Compressor/include/xxhash.h"
#include "Compressor/include/zstd.h"


namespace Utils
{

	static inline bool is_errors(int code)
	{
		return (code >= 400);
	}

	static inline std::string write_error(int code)
	{
		switch (code)
		{
		case 100: return "continue";
		case 101: return "switching protocols";
		case 102: return "processing";
		case 103: return "early hints";

		case 200: return "ok";
		case 201: return "created";
		case 202: return "accepted";
		case 203: return "non-authoritative information";
		case 204: return "no content";
		case 205: return "reset content";
		case 206: return "partial content";
		case 207: return "multi-status";
		case 208: return "already reported";
		case 226: return "im used";

		case 300: return "multiple choices";
		case 301: return "moved permanently";
		case 302: return "found";
		case 303: return "see other";
		case 304: return "not modified";
		case 305: return "use proxy";
		case 307: return "temporary redirect";
		case 308: return "permanent redirect";

		case 400: return "bad request";
		case 401: return "unauthorized";
		case 402: return "payment required";
		case 403: return "forbidden";
		case 404: return "not found";
		case 405: return "method not allowed";
		case 406: return "not acceptable";
		case 407: return "proxy authentication required";
		case 408: return "request timeout";
		case 409: return "conflict";
		case 410: return "gone";
		case 411: return "length required";
		case 412: return "precondition failed";
		case 413: return "payload too large";
		case 414: return "uri too long";
		case 415: return "unsupported media type";
		case 416: return "range not satisfiable";
		case 417: return "expectation failed";
		case 422: return "unprocessable entity";
		case 423: return "locked";
		case 424: return "failed dependency";
		case 426: return "upgrade required";
		case 428: return "precondition required";
		case 429: return "too many requests";
		case 431: return "request header fields too large";
		case 451: return "unavailable for legal reasons";

		case 500: return "internal server error";
		case 501: return "not implemented";
		case 502: return "bad gateway";
		case 503: return "service unavailable";
		case 504: return "gateway time-out";
		case 505: return "http version not supported";
		case 506: return "variant also negotiates";
		case 507: return "insufficient storage";
		case 508: return "loop detected";
		case 510: return "not extended";
		case 511: return "network authentication required";

		default: return std::string();
		}
	}

	static auto compress_bytecode(std::string source) -> std::string
	{
		const auto data_size = source.size();
		const auto max_size = ZSTD_compressBound(data_size);
		auto buffer = std::vector<char>(max_size + 8);

		strcpy_s(&buffer[0], buffer.capacity(), xorstr_("RSB1"));
		memcpy_s(&buffer[4], buffer.capacity(), &data_size, sizeof(data_size));

		const auto compressed_size = ZSTD_compress(&buffer[8], max_size, source.data(), data_size, ZSTD_maxCLevel());
		if (ZSTD_isError(compressed_size))
			throw std::runtime_error(xorstr_("Failed to compress the bytecode."));

		const auto size = compressed_size + 8;
		const auto key = XXH32(buffer.data(), size, 42u);
		const auto bytes = reinterpret_cast<const uint8_t*>(&key);

		for (auto i = 0u; i < size; ++i)
			buffer[i] ^= bytes[i % 4] + i * 41u;

		return std::string(buffer.data(), size);
	}


	static std::string decompress_bytecode(std::string data)
	{
		const char bytecode_magic[] = "RSB1";
		uint8_t hash_bytes[4];
		std::memcpy(hash_bytes, &data[0], 4);

		for (auto i = 0u; i < 4; ++i)
		{
			hash_bytes[i] ^= bytecode_magic[i];
			hash_bytes[i] -= i * 41;
		}

		for (size_t i = 0; i < data.length(); ++i)
			data[i] ^= hash_bytes[i % 4] + i * 41;

		XXH32(data.data(), data.length(), 42);

		uint32_t data_size;
		std::memcpy(&data_size, &data[4], 4);

		std::vector<uint8_t> zaazaa(data_size);
		ZSTD_decompress(&zaazaa[0], data_size, &data[8], data.size() - 8);

		return std::string((char*)(&zaazaa[0]), data_size);
	}

	namespace Workspace
	{
		__forceinline auto initialize() -> void
		{
			auto path = std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-workspace");
			if (!std::filesystem::exists(path))
				std::filesystem::create_directory(path);
		}


		__forceinline auto get() -> std::filesystem::path
		{
			return std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-workspace");
		}
	}

	namespace AutoExecute
	{
		__forceinline auto initialize() -> void
		{
			auto path = std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-autoexec");
			if (!std::filesystem::exists(path))
				std::filesystem::create_directory(path);
		}


		__forceinline auto get() -> std::filesystem::path
		{
			return std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-autoexec");
		}
	}

	namespace Assets
	{
		__forceinline auto initialize() -> void
		{
			auto path = std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-assets");
			if (!std::filesystem::exists(path))
				std::filesystem::create_directory(path);
		}


		__forceinline auto get() -> std::filesystem::path
		{
			return std::filesystem::path(getenv("localappdata")) / xorstr_("pluto-assets");
		}
	}

	__forceinline std::string random_str(int length) {
		static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
		std::string result;
		result.resize(length);

		srand(time(NULL));
		for (int i = 0; i < length; i++)
			result[i] = charset[rand() % charset.length()];

		return result;
	}

	__forceinline static std::string replace_all(std::string subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	__forceinline static auto replace(std::string& str, const std::string& from, const std::string& to) -> bool
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}


	__forceinline static std::string download_string(std::string URL)
	{
		HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		HINTERNET urlFile;
		std::string rtn;
		if (interwebs)
		{
			urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
			if (urlFile)
			{
				char buffer[2000];
				DWORD bytesRead;
				do
				{
					InternetReadFile(urlFile, buffer, 2000, &bytesRead);
					rtn.append(buffer, bytesRead);
					memset(buffer, 0, 2000);
				} while (bytesRead);
				InternetCloseHandle(interwebs);
				InternetCloseHandle(urlFile);
				std::string p = replace_all(rtn, "|n", "\r\n");
				return p;
			}
		}
		InternetCloseHandle(interwebs);
		std::string p = replace_all(rtn, "|n", "\r\n");
		return p;
	}

	__forceinline void terminate_process(const std::string& process_name) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			std::cerr << xorstr_("Failed to create snapshot") << std::endl;
			return;
		}

		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(snapshot, &process_entry)) {
			std::cerr << xorstr_("Failed to retrieve process information") << std::endl;
			CloseHandle(snapshot);
			return;
		}

		do {
			if (process_name == process_entry.szExeFile) {
				HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, process_entry.th32ProcessID);
				if (process == NULL) {
					CloseHandle(snapshot);
					return;
				}

				TerminateProcess(process, 0);
				CloseHandle(process);
				break;
			}
		} while (Process32Next(snapshot, &process_entry));

		CloseHandle(snapshot);
	}
}