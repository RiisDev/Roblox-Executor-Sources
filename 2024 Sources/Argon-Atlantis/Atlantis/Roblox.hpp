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



inline static uintptr_t max_capabilities = 0x3FFFFFF00i64 | 0xFFFFFFFFFFFFFCFFui64;
inline static __int64 blacklisted_job = 0;


inline std::queue<std::string> coderun_queue = {};
inline std::vector<std::string> console_logs = {};


using roblox_function = std::intptr_t(__fastcall*)(std::intptr_t L);


struct signal_t;
 
struct signal_connection_t {
	char padding[16];
	int thread_idx; // 0x10
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




struct live_thread_ref
{
	int __atomic_refs; // 0
	lua_State* th; // 8
	int thread_id; // 16
	int ref_id; // 20
};

struct weak_thread_ref_t
{
	std::uint8_t pad_0[16];

	weak_thread_ref_t* previous; // 16
	weak_thread_ref_t* next; // 24
	live_thread_ref* liveThreadRef; // 32
	struct Node_t* node; // 40

	std::uint8_t pad_1[8]; // 52
};

struct functionscriptslot_t {
	std::uint8_t pad_0[112];

	live_thread_ref* func_ref; // 112
};

struct slot_t {
	int64_t unk_0; // 0
	void* func_0; // 8

	slot_t* next; // 16
	void* __atomic; // 24

	int64_t sig; // 32

	void* func_1; // 40

	functionscriptslot_t* storage; // 48
};

struct SignalConnectionBridge {
	slot_t* islot; // 0
	int64_t __shared_reference_islot; // 8

	int64_t unk0; // 16
	int64_t unk1; // 24
};

struct Node_t
{
	std::uint8_t pad_0[8];

	weak_thread_ref_t* wtr; // 8
};

struct scriptvmstate_t {
	void* fill;

	// unknown in 64 bits!
   /* std::uint8_t pad_0[168]; // 0

	weak_thread_ref_t _G_tbl; // 168
	weak_thread_ref_t shared_tbl; // 192*/
};


struct ExtraSpace {
	struct Shared {
		int32_t threadCount;               
		void* scriptContext;               
		void* scriptVmState;           
		char field_18[0x8];               
		void* __intrusive_set_AllThreads; 
	};

	char _0[8];                  
	char _8[8];                
	char _10[8];                   
	struct Shared* sharedExtraSpace;
};



struct weak_thread_ref_t2 {
	std::atomic< std::int32_t > _refs;
	lua_State* thread;
	std::int32_t thread_ref;
	std::int32_t object_id;
	std::int32_t unk1;
	std::int32_t unk2;

	weak_thread_ref_t2(lua_State* L)
		: thread(L), thread_ref(NULL), object_id(NULL), unk1(NULL), unk2(NULL) {
	};
};

struct debugger_result_t {
	std::int32_t result;
	std::int32_t unk[0x4];
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

__forceinline bool lua_isscriptable(lua_State* L, uintptr_t property)
{
	auto scriptable = *reinterpret_cast<__int64*>(property + 64);
	//return (scriptable >> 5) & 1;
	return scriptable & 0x20;
}


__forceinline void lua_setscriptable(lua_State* L, uintptr_t property, bool enabled)
{
	*reinterpret_cast<int*>(property + 64) = enabled ? 0xFF : 0;
}


namespace Offsets
{
	//static auto map_descriptor = reinterpret_cast<__int64*>(aslr(0x55EF020)); //20.11.2024
	//static __int64 get_callback_prop = aslr(0xA04130); //20.11.2024
	//static __int64 pushinstance = aslr(0xE20B20); //20.11.2024
	//static __int64 decryptstate = 0xAEE8F0; //20.11.2024
	//static __int64 print = 0x12DB520; //20.11.2024
	//static __int64 getscheduler = 0x2D91830; //20.11.2024
	//static __int64 getstate = 0xD35D80; //20.11.2024
	//static __int64 spawn = 0xE42830; //20.11.2024
	//static __int64 defer = 0xEC1550; //20.11.2024
	//static __int64 luavm_load = 0xAF18A0; //20.11.2024
	//static __int64 touch = 0x11BBFE0; //20.11.2024
	//static __int64 click_detector = 0x18EE470; //20.11.2024
	//static __int64 impersonator = aslr(0x2B40B30); //20.11.2024
	//static __int64 getfflag = aslr(0x13F2C10); //20.11.2024
	//static __int64 flog_data_bank = aslr(0x56C0AB8); //20.11.2024
	//static __int64 setfflag = aslr(0x2F0CF90); //20.11.2024
	//static __int64 fireproximityprompt = aslr(0x16E2710); //20.11.2024

	//static __int64 loadmodule_flag = aslr(0x51859E8);

	//static __int64 RequireFlag = 0x1B0;

	//namespace TaskScheduler
	//{
	//	static __int64 job_start = 0x198;
	//	static __int64 job_end = 0x1A0;
	//	static __int64 job_name = 0x90;
	//}

	//namespace LuaState
	//{
	//	static __int64 userdata = 120;
	//	static __int64 top = 40;
	//	static __int64 script_context = 504;
	//}

	//namespace Bytecode
	//{
	//	static __int64 localscript = 448;
	//	static __int64 modulescript = 360;
	//}

	static auto map_descriptor = reinterpret_cast<__int64*>(aslr(0x56C0210)); //11.12.2024
	static __int64 get_callback_prop = aslr(0xA13A40); //11.12.2024
	static __int64 pushinstance = aslr(0xE2E890); //11.12.2024
	static __int64 decryptstate = 0xAFE360; //11.12.2024
	static __int64 print = 0x12F21D0; //11.12.2024
	static __int64 getscheduler = 0x2DD80A0; //11.12.2024
	static __int64 getstate = 0xD44D30; //11.12.2024
	static __int64 spawn = 0xED0220; //11.12.2024
	static __int64 luavm_load = 0xB012F0; //11.12.2024
	static __int64 touch = 0x11D63A0; //11.12.2024
	static __int64 click_detector = 0x190B8D0; //11.12.2024
	static __int64 impersonator = aslr(0x2B76430); //011.12.2024
	static __int64 getfflag = aslr(0x1403B30); //11.12.2024
	static __int64 flog_data_bank = aslr(0x5793E08); //11.12.2024
	static __int64 setfflag = aslr(0x2F438E0); //11.12.2024
	static __int64 fireproximityprompt = aslr(0x16FAFB0); //11.12.2024
	static __int64 scriptcontext_resume = aslr(0xD530C0); //11.12.2024

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
		static __int64 script = 0x50;
		static __int64 script_context = 504;
	}

	namespace Bytecode
	{
		static __int64 localscript = 448;
		static __int64 modulescript = 360;
	}

	namespace BaseScript
	{
		static __int64 ScriptNode = 0x198;
	}

	namespace ModuleScript
	{
		static __int64 RequireFlag = 0x1b0;
		static __int64 EnableLoadModule = aslr(0x524F208);
	}

	namespace Instance
	{
		static __int64 self = 0x8;
		static __int64 ClassDescriptor = 0x18;
		static __int64 ClassName = 0x8;
		static __int64 Name = 0x68;
		static __int64 Children = 0x70;
		static __int64 Size = 0x8;
		static __int64 Parent = 0x50;
		static __int64 Primitive = 352;
	}
}




namespace Defs
{
	using impersonator_t = void(__fastcall*)(std::int64_t*, std::int32_t*, std::int64_t);
	static impersonator_t rbx_impersonator = reinterpret_cast<impersonator_t>(Offsets::impersonator);

	using getcallback_t = __int64*(__fastcall*)(__int64 a1, __int64* prop_name);
	static getcallback_t rbx_getcallback = reinterpret_cast<getcallback_t>(Offsets::get_callback_prop);

	using print_t = void(__fastcall*)(int type, const char* source, ...);
	static print_t rbx_print = reinterpret_cast<print_t>(aslr(Offsets::print));


	using pushinstance2_t = void(__fastcall*)(lua_State* state, void* instance);
	static pushinstance2_t rbx_pushinstance2 = reinterpret_cast<pushinstance2_t>(Offsets::pushinstance);

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

	typedef enum { SUCCESS, YIELD, ERR } result_t;


	using scriptcontext_resume_t = result_t(__thiscall*)(std::int64_t scriptcontext_inst, debugger_result_t*,
		weak_thread_ref_t2**, int32_t narg,
		bool resumeError, const char* szErrorMessage);
	static scriptcontext_resume_t scriptcontext_resume = reinterpret_cast<scriptcontext_resume_t>(Offsets::scriptcontext_resume);
}


static __int64 menu_job = 0;

namespace TaskScheduler
{
	__forceinline static auto get_jobs() -> std::vector<__int64>
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

	__forceinline static __int64 get_jobs_by_name(std::string name)
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

	__forceinline static std::vector<__int64> get_all_jobs_by_name(std::string name)
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

	__forceinline static __int64 get_script_context()
	{
		__int64 needed;
		auto jobs = get_all_jobs_by_name("WaitingHybridScriptsJob");
		for (int i = 0; i < jobs.size(); i++)
		{
			if (jobs[i] != blacklisted_job)
				return *reinterpret_cast<__int64*>(jobs[i] + 504);
		}
	}



	__forceinline static __int64 get_lua_state()
	{
		__int64 script_context = get_script_context();

		__int64 arg1 = 0;
		__int64 arg2 = 0;
		__int64 state = Defs::rbx_getstate(script_context + 280, &arg1, &arg2);

		auto part = Defs::decryptstate(state + 136);
		return part;
	}

	__forceinline static bool is_loaded()
	{
		int counter = get_all_jobs_by_name("WaitingHybridScriptsJob").size();
		if (counter == 2)
			return true;
		else
			return false;
	}

	__forceinline static void set_fps(double fps)
	{
		static const double min_frame_delay = 1.0 / 10000.0;
		double frame_delay = fps <= 0.0 ? min_frame_delay : 1.0 / fps;

		*reinterpret_cast<double*>(Defs::rbx_getscheduler() + 0x178) = frame_delay;
	}

	__forceinline static int get_fps()
	{
		return 1 / *reinterpret_cast<double*>(Defs::rbx_getscheduler() + 0x178);
	}
}


namespace ProtoThings
{

	__forceinline static void raise_proto(Proto* proto)
	{
		proto->userdata = &max_capabilities;
		for (auto i = 0; i < proto->sizep; i++) {
			raise_proto(proto->p[i]);
		}
	}
}


namespace Identity
{
	__forceinline static auto set_context(__int64 state, int level) -> void
	{
		uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

		*reinterpret_cast<uintptr_t*>(userdata + 0x30) = level;
		*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;
	}

	__forceinline static auto get_context(__int64 state) -> __int64
	{
		uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

		return *reinterpret_cast<uintptr_t*>(userdata + 0x30);
	}
}
