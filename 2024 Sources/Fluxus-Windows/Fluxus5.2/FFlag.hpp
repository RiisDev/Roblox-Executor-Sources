#pragma once
#include <Windows.h>
#include <unordered_map>

namespace Fluxus {
	namespace FFlag {
		const char* GetFlag(const char*);
		bool SetFlag(std::string Flag, const char* To);
	}
}
