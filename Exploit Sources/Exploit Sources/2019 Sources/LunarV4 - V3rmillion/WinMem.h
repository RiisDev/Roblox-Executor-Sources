/*
 * File Name: WinMem.hpp
 * Author(s): Mathias Pedersen
 *
 * Windows Memory Library
 *
 * Copyright (C) 2019, Mathias Pedersen
 */

#pragma once

#include <Windows.h>
#include <vector>
#include <stdint.h>
#include <string>

namespace WinMem
{
	/* Write a value to a location */
	template <class T>
	static void write(void* lpTarget, T value)
	{
		DWORD dwOldProtection;
		VirtualProtect(lpTarget, sizeof(T), PAGE_READWRITE, &dwOldProtection);
		*reinterpret_cast<T*>(lpTarget) = value;
		VirtualProtect(lpTarget, sizeof(T), dwOldProtection, &dwOldProtection);
	}

	/* Write a number of values to a location */
	template <class T>
	static void write(void* lpTarget, std::vector<T> vec)
	{
		DWORD dwOldProtection;
		VirtualProtect(lpTarget, vec.size() * sizeof(T), PAGE_READWRITE, &dwOldProtection);
		std::memcpy(lpTarget, vec.data(), vec.size() * sizeof(T));
		VirtualProtect(lpTarget, vec.size() * sizeof(T), dwOldProtection, &dwOldProtection);
	}

	/* Scan for a pattern */
	uintptr_t scan(uintptr_t start, uintptr_t end, const std::string& pattern, ptrdiff_t offset, bool traceRelative, int alignment = 1);

	/* Scan a module for a pattern */
	uintptr_t scan(HMODULE hModule, const std::string& pattern, ptrdiff_t offset, bool traceRelative, int alignment = 1);

	/* Resolve a 32 bit relative call */
	inline uintptr_t traceRelativeCall(uintptr_t call)
	{
		return *reinterpret_cast<uintptr_t*>(call + 1) + call + 5;
	}

	/* Find the first occurence of an opcode */
	uintptr_t findOp(uintptr_t ptr, uint8_t op, int count = 0);
}