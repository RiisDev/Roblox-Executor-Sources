#pragma once
#include "Hook.hpp"

auto Hook::HookMethod(void* Pointer, const std::intptr_t Index) -> void* {
	this->New[Index + 1] = Convert(const std::uintptr_t, Pointer);
	return Convert(void*, Backup[Index]);
}

auto Hook::UnHookMethod(const std::intptr_t Index) -> void {
	this->New[Index + 1] = this->Backup[Index];
}