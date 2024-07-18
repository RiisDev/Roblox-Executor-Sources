#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <shared_mutex>

// Fake precompressed bytecode we feed into luavm_load
// Find it by breakpointing luavm_load or getting a script's ProtectedString bytecode
// The following is precompressed bytecode from a hello world script
const std::string dummy_bytecode = "\x1B\x7B\x56\x24\xA3\xCC\xB8\xB9\xB9\xC5\x73\xA0\x15\x5A\x1D\x03\xD9\x2A\x60\xAB\x6A\xFC\x61\x54\x58\x15\x64\x5A\x54\x7D\x66\x5E\x3F\x39\x62\xCA\xD4\x7F\xE9\x25\x3C\x7C\x51\x2E\xAA\xC3\x81\x56\xC2\x2D\x63\x1B\x40\xE1\xB2\xA6\x97\x74\xF3\x30\x1F\x65\x10\xDE\xA4\x66\x08\x8C\x3D\x85\x70\xE1\xD3\x13\xB8\xF6\x74\xB8\x42\x40\x19\x7B\xE4\x47\x00";

const uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

namespace addresses {
	const uintptr_t getscheduler = base + 0x738CC0;
	const uintptr_t task_defer = base + 0x3DFF20; // This could be any function that pops a function from the Luau stack and calls it. Defer is one of them. Also note that task.defer silently logs suspicious calls
	const uintptr_t luavm_load = base + 0x359BA0;
	const uintptr_t luavm_load_bytecode_hook = base + 0x35CD14;
	const uintptr_t luavm_load_hashcheck_hook = base + 0x35CCE8;
}

namespace offsets {
	namespace scriptcontext {
		constexpr uintptr_t get_scriptstate(uintptr_t scriptcontext) {
			return *(uintptr_t*)(scriptcontext + 0xF4) - (scriptcontext + 0xF4); // Every encryption changes every week
		}
	}
	namespace state {
		constexpr int top = 0x8; // Luau offsets change every week
	}
	// These only change when roblox makes changes to luavm_load
	// Find them using your disassembler
	namespace luavm_load_stackframe {
		// Because I can't type the whole namespace paths in inline assembly
		#define offsets__luavm_load_stackframe__bytecode -0x15C
		#define offsets__luavm_load_stackframe__bytecode_len -0x6C
	}
}

// Might as well paste from V2
namespace objects {
	typedef struct instance instance;
	typedef struct job job;
	struct property_descriptor {
		uintptr_t* vftable;
		std::string* name;
	};
	struct class_descriptor {
		uintptr_t* vftable;
		std::string* class_name;
		char padding1[0x10];
		std::vector<property_descriptor*> properties;
	};
	struct instance {
		uintptr_t* vftable;
		std::shared_ptr<instance> self;
		class_descriptor* class_descriptor;
		char padding1[0x1C];
		std::string* name;
		std::vector<std::shared_ptr<instance>>* children;
		char padding2[0x4];
		instance* parent;
	};
	constexpr int x = offsetof(instance, children);
	struct job {
		uintptr_t* vftable;
		job* self;
		char padding1[0x8];
		std::string name;
		uintptr_t datamodel_minus_4; // add 4 then cast to rbx::objects::instance* to get datamodel
	};
	struct task_scheduler {
		char padding1[0x118];
		double fps;
		char padding2[0x14];
		std::vector<std::shared_ptr<job>> jobs;
	};
}

namespace functions {
	namespace types {
		typedef objects::task_scheduler*(__cdecl* getscheduler)();
		typedef int(__fastcall* luavm_load)(IN uintptr_t state, IN std::string* compressed_bytecode, IN const char* chunkname, IN OPTIONAL int env);
		typedef int(__cdecl* task_defer)(IN uintptr_t state);
	}
	const types::getscheduler getscheduler = reinterpret_cast<types::getscheduler>(addresses::getscheduler);
	const types::luavm_load luavm_load = reinterpret_cast<types::luavm_load>(addresses::luavm_load);
	const types::task_defer task_defer = reinterpret_cast<types::task_defer>(addresses::task_defer);
}