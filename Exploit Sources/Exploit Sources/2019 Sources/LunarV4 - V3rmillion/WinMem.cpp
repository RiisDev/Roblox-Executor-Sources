/*
 * File Name: WinMem.cpp
 * Author(s): Mathias Pedersen
 *
 * Windows Memory Library
 *
 * Copyright (C) 2019, Mathias Pedersen
 */

#include "WinMem.h"
#include <Windows.h>
#include <Psapi.h>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "Hde32\hde32.h"

 /* Hex charset */
const char* hex = "0123456789ABCDEF";

/* Structure used to represent part of a pattern */
struct ScanByte
{
	bool wildcard;
	uint8_t byte;

	ScanByte() : wildcard(true), byte(0) {}
	ScanByte(uint8_t b) : wildcard(false), byte(b) {}
};

/* Check if memory matches a byte pattern */
static bool compare(uintptr_t p, const std::vector<ScanByte>& vec)
{
	uint8_t* ptr = reinterpret_cast<uint8_t*>(p);
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (ptr[i] != vec[i].byte && !vec[i].wildcard)
			return false;
	}
	return true;
}

/* Split a vector by spaces */
static std::vector<std::string> split(const std::string & str)
{
	std::istringstream buf(str);
	std::istream_iterator<std::string> beg(buf), end;
	return std::vector<std::string>(beg, end);
}

/* Parse part of a pattern */
static ScanByte parsePart(const std::string & part)
{
	/* Is it a wildcard? */
	if (part == "?" || part == "??")
		return ScanByte();

	/* Convert to byte */
	if (part.length() == 2)
	{
		const char* low1 = std::lower_bound(hex, hex + 16, part[0]);
		const char* low2 = std::lower_bound(hex, hex + 16, part[1]);
		uint8_t b = static_cast<uint8_t>(((low1 - hex) << 4) | (low2 - hex));
		return ScanByte(b);
	}
	else
	{
		throw std::exception("invalid format");
	}
}
#include <iostream>
uintptr_t WinMem::scan(uintptr_t start, uintptr_t end, const std::string & pattern, ptrdiff_t offset, bool traceRelative, int alignment)
{
	/* Split pattern into strings */
	std::vector<std::string> parts = split(pattern);

	/* Create vector of ScanBytes */
	std::vector<ScanByte> vec;
	vec.reserve(parts.size());

	for (size_t i = 0; i < parts.size(); i++)
		vec.push_back(parsePart(parts[i]));

	MEMORY_BASIC_INFORMATION mbi;

	/* Scan module */
	for (uintptr_t p = start; p != end; p += alignment)
	{
		VirtualQuery(reinterpret_cast<LPVOID>(p), &mbi, 1024);
		if (!(mbi.State & MEM_COMMIT))
		{
			p += 1024 - alignment;
			continue;
		}

		if (!(p % 0x100000))
			std::cout << std::hex << p << std::endl;
		if (IsBadReadPtr(reinterpret_cast<void*>(p), 1))
			continue;

		if (compare(p, vec))
		{
			if (traceRelative)
				return traceRelativeCall(p + offset);
			return p + offset;
		}
	}

	/* We didn't find the pattern, fuck! */
	return 0;
}

uintptr_t WinMem::scan(HMODULE hModule, const std::string & pattern, ptrdiff_t offset, bool traceRelative, int alignment)
{
	/* Get module start and end address */
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));

	uintptr_t start = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
	uintptr_t end = start + modInfo.SizeOfImage;

	/* Split pattern into strings */
	std::vector<std::string> parts = split(pattern);

	/* Create vector of ScanBytes */
	std::vector<ScanByte> vec;
	vec.reserve(parts.size());

	for (size_t i = 0; i < parts.size(); i++)
		vec.push_back(parsePart(parts[i]));

	/* Scan module */
	for (uintptr_t p = start; p != end; p += alignment)
	{
		if (IsBadReadPtr(reinterpret_cast<void*>(p), 1))
			continue;
		if (compare(p, vec))
		{
			if (traceRelative)
				return traceRelativeCall(p + offset);
			return p + offset;
		}
	}

	/* We didn't find the pattern, fuck! */
	return 0;
}

uintptr_t WinMem::findOp(uintptr_t ptr, uint8_t op, int count)
{
	hde32s dis;
	while (true)
	{
		hde32_disasm(reinterpret_cast<void*>(ptr), &dis);
		if (dis.opcode == op)
		{
			if (count-- == 0)
				return ptr;
		}
		ptr += dis.len;
	}
}