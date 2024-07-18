#pragma once

/* Core Includes */
#include <Windows.h>
#include <fstream>

/* Defines */
#define Convert(Type, Object) reinterpret_cast<Type>(Object);

class Hook {
	std::unique_ptr<std::uintptr_t[]> New{ nullptr };
	std::uintptr_t* Backup{ nullptr };
	std::uintptr_t Functions{};

public:
	Hook(void* Instance) {
		Backup = *reinterpret_cast<std::uintptr_t**>(Instance);

		while (reinterpret_cast<const std::uintptr_t*>(*reinterpret_cast<const std::uintptr_t*>(Instance))[Functions])
			Functions++;

		const auto Size = ((Functions * 4) + 4);
		New = std::make_unique<std::uintptr_t[]>(Functions + 1);

		std::memcpy(New.get(), &Backup[-1], Size);
		*reinterpret_cast<const std::uintptr_t**>(Instance) = &New.get()[1];
	}

	auto HookMethod(void* Pointer, const std::intptr_t Index) -> void*;
	auto UnHookMethod(const std::intptr_t Index) -> void;
};