#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>

#include "Luau/lapi.h"
#include "Luau/lobject.h"
#include "Luau/lstring.h"
#include "Luau/lua.h"
#include "Luau/lualib.h"
#include "Luau/lfunc.h"
#include "Luau/lmem.h"
#include "Luau/lgc.h"
#include "Luau/ltable.h"


static uintptr_t max_capabilities = 0x3FFFF00 | 0xFFFFFFFFFFFFFCFFui64;

struct live_thread_ref
{
	int unk_0;
	lua_State* state;
	int state_id;
	int unk_1;
};


struct thread_ref
{
	live_thread_ref* ref;
	thread_ref(lua_State* L)
	{
		ref = new live_thread_ref;
		ref->unk_0 = 0;
		ref->state = L;
		lua_pushthread(L);
		ref->state_id = lua_ref(L, -1);
		lua_settop(L, 0);

		ref->unk_1 = 0;
	}
};


static std::queue<std::pair<thread_ref, int>> resume_queue = {};
static std::queue<std::string> coderun_queue = {};

using roblox_function = std::intptr_t(__fastcall*)(std::intptr_t L);


struct signal_t;

struct signal_connection_t {
	char padding[20];
	int func_idx; //0x14
};

struct signal_data_t {
	uint64_t padding1;
	signal_t* root; //0x8
	uint64_t padding2[12];
	signal_connection_t* connection_data; //0x70
};

struct signal_t {
	uint64_t padding1[2];
	signal_t* next; //0x10
	uint64_t padding2;
	uint64_t state;
	uint64_t padding3;
	signal_data_t* signal_data; //0x30
};


static auto aslr(__int64 address) -> __int64
{
	return address + reinterpret_cast<__int64>(GetModuleHandle(NULL));
}

static Table* getcurrenv(lua_State* L)
{
	if (L->ci == L->base_ci)
		return L->gt;
	else
		return curr_func(L)->env;
}

static LUAU_NOINLINE TValue* pseudo2addr(lua_State* L, int idx)
{
	api_check(L, lua_ispseudo(idx));
	switch (idx)
	{ // pseudo-indices
	case LUA_REGISTRYINDEX:
		return registry(L);
	case LUA_ENVIRONINDEX:
	{
		sethvalue(L, &L->global->pseudotemp, getcurrenv(L));
		return &L->global->pseudotemp;
	}
	case LUA_GLOBALSINDEX:
	{
		sethvalue(L, &L->global->pseudotemp, L->gt);
		return &L->global->pseudotemp;
	}
	default:
	{
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->nupvalues) ? &func->c.upvals[idx - 1] : cast_to(TValue*, luaO_nilobject);
	}
	}
}


static LUAU_FORCEINLINE TValue* index2addr(lua_State* L, int idx)
{
	if (idx > 0)
	{
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top)
			return cast_to(TValue*, luaO_nilobject);
		else
			return o;
	}
	else if (idx > LUA_REGISTRYINDEX)
	{
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else
	{
		return pseudo2addr(L, idx);
	}
}

bool lua_isscriptable(lua_State* L, uintptr_t property)
{
	auto scriptable = *reinterpret_cast<__int64*>(property + 64);
	return (scriptable >> 5) & 1;
}


void lua_setscriptable(lua_State* L, uintptr_t property, bool enabled)
{
	*reinterpret_cast<int*>(property + 64) = enabled ? 0xFF : 0;
}


namespace Offsets
{
	static __int64 pushinstance = aslr(0xC74B40); //25.09.2024
	static __int64 decryptstate = 0xA7FE00;  //25.09.2024
	static __int64 print = 0x11FBE70; //25.09.2024
	static __int64 getscheduler = 0x2BC4A10; //25.09.2024 string 'V2cInference'
	static __int64 getstate = 0xC97180; //25.09.2024
	static __int64 spawn = 0xD357A0; //25.09.2024
	static __int64 luavm_load = 0xA82390; //25.09.2024
	static __int64 touch = 0x10E6AB0; //25.09.2024
	static __int64 click_detector = 0x1726340; //25.09.2024

	namespace TaskScheduler
	{
		static __int64 job_start = 0x198;
		static __int64 job_end = 0x1A0;
		static __int64 job_name = 0x90;
	}

	namespace LuaState
	{
		static __int64 userdata = 120;
		static __int64 top = 32;
		static __int64 script_context = 504;
	}

	namespace Bytecode
	{
		static __int64 localscript = 448;
		static __int64 modulescript = 352;
	}
}


namespace Defs
{
	using print_t = void(__fastcall*)(int type, const char* source);
	static print_t rbx_print = reinterpret_cast<print_t>(aslr(Offsets::print));

	using pushinstance_t = void(__fastcall*)(__int64 state, __int64* instance);
	static pushinstance_t rbx_pushinstance = reinterpret_cast<pushinstance_t>(Offsets::pushinstance);

	using epushinstance_t = void(__fastcall*)(__int64 state, std::shared_ptr<std::uintptr_t>& instance);
	static epushinstance_t rbx_e_pushinstance = reinterpret_cast<epushinstance_t>(Offsets::pushinstance);

	using getscheduler_t = __int64(__cdecl*)();
	static getscheduler_t rbx_getscheduler = reinterpret_cast<getscheduler_t>(aslr(Offsets::getscheduler));

	using spawn_t = __int64(__fastcall*)(__int64 state);
	static spawn_t rbx_spawn = reinterpret_cast<spawn_t>(aslr(Offsets::spawn));

	using getstate_t = __int64(__fastcall*)(__int64 script_context, __int64* a1, __int64* a2);
	static getstate_t rbx_getstate = reinterpret_cast<getstate_t>(aslr(Offsets::getstate));

	using luavmload_t = __int64(__fastcall*)(__int64 state, std::string* source, const char* chunk, int enviroment);
	static luavmload_t rbx_luavmload = reinterpret_cast<luavmload_t>(aslr(Offsets::luavm_load));

	using decryptstate_t = __int64(__fastcall*)(__int64 a1);
	static decryptstate_t decryptstate = reinterpret_cast<decryptstate_t>(aslr(Offsets::decryptstate));

	using touch_t = void(__fastcall*)(uint64_t world, uint64_t from_prim, uint64_t to_prim, char touched);
	static touch_t rbx_touch = reinterpret_cast<touch_t>(aslr(Offsets::touch));

	using clickdetector_t = void(__fastcall*)(__int64 a1, float a2, __int64 a3);
	static clickdetector_t rbx_click = reinterpret_cast<clickdetector_t>(aslr(Offsets::click_detector));
}


static __int64 menu_job = 0;

namespace TaskScheduler
{
	static auto get_jobs() -> std::vector<__int64>
	{
		std::vector<__int64> jobs;

		__int64 task_scheduler = Defs::rbx_getscheduler();

		__int64* current_job = *reinterpret_cast<__int64**>(task_scheduler + Offsets::TaskScheduler::job_start);

		do {
			jobs.push_back(*current_job);
			current_job += 2;
		} while (current_job != *reinterpret_cast<__int64**>(task_scheduler + Offsets::TaskScheduler::job_end));

		return jobs;
	}

	static __int64 get_jobs_by_name(std::string name)
	{
		std::vector <__int64> jobs = get_jobs();

		__int64 last_job = 0;

		for (__int64& job : jobs)
		{
			if (std::string* job_name = reinterpret_cast<std::string*>(job + Offsets::TaskScheduler::job_name); *job_name == name)
			{
				last_job = job;
				break;
			}
		}

		return last_job;
	}

	static std::vector<__int64> get_all_jobs_by_name(std::string name)
	{
		std::vector <__int64> result;
		std::vector <__int64> jobs = get_jobs();

		for (__int64& job : jobs)
		{
			if (std::string* job_name = reinterpret_cast<std::string*>(job + Offsets::TaskScheduler::job_name); *job_name == name)
			{
				result.push_back(job);
			}
		}

		return result;
	}

	static __int64 get_script_context()
	{
		__int64 needed;
		auto jobs = get_all_jobs_by_name("WaitingHybridScriptsJob");
		for (int i = 0; i < jobs.size(); i++)
		{
			if (jobs[i] != menu_job) { needed = jobs[i]; break; }
		}

		__int64 waitinghybridscripts_job = needed;
		return *reinterpret_cast<__int64*>(waitinghybridscripts_job + 504);
	}

	static __int64 get_lua_state()
	{
		__int64 script_context = get_script_context();

		__int64 arg1 = 0;
		__int64 arg2 = 0;
		__int64 state = Defs::rbx_getstate(script_context + 272, &arg1, &arg2);

		auto part = Defs::decryptstate(state + 136);
		return part;
	}

	static bool is_loaded()
	{
		int counter = get_all_jobs_by_name("WaitingHybridScriptsJob").size();
		if (counter == 2)
			return true;
		else
			return false;
	}

	static void set_fps(double fps)
	{
		static const double min_frame_delay = 1.0 / 10000.0;
		double frame_delay = fps <= 0.0 ? min_frame_delay : 1.0 / fps;

		*reinterpret_cast<double*>(Defs::rbx_getscheduler() + 0x178) = frame_delay;
	}

	static int get_fps()
	{
		return 1 / *reinterpret_cast<double*>(Defs::rbx_getscheduler() + 0x178);
	}
}


namespace ProtoThings
{
	static uintptr_t deobfuscate_proto(uintptr_t object)
	{
		return (object + 24) + *reinterpret_cast<uintptr_t*>(object + 24);
	}

	static uintptr_t deobfuscate_proto_p(uintptr_t object)
	{
		return (object + 16) + *reinterpret_cast<uintptr_t*>(object + 16);
	}

	static void raise_proto(uintptr_t proto)
	{
		*reinterpret_cast<uintptr_t**>(proto + 0x78) = &max_capabilities;
		uintptr_t* p = (uintptr_t*)(deobfuscate_proto_p(proto));
		for (int i = 0; i < *reinterpret_cast<int*>(proto + 0xA0); i++)
			raise_proto(p[i]);
	}
}


namespace Identity
{
	static auto set_context(__int64 state, int level) -> void
	{
		uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

		*reinterpret_cast<uintptr_t*>(userdata + 0x30) = level;
		*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;
	}
}