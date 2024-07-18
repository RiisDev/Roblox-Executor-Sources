// [INFO] Roblox shuffles everything they can every update: addresses, offsets, and even offset encryptions

#pragma once

#include <string>
#include <vector>
#include <shared_mutex>
#include <Windows.h>
#include <functional>

#include "encryption.h"

const UINT base = reinterpret_cast<UINT>(GetModuleHandleA(NULL));

#define	define_address(name, address) const UINT name = base + address
#define define_type(return_type, calling_convention, name, ...) typedef return_type(calling_convention* rbx_newthread)(__VA_ARGS__)
#define define_function(name) inline types::name name = reinterpret_cast<types::name>(addresses::name)
#define	define_runtime_object(type, name) static type name
#define	define_offset(name, offset, getlamb, setlamb) \
	const int name = offset; \
	const auto name##_##get = []<typename T>(T address_of_ptr) -> T {getlamb;}; \
	const auto name##_##set = []<typename T>(T address_of_ptr, T value) -> void {setlamb;};

typedef struct {
	BYTE bytes[5];
} patch_5;
typedef struct {
	BYTE bytes[6];
} patch_6;
typedef struct {
	BYTE bytes[16];
} patch_16;
typedef UINT* vftable;
typedef struct {
	char contents[128];
} rbx_string;

typedef enum {
	_0_Anonymous,
	_1_LocalGUI,				// Any action initiated by Roblox Studio or the mouse 
	_2_GameScript,				// Execution of a BaseScript object inside any DataModel
	_3_GameScriptInRobloxPlace,	// Execution of a BaseScript object inside any DataModel, if the place was authored by Roblox
	_4_RobloxGameScript,		// Execution of a BaseScript object written by Roblox inside any DataModel
	_5_CmdLine,					// Any script executed from the Studio command line
	_6_COM,						// Scripts executed via the COM API (usually comes from roblox.com)
	_7_WebService,				// Scripts executed via the Web Service API (usually comes from roblox.com)
	_8_Replicator,				// Receiving data via replication
	COUNT_Identities            // Not a true identity. Used for enumeration
} Identities;
#define execution_level _8_Replicator // Feel free to edit
typedef enum {
	Output,
	Info,
	Warn,
	Error
} PrintType;
typedef enum {
	StopJob,
	ContinueStepping,
} JobStatus;

namespace objects {
	typedef struct instance instance;
	typedef struct class_descriptor class_descriptor;
	typedef struct datamodel datamodel;
	typedef struct job job;
	typedef struct waiting_hybrid_scripts_job waiting_hybrid_scripts_job;
	typedef struct task_scheduler task_scheduler;
	typedef struct script_start script_start;
	struct instance {
		vftable vftable;
		std::shared_ptr<instance> self;
		class_descriptor* class_descriptor;
		char padding1[0x14];
		std::string* name;
		std::vector<instance*>* children;
		char padding2[0x4];
		instance* parent;
	};
	struct class_descriptor {
		char padding2[0x4];
		std::string* class_name;
	};
	struct job {
		vftable vftable;
		job* self;
		char padding1[8];
		std::string name;
	};
	struct waiting_hybrid_scripts_job {
		vftable vftable;
		char padding1[0x12C];
		instance* script_context;
	};
	struct task_scheduler {
		char padding1[0x118];
		double fps;
		char padding2[0x14];
		std::vector<std::shared_ptr<objects::job>> jobs;
	};
	struct script_start {
		std::shared_ptr<objects::instance> script;
		Identities identity;
		int nullptrs[8];
	};
}
typedef JobStatus(__fastcall* job_step_func)(objects::job* self, int fakearg, int a2);

// plusgiant5#7930 for address help
// you can get base execution with a bit of editing with no offsets and only type_name, rbx_getscheduler, rbx_addscript, rbx_runscript, and rbx_deserializer_detour 1 and 2
const UINT rbx_writevoxels_instsize = 13; // sum of length of instructions affected by placing a push ret (5 bytes) at rbx_writevoxels
// instructions affected by placing a push ret (5 bytes) at rbx_writevoxels
#define rbx_writevoxels_overwritten_instructions \
	mov[ebp - 0x44], ecx; \
	mov[ebp - 0x134], 0x3246AE8; 
namespace addresses {
	define_address(rbx_printf, 0xC2DCE0); // tons of ways to get this
	define_address(rbx_getscheduler, 0x718F60); // tons of ways to get this
	define_address(rbx_addscript, 0x3B04B0); // startscript's only xref (used for setting level)
	define_address(rbx_runscript, 0x3BC530); // xref addscript and check each one until a breakpoint hits (used for starting execution cycle)
	define_address(rbx_deserializer_detour, 0x340230); // after getbytecode in deserializer, right after the conditional (used for setting bytecode)
	define_address(rbx_deserializer_detour_2, 0x340204); // after getbytecode in deserializer, inside the conditional, after the label definition (used for hashcheck bypass)
	define_address(rbx_writevoxels, 0xF2BAA0); // [AFTER THE PROLOGUE] cause a Lua error in WriteVoxels to know how to find this

	// Look at Luau source for all of these
	define_address(type_name, 0x13ECB30); // easiest Luau function "no value"
	define_address(pseudo2addr, 0x13DE050); // literally just scroll down from type_name and you will 99% see a function that calls this lol
	define_address(luaO_nilobject, 0x2482770); // the only pointer inside pseudo2addr

	define_address(load, 0x33D0E0); // aka LuaVM::load, for loadstring only, also credit to Nezy-Exploit for the code involving this
}

namespace types {
	typedef void (__cdecl* rbx_printf)(PrintType type, const char* format, ...);
	typedef objects::task_scheduler* (__cdecl* rbx_getscheduler)();
	typedef void(__fastcall* rbx_addscript)(objects::instance* scriptcontext, int fake, objects::script_start script_start);
	typedef void(__fastcall* rbx_runscript)(objects::instance* runtimescriptservice, int fake, objects::instance* script);

	typedef const char*(__fastcall* type_name)(UINT rL, int type);
	typedef void*(__fastcall* pseudo2addr)(UINT rL, int idx);

	typedef UINT(__fastcall* load)(UINT rL, std::string* source, const char* chunkname, int env);
}

namespace functions {
	define_function(rbx_printf);
	define_function(rbx_getscheduler);
	namespace runtimescriptservice {
		define_function(rbx_runscript);
	}

	namespace luau {
		define_function(type_name);
		define_function(pseudo2addr);
	}

	define_function(load);
}

/*
Offset Guide:
1. Look at pseudocode of any newlstr call (noob tip: looking around ATOM_UNDEF will help you get the right offsets)
2. Basically everywhere
3. Any checking of arguments
4. Found experimenting with Cheat Engine
5. Look at pseudocode of any newthread call
6. Look at pseudocode of luaL_getmetafield
7. Basic reversing using error messages (use Ghidra to find encrypted strings)
8. Checking of Luau source and use of Ghidra for strings
*/
#define get_unencrypted return *(T*)(address_of_ptr)
#define set_unencrypted	*(T*)(address_of_ptr) = value
// Don't get overwhelmed. None of these are required for base execution, and only top and base are required for __send_to_C
namespace offsets {
	namespace rCommonHeader {
		// I don't know how GC works fully yet, I just know every object NEEDS this to cooperate with roblox properly
		define_offset(tt, 0x2, get_unencrypted, set_unencrypted); // 1
		define_offset(marked, 0x0, get_unencrypted, set_unencrypted); // 1
		define_offset(memcat, 0x1, get_unencrypted, set_unencrypted); // 1
	}
	namespace rL {
		define_offset(activememcat, 0x4, get_unencrypted, set_unencrypted); // 1
		define_offset(top, 0x8, get_unencrypted, set_unencrypted); // 3
		define_offset(base, 0x10, get_unencrypted, set_unencrypted); // 3
		define_offset(global, 0xC, // 1, 2, 3, 5, 6, 7, 8
			/* get */ return *(UINT*)(address_of_ptr) + address_of_ptr,
			/* set */ );
		define_offset(namecall, 0x44, get_unencrypted, set_unencrypted); // (getnamecallmethod) 8h
		define_offset(userdata, 0x48, get_unencrypted, set_unencrypted); // (aka extraspace) 5h
	}
	namespace rG {
		// rG->rstringtable isn't a pointer
		//define_offset(strt_size, 0x8, get_unencrypted, set_unencrypted); // (unused) 1h
		//define_offset(strt_nuse, 0x4, get_unencrypted, set_unencrypted); // (unused) 1h
		//define_offset(strt_hash, 0x0, get_unencrypted, set_unencrypted); // (unused) 1h
		define_offset(currentwhite, 0x14, get_unencrypted, set_unencrypted); // 1
	}
	namespace rTString {
		define_offset(tt, offsets::rCommonHeader::tt, get_unencrypted, set_unencrypted);
		define_offset(marked, offsets::rCommonHeader::marked, get_unencrypted, set_unencrypted);
		define_offset(memcat, offsets::rCommonHeader::memcat, get_unencrypted, set_unencrypted);

		define_offset(atom, 0x4, get_unencrypted, set_unencrypted); // 1
		//define_offset(next, 0x8, get_unencrypted, set_unencrypted); // (unused) 1
		define_offset(hash, 0xC, // 1
			/* get */ return *(T*)(address_of_ptr) + address_of_ptr,
			/* set */ *(T*)(address_of_ptr) = value - address_of_ptr);
		define_offset(len, 0x10, // 1
			/* get */ return *(T*)(address_of_ptr) - address_of_ptr,
			/* set */ *(T*)(address_of_ptr) = address_of_ptr - value);
		define_offset(data, 0x14, get_unencrypted, set_unencrypted); // (unchanging) 1
	}
	namespace rUdata {
		define_offset(metatable, 0x8, // 6h
		/* get */ return address_of_ptr - *(T*)(address_of_ptr),
		/* set */ *(T*)(address_of_ptr) = address_of_ptr - value);
		define_offset(data, 0x10, get_unencrypted, set_unencrypted); // (unchanging) 4h
	}
	namespace rTable {
		define_offset(readonly, 0x6, get_unencrypted, set_unencrypted); // (unchanging) 8h
		define_offset(metatable, 0x1C, // 6h
		/* get */ return address_of_ptr - *(T*)(address_of_ptr),
		/* set */ *(T*)(address_of_ptr) = address_of_ptr - value);
	}
	namespace rextraspace {
		define_offset(identity, 0x18, get_unencrypted, set_unencrypted);
	}
	namespace rPlayer {
		define_offset(simulationradius, 0x1D0, get_unencrypted, set_unencrypted); // 4h
		define_offset(maximumsimulationradius, 0x1D4, get_unencrypted, set_unencrypted); // 4h
	}
}
inline UINT get_rL(UINT scriptcontext) {
	return (scriptcontext + 0x104) ^ *(UINT*)(scriptcontext + 0x104);
}
