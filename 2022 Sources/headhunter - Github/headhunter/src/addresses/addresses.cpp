#include "addresses.h"
#include "../console/console.h"

const uintptr_t current_module = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));


const rbx_getscheduler_t rbx_getscheduler = reinterpret_cast<rbx_getscheduler_t>(current_module + 0xde3590);
const rbx_output_t rbx_output = reinterpret_cast<rbx_output_t>(current_module + 0x223380);
const rbx_getstate_t rbx_getstate = reinterpret_cast<rbx_getstate_t>(current_module + 0x3a97b0);

const std::uintptr_t spawn_func = current_module + 0x3c0d90;
const std::uintptr_t deserializer_func = current_module + 0x16a2f40;
const std::uintptr_t fake_ret = current_module + 0xf405ee;

void rbx_spawn(std::uintptr_t rl)
{
	__asm {
		mov edi, finished
		push rl
		push fake_ret
		jmp spawn_func
	finished:
		add esp, 4
	}
}

void rbx_deserialize(std::uintptr_t rl, const char* chunk_name, const char* bytecode, int byte_len)
{
	__asm {
		mov edi, finished
		mov ecx, rl
		mov edx, chunk_name
		push 0
		push byte_len
		push bytecode
		push fake_ret
		jmp deserializer_func
	finished:
		add esp, 0xC
	}
}

void rbx_decrement_top(std::uintptr_t rl, std::int32_t amount)
{
	*reinterpret_cast<std::uintptr_t*>(rl + offsets::luastate::top) -= (16 * amount);
}

void rbx_setidentity(std::uintptr_t rl, std::int8_t identity)
{
	*reinterpret_cast<std::int8_t*>(*reinterpret_cast<std::uintptr_t*>(rl + offsets::identity::extra_space) + offsets::identity::identity) = identity;
}