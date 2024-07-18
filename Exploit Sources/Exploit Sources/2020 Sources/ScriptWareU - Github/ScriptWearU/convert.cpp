#include "gui.h"
#include "memscan.h"
#include <Shlwapi.h>
#include <mutex>
#include <queue>
extern "C"
{
#include "Lua/ldo.h"
#include "Lua/llex.h"
#include "Lua/lundump.h"
}
#include <list>
#include "Spoofer.h"

static bool scan_finished = false;

#define R_PROTO_OBFUSCATE(p, v) *(int*)(int)(p) = (int)(v) + (int)(p) // updated (hopefully)
#define RL_CLOSURE_OBFUSCATE(p, v) *(int*)(int)(p) = (int)(v) + (int)(p) // updated (hopefully)
#define RL_CLOSURE_DEOBFUSCATE(v) (*(int*)(v) - (int)(v));

static __forceinline Instruction RetEncrypt(Instruction inst, uint32_t pc)
{
	uint32_t res = 0;

	for (uint32_t mask = 1; mask; mask <<= 1)
	{
		uint32_t x = res * pc - (0x541B9F);
		uint32_t y = res * (pc + (0x1F3D8AF)) - (0x1C0BCC3);

		if ((mask & (x ^ y)) != (mask & inst))
			res = res | mask;
	}

	return res;
}

static __forceinline uint32_t CallClosureEncryption(Instruction instr, uint32_t pc, int a, int b, int c)
{
	uint32_t res = 0;

	for (unsigned int mask = 1; mask; mask <<= 1)
	{
		auto x = pc + res * a;
		auto y = pc + res * b + c;

		if ((mask & (x ^ y)) != (mask & instr))
			res = res | mask;
	}

	return res;
}

/* Abuse double rot bijection (still have to bruteforce ): ) */
static __forceinline Instruction InverseRot(Instruction Inst, int rotr, int rotl)
{
	auto res = Inst;

	for (uint8_t i = 0; i < 5; i++)
	{
		res = res ^ _rotr(res, rotr) ^ _rotl(res, rotl);
		rotr = rotr * 2 % 32;
		rotl = rotl * 2 % 32;
	}

	return res;
}

static __forceinline Instruction JumpEncode(Instruction Inst, uint32_t PC)
{
	return InverseRot(Inst ^ (0xFFFFD8AF) * PC & (0x3FFFF), 6, 9);
}

static __forceinline Instruction JumpDecode(Instruction Inst, uint32_t PC)
{
	return Inst ^ ((0xFFFFD8AF) * PC & (0x3FFFF) ^ (_rotr(Inst, 6) ^ _rotl(Inst, 9)));
}

static __forceinline Instruction JumpEncryption(Instruction Inst, uint32_t PC)
{
	for (auto i = 0; i <= MAXARG_A; i++)
	{
		auto NewInstr = Inst;
		RSETARG_A(NewInstr, i);

		auto Sol = JumpEncode(NewInstr, PC);
		RSET_OPCODE(Sol, ReadROpcode(OP_JMP));

		if (NewInstr == JumpDecode(Sol, PC))
			return Sol;
	}

	return NULL;
}

static __forceinline Instruction SetUpvalEncode(Instruction Inst, uint32_t PC)
{
	return InverseRot(Inst ^ (0x961C86) * PC & (0x3FC01FF), 7, 13);
}

static __forceinline Instruction SetUpvalDecode(Instruction Inst, uint32_t PC)
{
	return Inst ^ ((0x961C86) * PC & (0x3FC01FF) ^ (_rotr(Inst, 7) ^ _rotl(Inst, 13)));
}

static __forceinline Instruction SetUpvalEncryption(Instruction Inst, uint32_t PC)
{
	for (auto i = 0; i <= MAXARG_C; i++)
	{
		auto NewInstr = Inst;
		RSETARG_C(NewInstr, i);

		auto Sol = SetUpvalEncode(NewInstr, PC);
		RSET_OPCODE(Sol, ReadROpcode(OP_SETUPVAL));
		
		if (NewInstr == SetUpvalDecode(Sol, PC))
			return Sol;
	}

	return NULL;
}


namespace lua_to_roblox
{
	void convert_proto(int rl, Proto* p, r_proto* rp) {
		auto source = RLUAS_NEW(rl, getstr(p->source));
		R_PROTO_OBFUSCATE(&rp->source, source);

		rp->sizep = p->sizep;
		auto** rpp_mem = (r_proto**)RLUAM_MALLOC(rl, sizeof(r_proto**) * p->sizep);
		R_PROTO_OBFUSCATE(&rp->p, rpp_mem);

		for (int k = 0; k < p->sizep; k++) {
			rpp_mem[k] = rluaF_newproto(rl);
			convert_proto(rl, p->p[k], rpp_mem[k]);
		}

		rp->sizek = p->sizek;
		auto* rp_k_mem = (r_tvalue*)RLUAM_MALLOC(rl, sizeof(r_tvalue) * p->sizek);
		R_PROTO_OBFUSCATE(&rp->k, rp_k_mem);

		for (int k = 0; k < p->sizek; k++) {
			TValue* o = &p->k[k];
			r_tvalue* ro = &rp_k_mem[k];
			switch (o->tt) {
			case LUA_TNIL:
				ro->tt = RLUA_TNIL;
				ro->value.n = 0;
				break;
			case LUA_TBOOLEAN:
				ro->tt = RLUA_TBOOLEAN;
				ro->value.b = o->value.b;
				break;
			case LUA_TNUMBER:
				ro->tt = RLUA_TNUMBER;
				ro->value.n = r_xor_number(&o->value.n);
				break;
			case LUA_TSTRING:
				ro->tt = RLUA_TSTRING;
				ro->value.gc = rluaS_newlstr(rl, getstr((TString*)o->value.gc), ((TString*)o->value.gc)->tsv.len);
				break;
			default:
				break;
			}
		}

		rp->sizecode = p->sizecode;
		auto* rp_code_mem = (int*)RLUAM_MALLOC(rl, sizeof(int*) * p->sizecode);
		R_PROTO_OBFUSCATE(&rp->code, rp_code_mem);

		for (int k = 0; k < p->sizecode; k++) {
			/* instruction conversion */

			Instruction inst = p->code[k]; // vanilla instruction
#ifdef SYN
			if (GET_OPCODE(inst) == OP_TAILCALL)
				SET_OPCODE(inst, OP_CALL);
#endif
			int r_inst = 0;
			OpCode op = GET_OPCODE(inst);

			RSET_OPCODE(r_inst, get_roblox_opcode[op]);

			switch (getOpMode(op)) {
			case iABC:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_B(r_inst, GETARG_B(inst));
				RSETARG_C(r_inst, GETARG_C(inst));
				break;
			case iABx:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_Bx(r_inst, GETARG_Bx(inst));
				break;
			case iAsBx:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_sBx(r_inst, GETARG_sBx(inst));
				break;
			}

			/* enc has been added this time! includes eternals really shit wrong encryption he gave everyone as an attempt to seem smarter in the eyes of others */
			switch (op) {
#ifdef SYN
			case OP_JMP:
				r_inst = JumpEncryption(r_inst, k);
				RSET_OPCODE(r_inst, ReadROpcode(op));
				break;

			case OP_CALL:
				r_inst = CallClosureEncryption(r_inst, k, (0x1F3D8AF), (int)(-0x1C6B438), (int)(-0x541B9F));
				RSET_OPCODE(r_inst, ReadROpcode(op));
				
				break;

			case OP_RETURN:
				r_inst = RetEncrypt(r_inst, k);
				RSET_OPCODE(r_inst, ReadROpcode(op));
				break;

			case OP_CLOSURE:
				r_inst = CallClosureEncryption(r_inst, k, (int)(0x961C86), (int)(0x1F3D8AF), (int)(-0x1C0BCC3));
				RSET_OPCODE(r_inst, ReadROpcode(op));
				break;

			case OP_SETUPVAL:
				r_inst = SetUpvalEncryption(r_inst, k);
				RSET_OPCODE(r_inst, ReadROpcode(op));
				break;
			case OP_MOVE:
				RSETARG_C(r_inst, 1);
				break;
#else
			case OP_JMP:
				r_inst = r_inst >> 26 << 26 | jump_encryption(r_inst, k);
				break;
			case OP_CALL:
				r_inst = r_inst >> 26 << 26 | call_enc(r_inst, k);
				break;
			case OP_TAILCALL:
			case OP_RETURN:
				r_inst = r_inst >> 26 << 26 | dax_encode_op(r_inst, k, 1470882913, k - 1577854801, 641680189) & 0x3FFFFFF;
				break;
			case OP_CLOSURE:
				r_inst = r_inst >> 26 << 26 | closure_enc(r_inst, k);
				break;
			case OP_SETUPVAL:
				r_inst = r_inst >> 26 << 26 | setupval_encryption(r_inst, k);
			case OP_MOVE:
				r_inst = r_inst & 0xFFFC21FF | 0x2000;
				break;
#endif
			}

			rp_code_mem[k] = r_inst * encode_key;

			// TODO: test
			if (GET_OPCODE(inst) == OpCode::OP_SETLIST && GETARG_C(inst) == 0)
			{
				std::cout << "SETLIST PSUEDO INSTR\n";
				++k;
				rp_code_mem[k] = p->code[k];
			}
		}

		// sizelocvars set
		rp->sizelineinfo = p->sizelineinfo;
		auto* rp_lineinfo_mem = (int*)RLUAM_MALLOC(rl, sizeof(int*) * p->sizelineinfo);
		R_PROTO_OBFUSCATE(&rp->lineinfo, rp_lineinfo_mem);

		for (int k = 0; k < p->sizelineinfo; k++)
			rp_lineinfo_mem[k] = p->lineinfo[k];

		// sizeupvalues set
		rp->maxstacksize = p->maxstacksize;
		rp->is_vararg = p->is_vararg;
		rp->nups = p->nups;
		rp->numparams = p->numparams;
	}

	VOID set_l_closure(int rl, LClosure* lcl) {
		Proto* p = lcl->p;
		r_proto* rp = rluaF_newproto(rl);

		// luaU_print(p, false);

		convert_proto(rl, p, rp);
		rl_closure* rlcl = rluaF_newLclosure(rl, 0, *(int*)(rl + RL_L_GT), (int)rp);
		RL_CLOSURE_OBFUSCATE(&rlcl->p, rp);

		rlua_pushlclosure(rl, rlcl);
	}

	VOID call_l_closure(int rl, LClosure* lcl) {
		set_l_closure(rl, lcl);
		auto tv = r_lua_index2adr(rl, -1);
		r_spawn(rl);
	}

	VOID execute_script(int r_thread, lua_State* l, std::string source) {
		decode_key = GET_CKEY(r_thread);
		set_encode_key(r_thread, &encode_key);

		r_lua_getfield(r_thread, LUA_GLOBALSINDEX, "Instance");
		r_lua_getfield(r_thread, -1, "new");
		r_lua_pushstring(r_thread, "LocalScript");
		r_lua_pcall(r_thread, 1, 1, 0);

		auto scriptInstance = rlua_touserdata(r_thread, -1);
		r_sandbox_thread(r_thread, 6, (void*)scriptInstance);

		r_lua_setfield(r_thread, LUA_GLOBALSINDEX, "script");

		r_lua_pushboolean(r_thread, true);
		r_lua_setfield(r_thread, LUA_GLOBALSINDEX, "ScriptWear Thread");

		r_lua_settop(r_thread, 0);

		if (luaL_loadstring(l, BS_SYNAPSE, source.c_str())) {
			print_output(RGB(255, 20, 20), "%s\n", lua_tostring(l, -1));
			lua_pop(l, 1);
		}

		else {
			util::pause();
			TValue* o = (l->top) - 1;
			LClosure* lcl = &clvalue(o)->l;
			call_l_closure(r_thread, lcl);
			lua_pop(l, 1);
			util::resume();
		}

		printf("\nlstack: %d\n", lua_gettop(l));
		printf("rstack: %d\n", RLUA_GETTOP(r_thread));
	}
}

#ifdef _DEBUG
struct Job
{
	void* vft;
	byte padding[8];
	std::string name;
};
#else
struct Job
{
	void* vft;
	byte padding[12];
	std::string name;
};
#endif

enum class StepResult : int
{
	Done,	        // The job will be removed from the TaskScheduler
	Stepped,        // Another step will be scheduled
};

template<typename T>
class safe_queue
{
protected:
	std::queue<T> queue;

	std::mutex m;
public:
	void clear()
	{
		std::scoped_lock lock(m);

		while (!queue.empty())
			queue.pop();
	}
	void push(const T& value)
	{
		std::scoped_lock lock(m);
		queue.push(value);
	}
	bool pop_if_present(T& value)
	{
		std::scoped_lock lock(m);
		if (!queue.empty())
		{
			value = queue.front();
			queue.pop();
			return true;
		}
		else
			return false;
	}

	bool pop_if_present()
	{
		std::scoped_lock lock(m);
		if (!queue.empty())
		{
			queue.pop();
			return true;
		}
		else
			return false;
	}

	// WARNING: Peeking has side effects, if T has copy constructors and destructors
	bool peek_if_present(T& value)
	{
		std::scoped_lock lock(m);
		if (!queue.empty())
		{
			value = queue.front();
			return true;
		}
		else
			return false;
	}

	// Lock and spin-free calls:
	inline size_t size() const { return queue.size(); }
	inline bool empty() const { return queue.empty(); }
};

int GLOBALRL;
lua_State* GLOBALL;
safe_queue<std::string> scriptQueue;

#pragma pack(push, 1)

typedef struct _JMP_REL
{
	UINT8  opcode;      // E9/E8 xxxxxxxx: JMP/CALL +5+xxxxxxxx
	UINT32 operand;     // Relative destination address
} JMP_REL, * PJMP_REL, CALL_REL;

#pragma pack(pop)

void fakeChain(DWORD* chain)
{
	chain[1] = 0x1555555;
	((DWORD*)chain[0])[1] = 0x1555555;
}

void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
{
	chain[1] = unk;
	((DWORD*)chain[0])[1] = nextUnk;
}

rluaD_precall_def oLuauPrecall = (rluaD_precall_def)Retcheck::clone((BYTE*)addresses::luau_precall);
int __cdecl luauPrecallHook(int rL, r_tvalue* func, int nres)
{
	bool wasOurs = false;

	auto top = RLUA_GETTOP(rL);
	r_lua_getfield(rL, LUA_GLOBALSINDEX, "ScriptWear Thread");
	if (r_lua_index2adr(rL, -1)->tt == RLUA_TBOOLEAN)
		wasOurs = true;
	r_lua_settop(rL, top);

	if (wasOurs)
	{
		auto start = *(DWORD*)(RobloxBase + 0x21A601C - IdaBase);
		if (r_luaD_precall(rL, func, nres) == 0)
		{
			DWORD* exceptionChain = (DWORD*)__readfsdword(0);
			DWORD unk = exceptionChain[1], nextUnk = ((DWORD*)exceptionChain[0])[1];
			fakeChain(exceptionChain);
			// r_luaV_execute(rL, 1);
			//typedef void(__fastcall* meme_def)(int, int);
			//meme_def meme = (meme_def)RebaseAddress(0x42A5E4);
			//meme(rL, 1);
			syn::SpoofCall<void>(syn::CallingConvention::CDECL, (void*)addresses::luaV_execute, rL, 1);

			restoreChain(exceptionChain, unk, nextUnk);
			*(DWORD*)(RobloxBase + 0x21B30E4 - IdaBase) = 0; // exception chain check
			// retcheck
			// *(DWORD*)(RobloxBase + 0x21AB770 - IdaBase) &= ~(1 << 21);
			//if (*(DWORD*)(RobloxBase + 0x21A601C - IdaBase) != start)
			//{
			//	std::cout << "potential retcheck detection\n";
			//}

		}
		return 1;
	}

	return oLuauPrecall(rL, func, nres);
}

rlua_resume_def oLuaResume = (rlua_resume_def)Retcheck::clone((BYTE*)addresses::lua_resume);
int __cdecl luaResumeHook(int rL, int nargs)
{
	bool wasOurs = false;

	auto top = RLUA_GETTOP(rL);
	r_lua_getfield(rL, LUA_GLOBALSINDEX, "ScriptWear Thread");
	if (r_lua_index2adr(rL, -1)->tt == RLUA_TBOOLEAN)
		wasOurs = true;
	r_lua_settop(rL, top);

	if (wasOurs)
	{
		*(BYTE*)(rL + *(DWORD*)(rL + 20) + 20) = 0;
	}

	auto start = *(DWORD*)(RobloxBase + 0x21A601C - IdaBase);

	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1], nextUnk = ((DWORD*)exceptionChain[0])[1];
	fakeChain(exceptionChain);
	auto ret = oLuaResume(rL, nargs);
	restoreChain(exceptionChain, unk, nextUnk);

	if (wasOurs)
	{
		*(BYTE*)(rL + *(DWORD*)(rL + 20) + 20) = 1;
		*(DWORD*)(RobloxBase + 0x21B30E4 - IdaBase) = 0;
	}

	return ret;
}

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

// aids amiright
std::string initScript = R"skidded(do local a=Instance.new;Instance={}function Instance.new(b,c)local d=a(b)nigga,d.Parent=pcall(function()return c end)return d end;setmetatable(Instance,{__newindex=function()error'Attempt to modify a readonly table'end})local e=getmetatable;function getmetatable(f)if f==Instance then return nil end;return e(f)end;WEAR_INITIALIZED=true end )skidded";

StepResult(__fastcall* stepDataModelOriginal)(uintptr_t this_, void* junk, int stats);
StepResult __fastcall stepDataModelHook(uintptr_t this_, void* junk, int stats)
{
	std::string source;
	size_t count = scriptQueue.size();
	while (count-- > 0 && scriptQueue.pop_if_present(source))
	{
		int r_thread = r_lua_newthread(GLOBALRL);
		r_lua_setfield(GLOBALRL, LUA_REGISTRYINDEX, random_string(16).c_str());
		lua_to_roblox::execute_script(r_thread, GLOBALL, initScript + source);
	}

	auto ret = stepDataModelOriginal(this_, junk, stats);

	return ret;
}

StepResult(__fastcall* stepDataModelOriginalMCJ)(uintptr_t this_, void* junk, int stats);
StepResult __fastcall stepDataModelHookMCJ(uintptr_t this_, void* junk, int stats)
{
	static PJMP_REL precallBackup;
	static PJMP_REL resumeBackup;

	static std::once_flag once;
	std::call_once(once, []()
		{
			precallBackup = new JMP_REL;
			memcpy(precallBackup, (void*)addresses::luau_precall, sizeof(JMP_REL));

			resumeBackup = new JMP_REL;
			memcpy(resumeBackup, (void*)addresses::lua_resume, sizeof(JMP_REL));
		});

	DWORD oldProtect;

	VirtualProtect((void*)addresses::luau_precall, sizeof(JMP_REL), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)addresses::luau_precall, precallBackup, sizeof(JMP_REL));
	VirtualProtect((void*)addresses::luau_precall, sizeof(JMP_REL), oldProtect, &oldProtect);

	VirtualProtect((void*)addresses::lua_resume, sizeof(JMP_REL), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)addresses::lua_resume, resumeBackup, sizeof(JMP_REL));
	VirtualProtect((void*)addresses::lua_resume, sizeof(JMP_REL), oldProtect, &oldProtect);

	auto ret = stepDataModelOriginalMCJ(this_, junk, stats);

	VirtualProtect((void*)addresses::luau_precall, sizeof(JMP_REL), PAGE_EXECUTE_READWRITE, &oldProtect);
	PJMP_REL pJmp = (PJMP_REL)addresses::luau_precall;
	pJmp->opcode = 0xE9;
	pJmp->operand = (UINT32)((LPBYTE)&luauPrecallHook - ((LPBYTE)addresses::luau_precall + sizeof(JMP_REL)));
	VirtualProtect((void*)addresses::luau_precall, sizeof(JMP_REL), oldProtect, &oldProtect);

	VirtualProtect((void*)addresses::lua_resume, sizeof(JMP_REL), PAGE_EXECUTE_READWRITE, &oldProtect);
	pJmp = (PJMP_REL)addresses::lua_resume;
	pJmp->opcode = 0xE9;
	pJmp->operand = (UINT32)((LPBYTE)&luaResumeHook - ((LPBYTE)addresses::lua_resume + sizeof(JMP_REL)));
	VirtualProtect((void*)addresses::lua_resume, sizeof(JMP_REL), oldProtect, &oldProtect);

	return ret;
}

int initializeWSJ(lua_State* L)
{
	GLOBALL = L;
	auto taskScheduler = *(DWORD*)(RobloxBase + 0x2270198 - 0x400000);
	auto it = *(DWORD*)(taskScheduler + 0x144);
	auto end = *(DWORD*)(taskScheduler + 0x148);

	Job* waitingScriptsJob = nullptr;
	Job* memoryCheckerJob = nullptr;
	if (it != end)
	{
		while (true)
		{
			auto job = *(Job**)it;
			std::cout << job->name << "\n";
			if (job->name == "WaitingScriptJob")
			{
				waitingScriptsJob = job;
			}
			else if (job->name == "US14116")
			{
				memoryCheckerJob = job;
			}

			if (waitingScriptsJob && memoryCheckerJob)
				break;

			it += 8;
			if (it == end)
				break;
		}
	}

	if (!waitingScriptsJob)
	{
		std::cerr << "could not find WaitingScriptsJob\n";
		return 0;
	}

	if (!memoryCheckerJob)
	{
		std::cerr << "could not find MemoryCheckerJob\n";
		return 0;
	}

	{
		size_t vftSize = 4 * 5;
		uintptr_t* newVft = (uintptr_t*)malloc(vftSize);
		memcpy(newVft, waitingScriptsJob->vft, vftSize);

		stepDataModelOriginal = (decltype(stepDataModelOriginal))newVft[4];
		newVft[4] = (uintptr_t)stepDataModelHook;

		waitingScriptsJob->vft = newVft;
		std::cout << "done vft hook 1\n";
	}

	{
		size_t vftSize = 4 * 5;
		uintptr_t* newVft = (uintptr_t*)malloc(vftSize);
		memcpy(newVft, memoryCheckerJob->vft, vftSize);

		stepDataModelOriginalMCJ = (decltype(stepDataModelOriginalMCJ))newVft[4];
		newVft[4] = (uintptr_t)stepDataModelHookMCJ;

		memoryCheckerJob->vft = newVft;
		std::cout << "done vft hook 2\n";
	}

	return *(int*)((int)waitingScriptsJob + 384);
}

void execute_script(int rL, lua_State* L, const std::string &source) {

	GLOBALRL = rL;
	scriptQueue.push(source);
}