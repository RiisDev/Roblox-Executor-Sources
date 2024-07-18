#include "luavm_load_hook.h"

bool init_done = false;

uint8_t bytecode_hook_bytes[1 + sizeof(uintptr_t)];
uint8_t bytecode_original_bytes[1 + sizeof(uintptr_t)];
uint8_t hashcheck_hook_bytes[1 + sizeof(uintptr_t)];
uint8_t hashcheck_original_bytes[1 + sizeof(uintptr_t)];

size_t old_bytecode_len;
char* old_bytecode;
size_t new_bytecode_len;
char* new_bytecode;

int i;
DWORD old_protection;

void __declspec(naked) bytecode_hook() {
	__asm {
		pushad;
	}
	printf("Intercepting bytecode...\n");
	VirtualProtect((void*)(addresses::luavm_load_bytecode_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old_protection);
	for (i = 0; i < 1 + sizeof(uintptr_t); i++) {
		*reinterpret_cast<uint8_t*>(addresses::luavm_load_bytecode_hook + i) = bytecode_original_bytes[i];
	}
	VirtualProtect((void*)(addresses::luavm_load_bytecode_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READ, &old_protection);
	__asm {
		mov eax, [ebp + offsets__luavm_load_stackframe__bytecode_len];
		mov old_bytecode_len, eax;
		mov eax, [ebp + offsets__luavm_load_stackframe__bytecode];
		mov old_bytecode, eax;

		mov eax, new_bytecode_len;
		mov [ebp + offsets__luavm_load_stackframe__bytecode_len], eax;
		mov eax, new_bytecode;
		mov [ebp + offsets__luavm_load_stackframe__bytecode], eax;
	}
	printf("Bytecode intercepted\nold_bytecode_len: %X\nold_bytecode: %p\n", old_bytecode_len, old_bytecode);
	__asm {
		popad;
		jmp addresses::luavm_load_bytecode_hook;
	}
}

void __declspec(naked) hashcheck_hook() {
	__asm {
		pushad;
	}
	printf("Bypassing hashcheck...\n");
	VirtualProtect((void*)(addresses::luavm_load_hashcheck_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old_protection);
	for (i = 0; i < 1 + sizeof(uintptr_t); i++) {
		*reinterpret_cast<uint8_t*>(addresses::luavm_load_hashcheck_hook + i) = hashcheck_original_bytes[i];
	}
	VirtualProtect((void*)(addresses::luavm_load_hashcheck_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READ, &old_protection);
	// Hashcheck is a check placed at the end of luavm_load that checks if the bytecode has changed from when it first got called
	__asm {
		mov eax, old_bytecode_len;
		mov [ebp + offsets__luavm_load_stackframe__bytecode_len], eax;
		mov eax, old_bytecode;
		mov [ebp + offsets__luavm_load_stackframe__bytecode], eax;
	}
	printf("Hashcheck bypassed\n");
	__asm {
		popad;
		jmp addresses::luavm_load_hashcheck_hook;
	}
}

bool decompressed_luavm_load(uintptr_t state, std::string bytecode) {
	if (!init_done) {
		printf("Saving original bytes...\n");
		memcpy(bytecode_original_bytes, reinterpret_cast<void*>(addresses::luavm_load_bytecode_hook), 1 + sizeof(uintptr_t));
		memcpy(hashcheck_original_bytes, reinterpret_cast<void*>(addresses::luavm_load_hashcheck_hook), 1 + sizeof(uintptr_t));

		printf("Creating bytecode hook bytes at %X...\n", addresses::luavm_load_bytecode_hook);
		uintptr_t relative_bytecode_hook = reinterpret_cast<uintptr_t>(bytecode_hook) - addresses::luavm_load_bytecode_hook - (1 + sizeof(uintptr_t));
		bytecode_hook_bytes[0] = 0xE9;
		memcpy(bytecode_hook_bytes + 1, &relative_bytecode_hook, sizeof(uintptr_t));

		printf("Creating hashcheck hook bytes at %X...\n", addresses::luavm_load_hashcheck_hook);
		uintptr_t relative_hashcheck_hook = reinterpret_cast<uintptr_t>(hashcheck_hook) - addresses::luavm_load_hashcheck_hook - (1 + sizeof(uintptr_t));
		hashcheck_hook_bytes[0] = 0xE9;
		memcpy(hashcheck_hook_bytes + 1, &relative_hashcheck_hook, sizeof(uintptr_t));

		printf("Initialized hook!\n");
		init_done = true;
	}
	printf("Hooking...\n");
	MEMORY_BASIC_INFORMATION info;
	VirtualQuery((void*)(addresses::luavm_load_bytecode_hook), &info, 1 + sizeof(uintptr_t));
	if ((info.Protect | PAGE_GUARD) == PAGE_GUARD) {
		printf("The time has come. %X\n", info.Protect);
	}
	VirtualProtect((void*)(addresses::luavm_load_bytecode_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old_protection);
	for (int offset = 0; offset < 1 + sizeof(uintptr_t); offset++) {
		*reinterpret_cast<uint8_t*>(addresses::luavm_load_bytecode_hook + offset) = bytecode_hook_bytes[offset];
	}
	VirtualProtect((void*)(addresses::luavm_load_bytecode_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READ, &old_protection);
	VirtualProtect((void*)(addresses::luavm_load_hashcheck_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old_protection);
	for (int offset = 0; offset < 1 + sizeof(uintptr_t); offset++) {
		*reinterpret_cast<uint8_t*>(addresses::luavm_load_hashcheck_hook + offset) = hashcheck_hook_bytes[offset];
	}
	VirtualProtect((void*)(addresses::luavm_load_hashcheck_hook), 1 + sizeof(uintptr_t), PAGE_EXECUTE_READ, &old_protection);

	printf("Preparing bytecode...\n");
	new_bytecode = const_cast<char*>(bytecode.c_str());
	new_bytecode_len = bytecode.length();
	printf("Calling luavm_load...\n");
	int status_code = functions::luavm_load(state, const_cast<std::string*>(&dummy_bytecode), "=TaaprWareV3", NULL);
	// In case the hooks failed to be called
	if (*reinterpret_cast<uint8_t*>(addresses::luavm_load_bytecode_hook) != *bytecode_original_bytes) {
		printf("Bytecode hook failed to hit! Expect a 268 kick right now\n");
	}
	if (*reinterpret_cast<uint8_t*>(addresses::luavm_load_hashcheck_hook) != *hashcheck_original_bytes) {
		printf("Hashcheck hook failed to hit! Expect a 268 kick right now\n");
	}
	printf("Attempted to load function! Status code: %d ", status_code);
	switch (status_code) {
	case 0:
		printf("(success)\n");
		return true;
	case 1:
		printf("(compilation_failure)\n");
		break;
	case 3:
		printf("(decompression_failure)\n");
		break;
	default:
		printf("(unknown)\n");
		break;
	}
	return false;
}