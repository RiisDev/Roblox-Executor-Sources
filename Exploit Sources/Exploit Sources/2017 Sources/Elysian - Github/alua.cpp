#include "alua.h"
#include <Windows.h>
#include <queue>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "VMProtectSDK.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
// lua internals 
#include "lualib.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lobject.h"
#include "lfunc.h"
}
#include "memutil.h"
#include "form.h"
#include "rinstance.h"
#include "elysian.h"

// --- globals --- \\

lua_State*	L;
int			r_L;
int			script_context;

// --- roblox internal definitions --- \\

typedef unsigned char byte;

const char RBXL_OPNAMES[NUM_OPCODES] {
	0x06, /* MOVE		00 */
	0x04, /* LOADK		01 */
	0x00, /* LOADBOOL	02 */
	0x07, /* LOADNIL	03 */
	0x02, /* GETUPVAL	04 */
	0x08, /* GETGLOBAL	05 */
	0x01, /* GETTABLE	06 */
	0x03, /* SETGLOBAL	07 */
	0x05, /* SETUPVAL	08 */
	0x0F, /* SETTABLE	09 */
	0x0D, /* NEWTABLE	10 */
	0x09, /* SELF		11 */
	0x10, /* ADD		12 */
	0x0B, /* SUB		13 */
	0x11, /* MUL		14 */
	0x0A, /* DIV		15 */
	0x0C, /* MOD		16 */
	0x0E, /* POW		17 */
	0x18, /* UNM		18 */
	0x16, /* NOT		19 */
	0x12, /* LEN		20 */
	0x19, /* CONCAT		21 */
	0x14, /* JMP		22 */
	0x1A, /* EQ			23 */
	0x13, /* LT			24 */
	0x15, /* LE			25 */
	0x17, /* TEST		26 */
	0x21, /* TESTSET	27 */
	0x1F, /* CALL		28 */
	0x1B, /* TAILCALL	29 */
	0x22, /* RETURN		30 */ // In the compiled code, this is at the end.
	0x1D, /* FORLOOP	31 */
	0x23, /* FORPREP	32 */
	0x1C, /* TFORLOOP	33 */
	0x1E, /* SETLIST	34 */
	0x20, /* CLOSE		35 */
	0x25, /* CLOSURE	36 */
	0x24, /* VARARG		37 */
};

#define RLUA_TNIL			0
#define RLUA_TLIGHTUSERDATA	1
#define RLUA_TNUMBER		2
#define RLUA_TBOOLEAN		3
#define RLUA_TSTRING		4
#define RLUA_TTABLE			7
#define RLUA_TUSERDATA		8
#define RLUA_TFUNCTION		6
#define RLUA_TTHREAD		5
#define RLUA_TPROTO			9
#define RLUA_TUPVAL			10

typedef struct r_LClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int *p;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_CClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int* f;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_TString {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	unsigned int len;			// +08	08
	unsigned int hash;			// +12  0C
};

typedef struct r_Udata {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	int* env;					// +08	08
	unsigned int len;			// +12	0C
	int* metatable;				// +16	10
	int unk;					// +20	14
};

typedef struct r_LocVar {
	r_TString *varname;
	int startpc;  /* first point where variable is active */
	int endpc;    /* first point where variable is dead */
};

typedef struct r_TValue {
	int value_0;
	int value_1;
	int tt;
	int unk;
};

typedef struct r_Proto {
	int* next;					// +00	00
	byte marked;				// +04	04
	byte tt;					// +05	05
	byte unk_0;					// +06	06
	byte unk_1;					// +07	07
	struct r_Proto** p;			// +08	08
	r_TString* source;			// +12	0C
	struct r_LocVar *locvars;	// +16	10
	int unk_3;					// +20	14
	int* code;					// +24	18
	int sizek;					// +28	1C
	r_TValue* k;				// +32	20
	int* lineinfo;				// +36	24 ???
	int unk_5;					// +40	28
	int sizep;					// +44	2C
	int sizelocvars;			// +48	30
	int sizecode;				// +52	34
	int unk_6;					// +56	38
	int unk_7;					// +60	3C
	int sizelineinfo;			// +64	40 ???
	int unk_9;					// +68	44
	byte maxstacksize;			// +72	48
	byte numparams;				// +73	49
	byte nups;					// +74	4A
	byte is_vararg;				// +75	4B
};

// --- core util declarations --- \\

int get_rbxl_opcode(OpCode opcode);
OpCode get_lua_opcode(int opcode);
int decrypt_ptr(int loc);
int encrypt_ptr(int loc, void* value);
int convert_lua_instruction(Instruction inst);
r_Proto* convert_lua_proto(int r_lua_State, Proto* p, const char* script_loc);

// --- constants --- \\

int ud_hook_location = 0x69B0;
__int64* lua_number_key = (__int64*)0x130B710;
int render_step_location = 0x18E7A0;
int ret_check = 0x144E80;
int ret_check_flag_0 = 0xEE3EA8;
int ret_check_flag_1 = 0x130D764;
int vm_shutdown_bp = 0x003D29B0;
int vm_hook_location = 0x003D29F5;
int trust_check_bp = 0x00269016;

typedef void*(__cdecl *r_luaM_realloc__Def)(int r_lua_State, int block, int osize, int nsize);
typedef r_Proto*(__cdecl *r_luaF_newproto_Def)(int r_lua_State);
typedef int(__cdecl *r_lua_newthread_Def)(int r_lua_State);
typedef int(__cdecl *r_lua_resume_Def)(int r_lua_State, int nargs);
typedef r_TString*(__cdecl *r_luaS_newlstr_Def)(int r_lua_State, const char* str, size_t l);
typedef void(__thiscall *openState_Def)(int scriptContext, int id);
typedef int(__cdecl *r_lua_pushcclosure_Def)(int r_lua_State, void* f, int n);
typedef void(__cdecl *r_lua_getfield_Def)(int r_lua_State, int idx, const char* k);
typedef int(__cdecl *r_lua_setmetatable_Def)(int r_lua_State, int idx);
typedef int(__thiscall *getContent_Def)(int self, int* buffer, std::string* content, int unk0, int unk1);
typedef int(__thiscall *loadInstances_Def)(int self, std::istream* file, std::vector<RBXInstance>* out);
typedef void*(__cdecl *r_lua_newuserdata_Def)(int r_lua_State, int sz);
typedef int(__cdecl *r_lua_sethook_Def)(int r_lua_State, void* f, int mask, int count);
typedef void(__cdecl *r_luaD_call_Def)(int r_lua_State, r_TValue* f, int nresults);
typedef int(__cdecl *GetGlobalState_Def)(int r_lua_State);
typedef void(__cdecl *SerializeInstance_Def)(std::istream& out, const std::vector<RBXInstance>& instances, int unk1);

SerializeInstance_Def	SerializeInstance = (SerializeInstance_Def)0x5F9E60;
r_luaD_call_Def			r_luaD_call = (r_luaD_call_Def)(0x1434E0);
r_lua_sethook_Def		r_lua_sethook = (r_lua_sethook_Def)(0x395020);

// --- other globals --- \\

// retrieved from server
r_lua_setfield_Def		r_lua_setfield = 0;
SetThreadIdentity_Def	SetThreadIdentity = 0;

// found by signature scanner
GetGlobalState_Def		GetGlobalState = 0;
r_lua_newuserdata_Def	r_lua_newuserdata = 0;
loadInstances_Def		loadInstances = 0;
getContent_Def			getContent = 0;
r_lua_pushcclosure_Def	r_lua_pushcclosure = 0;
r_lua_resume_Def		r_lua_resume = 0;
r_lua_newthread_Def		r_lua_newthread = 0;
r_luaF_newproto_Def		r_luaF_newproto = 0;
openState_Def			openState = 0;
r_luaS_newlstr_Def		r_luaS_newlstr = 0;
r_luaM_realloc__Def		r_luaM_realloc_ = 0;
r_lua_getfield_Def		r_lua_getfield = 0;
r_lua_setmetatable_Def	r_lua_setmetatable = 0;

#define r_luaS_new(L,c) (r_luaS_newlstr(L, c, strlen(c)))
#define getstateindex(sc, i) *(int*)(sc + 80 + 64 * i)

typedef int(*rlua_CFunction)(int r_lua_State);

// --- roblox lua c api + extensions --- \\

#define r_luaM_malloc(luastate,t)	r_luaM_realloc_(luastate, NULL, 0, (t))

void rtvalue_setn(r_TValue* tval, __int64 val) {
	*(__int64*)(&tval->value_0) = (__int64)val ^ (__int64)*lua_number_key;
}

double rtvalue_getn(r_TValue* val) {
	double n;
	__int64 n64 = *(__int64*)(&val->value_0) ^ (__int64)*lua_number_key;
	memcpy(&n, &n64, 8);

	return n;
}

// sets a to b
void r_setobj(r_TValue* a, r_TValue* b) {
	a->value_0 = b->value_0;
	a->value_1 = b->value_1;
	a->tt = b->tt;
	a->unk = b->unk;
}

void r_incr_top(int r_lua_State) {
	*(int*)(r_lua_State + 16) += sizeof(r_TValue);
}

void r_decr_top(int r_lua_State) {
	*(int*)(r_lua_State + 16) -= sizeof(r_TValue);
}

r_TValue* r_gettop(int r_lua_State) {
	return *(r_TValue**)(r_lua_State + 16);
}

void r_luaC_link(int r_lua_State, int o, byte tt) {
	int g = decrypt_ptr(r_lua_State + 8);	// global_State *g = G(L)
	*(int*)(o) = *(int*)(g + 48);			// o->gch.next = g->rootgc
	*(int*)(g + 48) = o;					// g->rootgc = o
	*(byte*)(o + 4) = *(byte*)(g + 21) & 3;	// o->gch.marked = luaC_white(g)
	*(byte*)(o + 5) = tt;					// o->gch.tt = tt;

}

r_LClosure* r_luaF_newLclosure(int r_lua_State, int nups, int e) {
	r_LClosure* nlc = (r_LClosure*)r_luaM_malloc(r_lua_State, 20 + nups * 4);
	r_luaC_link(r_lua_State, (int)nlc, RLUA_TFUNCTION);

	nlc->isC = false;
	nlc->env = (int*)e;
	nlc->nupvalues = nups;
	nlc->gclist = 0; // wat

	while (nups--) nlc->upvals[nups] = NULL;
	return nlc;
}

void r_lua_pushLclosure(int r_lua_State, r_LClosure* lc) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	top->value_0 = (int)lc;
	top->value_1 = 0;
	top->tt = RLUA_TFUNCTION;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

void r_lua_pushstring(int r_lua_State, const char* str) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	top->value_0 = (int)r_luaS_new(r_lua_State, str);
	top->value_1 = 0;
	top->tt = RLUA_TSTRING;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

int r_lua_gettop(int r_lua_State) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	r_TValue* base = *(r_TValue**)(r_lua_State + 28);

	return (int)(top - base);
}

r_TValue* r_getobjat(int r_lua_State, int off) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	r_TValue* base = *(r_TValue**)(r_lua_State + 28);

	if (off > r_lua_gettop(r_lua_State)) {
		return 0;
	}

	return base + (off - 1);
}

void r_lua_pushnumber(int r_lua_State, lua_Number n) {
	r_TValue* top = r_gettop(r_lua_State);

	__int64 val;
	memcpy(&val, &n, sizeof(__int64));
	rtvalue_setn(top, val);

	top->tt = RLUA_TNUMBER;
	top->unk = 0;
	r_incr_top(r_lua_State);
}

/* * `index` must be above 0 */
lua_Number r_tonumber(int r_lua_State, int index) {
	r_TValue* obj = r_getobjat(r_lua_State, index);
	if (obj->tt != RLUA_TNUMBER) return 0;

	double n;
	__int64 n64 = rtvalue_getn(obj);
	memcpy(&n, &n64, 8);

	return n;
}

const char* r_tostring(int r_lua_State, int index) {
	r_TValue* obj = r_getobjat(r_lua_State, index);
	if (obj->tt != RLUA_TSTRING) return 0;

	return (const char*)(((r_TString*)obj->value_0) + 1);
}

/* has a 256 character limit */
void r_lua_pushvfstring(int r_lua_State, const char* fmt, ...) {
	char message[256];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf_s(message, 256, fmt, vl);
	va_end(vl);

	r_lua_pushstring(r_lua_State, message);
}

void r_lua_pushnil(int r_lua_State) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	top->value_0 = 0;
	top->value_1 = 0;
	top->tt = RLUA_TNIL;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

void r_lua_pushboolean(int r_lua_State, int b) {
	r_TValue* top = r_gettop(r_lua_State);
	top->value_0 = b;
	top->value_1 = 0;
	top->tt = RLUA_TBOOLEAN;
	top->unk = 0;
	r_incr_top(r_lua_State);
}

/* gets metatable from last object on stack */
int r_lua_getmetatable(int r_lua_State) {
	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* mt = top - 1;
	int table;

	switch (mt->tt) {
	case RLUA_TTABLE:
		table = mt->value_0 + 28;
		break;
	case RLUA_TUSERDATA:
		table = mt->value_0 + 16;
		break;
	default:
		return 0;
	}

	table = decrypt_ptr(table);
	if (!table)
		return 0;

	top->value_0 = table;
	top->value_1 = 0;
	top->tt = RLUA_TTABLE;
	top->unk = 0;
	r_incr_top(r_lua_State);

	return 1;
}

int wrap_object(int r_lua_State, int inst, int ext) {
	if (inst) {
		int* ud = (int*)r_lua_newuserdata(r_lua_State, 8);
		*ud = inst;
		*(ud + 1) = ext;

		r_lua_getfield(r_lua_State, -10000, "Object");
		r_lua_setmetatable(r_lua_State, -2);
		return 1;
	}

	r_lua_pushnil(r_lua_State);
	return 1;
}

void* r_getudata(int r_lua_State, int idx) {
	if (!idx) idx = r_lua_gettop(r_lua_State);
	r_TValue* obj = r_getobjat(r_lua_State, idx);
	if (obj->tt != RLUA_TUSERDATA)
		return 0;

	return (void*)(obj->value_0 + 24);
}

void r_lua_settop(int r_lua_State, int idx) {
	if (idx >= 0) {
		while (*(r_TValue**)(r_lua_State + 16) < *(r_TValue**)(r_lua_State + 28) + idx) {
			(*(r_TValue**)(r_lua_State + 16))->tt = RLUA_TNIL;
			*(r_TValue**)(r_lua_State + 16) += 1;
		}

		*(r_TValue**)(r_lua_State + 16) = *(r_TValue**)(r_lua_State + 28) + idx;

	}
	else {
		*(r_TValue**)(r_lua_State + 16) += idx + 1;
	}
}

#define r_lua_pop(r, n) r_lua_settop(r, -(n)-1)

// --- thread scheduler declarations --- \\

typedef struct ThreadRef {
	int state;
	std::string code;
	std::string source;
	RBXInstance instance;
};

class ThreadScheduler {
private:
	std::queue<ThreadRef>* queue;
	unsigned char initiated;
public:
	ThreadScheduler();
	~ThreadScheduler();
	int GetQueueSize();
	std::queue<ThreadRef>* GetQueue();
	void Queue(int r_lua_State, std::string code, std::string source, int inst, int ext);
	void Start();
	void Step();
};

ThreadScheduler tsched;

// --- core utilities --- \\


#define RSET_OPCODE(i,o)	((i) = (((i) & MASK0(6, 26)) | \
		(((Instruction)o << 26) & MASK1(6, 26))))
#define RGET_OPCODE(i)		(i >> 26 & MASK1(6, 0))

#define RSETARG_A(i,o)		((i) = (((i) & MASK0(8, 18)) | \
		(((Instruction)o << 18) & MASK1(8, 18))))
#define RGETARG_A(i)		(i >> 18 & MASK1(8, 0))

#define RSETARG_B(i,o)		((i) = (((i) & MASK0(9, 0)) | \
		(((Instruction)o << 0) & MASK1(9, 0))))
#define RGETARG_B(i)		(i >>  0 & MASK1(9, 0))

#define RSETARG_C(i,o)		((i) = (((i) & MASK0(9, 9)) | \
		(((Instruction)o << 9) & MASK1(9, 9))))
#define RGETARG_C(i)		(i >>  9 & MASK1(9, 0))

#define RSETARG_Bx(i,b)		((i) = (((i) & MASK0(18, 0)) | \
		(((Instruction)b << 0) & MASK1(18, 0))))
#define RGETARG_Bx(i)		(i >>  0 & MASK1(18, 0))

#define RSETARG_sBx(i,b)	RSETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))
#define RGETARG_sBx(i)		(RGETARG_Bx(i)-MAXARG_sBx)

void random_string(char* b, int sz) {
	const char* characters =
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < sz; i++) {
		b[i] = characters[rand() % (strlen(characters) + 1)];
	}

	b[sz] = 0;
}

int get_rbxl_opcode(OpCode opcode) {
	return RBXL_OPNAMES[opcode];
}

OpCode get_lua_opcode(int opcode) {
	for (int i = 0; i < NUM_OPCODES; i++) {
		if (get_rbxl_opcode((OpCode)i) == opcode) {
			return (OpCode)i;
		}
	}
	return (OpCode)-1;
}

int decrypt_ptr(int loc) {
	return *(int*)loc + loc;
}

int encrypt_ptr(int loc, void* value) {
	*(int*)loc = (int)value - loc;
	return loc;
}

int convert_lua_instruction(Instruction inst) {
	int rtn = ~0;

	OpCode opcode = GET_OPCODE(inst);
	RSET_OPCODE(rtn, get_rbxl_opcode(opcode));

	switch (getOpMode(opcode)) {
	case iABC:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_B(rtn, GETARG_B(inst));
		RSETARG_C(rtn, GETARG_C(inst));
		break;
	case iABx:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_Bx(rtn, GETARG_Bx(inst));
		break;
	case iAsBx:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_sBx(rtn, GETARG_sBx(inst));
		break;
	default:
		return 0;
	}

	if (opcode == OP_MOVE)
		RSETARG_C(rtn, 1);

		return rtn;
}

r_Proto* convert_lua_proto(int r_lua_State, Proto* p, const char* script_loc) {
	r_Proto* np = r_luaF_newproto(r_lua_State);

	np->sizecode = p->sizecode;
	int* pcode = (int*)r_luaM_malloc(r_lua_State, np->sizecode * sizeof(int));
	for (unsigned int i = 0; i < np->sizecode; i++)
		pcode[i] = convert_lua_instruction(p->code[i]);
	encrypt_ptr((int)&np->code, pcode);

	np->sizek = p->sizek;
	r_TValue* nk = (r_TValue*)r_luaM_malloc(r_lua_State, np->sizek * sizeof(r_TValue));
	for (unsigned int i = 0; i < np->sizek; i++) {
		r_TValue* r_k = &nk[i];
		TValue* l_k = &p->k[i];

		switch (l_k->tt) {
		case LUA_TNIL:
			r_k->value_0 = 0;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TNIL;
			r_k->unk = 0;
			break;
		case LUA_TBOOLEAN:
			r_k->value_0 = l_k->value.b;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TBOOLEAN;
			r_k->unk = 0;
			break;
		case LUA_TNUMBER:
		{
			__int64 val;
			memcpy(&val, &l_k->value.n, sizeof(__int64));

			rtvalue_setn(r_k, val);
			r_k->tt = RLUA_TNUMBER;
			r_k->unk = 0;
			break;
		}
		case LUA_TSTRING:
		{
			TString* l_tsv = &l_k->value.gc->ts;
			r_TString* r_tsv = r_luaS_newlstr(r_lua_State, (const char*)(l_tsv + 1), l_tsv->tsv.len);

			r_k->value_0 = (int)r_tsv;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TSTRING;
			r_k->unk = 0;
			break;
		}
		default:
			printf("bad constant type\n");
			break;
		}
	}
	encrypt_ptr((int)&np->k, nk);

	np->maxstacksize = p->maxstacksize;
	np->numparams = p->numparams;
	np->nups = p->nups;
	np->is_vararg = p->is_vararg;

	r_TString* src = r_luaS_new(r_lua_State, script_loc ? script_loc : "@elysian");
	encrypt_ptr((int)&np->source, src);

	int szlineinfo = p->sizelineinfo;
	np->sizelineinfo = szlineinfo;
	int* li = (int*)r_luaM_malloc(r_lua_State, szlineinfo * sizeof(int));
	for (int i = 0; i < szlineinfo; i++) {
		li[i] = p->lineinfo[i] - 1 ^ (i << 8);
	}
	encrypt_ptr((int)&np->lineinfo, li);

	int szproto = p->sizep;
	np->sizep = szproto;

	r_Proto** nplist = (r_Proto**)r_luaM_malloc(r_lua_State, szproto * sizeof(r_Proto**));
	for (int i = 0; i < szproto; i++)
		nplist[i] = convert_lua_proto(r_lua_State, p->p[i], script_loc);
	encrypt_ptr((int)&np->p, nplist);

	return np;
}

int convert(int r_lua_State, const char* source) {
	TValue* value = L->top - 1;
	if (!isLfunction(value)) {
		lua_pop(L, 1);
		return 0;
	}

	LClosure* lc = &clvalue(value)->l;
	Proto* p = lc->p;

	int env = *(int*)(r_lua_State + 104);
	r_LClosure* nlc = r_luaF_newLclosure(r_lua_State, 0, env); // 2nd arg: lc->nupvalues
	r_Proto* np = convert_lua_proto(r_lua_State, p, source);
	encrypt_ptr((int)&nlc->p, np);

	lua_pop(L, 1); // get rid of function on L stack

	r_lua_pushLclosure(r_lua_State, nlc);
	return 1;
}

void r_snapshot(int from, int to, int start, int end) {
	for (int i = start; i <= end; i++) {
		r_TValue* c = r_getobjat(from, i);
		r_lua_getfield(from, -10002, "tostring");
		r_setobj(r_gettop(from), c);
		r_incr_top(from);
		r_luaD_call(from, r_gettop(from) - 2, 1);

		r_TValue* rtn = r_gettop(from) - 1;
		if (rtn->tt == RLUA_TSTRING) {
			r_lua_pushstring(to, (const char*)((r_TString*)(rtn->value_0) + 1));
		}
		else {
			r_lua_pushnil(to);
		}


		/*switch (c->tt) {
		case RLUA_TFUNCTION:
		{
		r_lua_pushstring(to, "[FUNCTION]");
		break;
		}
		case RLUA_TUSERDATA:
		{
		//r_Udata* udata = (r_Udata*)c->value_0;
		//void* ndata = (int*)r_lua_newuserdata(to, udata->len);
		//memcpy(ndata, udata + 1, udata->len);
		r_lua_pushstring(to, "[USERDATA]");
		break;
		}
		case RLUA_TSTRING:
		{
		r_TString* tsv = (r_TString*)c->value_0;
		r_lua_pushstring(to, (const char*)(tsv + 1));
		break;
		}
		default:
		{
		r_setobj(r_gettop(to), c);
		r_incr_top(to);
		break;
		}
		}*/


	}
}

// --- lua function implementations --- \\

#define LIMPL_ERR(s) r_lua_pushnil(r_lua_State); r_lua_pushstring(r_lua_State, s);
#define	LIMPL_ERR_RTN(s) LIMPL_ERR(s) return 2;

int loadstring_impl(int r_lua_State) {
	r_TValue* codeval = r_getobjat(r_lua_State, 1);

	if (codeval->tt == RLUA_TSTRING) {
		r_TString* tsv = *(r_TString**)(codeval);
		const char* code = (const char*)(tsv + 1);

		if (luaL_loadstring(L, code)) {
			r_lua_pushnil(r_lua_State);
			r_lua_pushstring(r_lua_State, lua_tostring(L, -1));
			lua_pop(L, 1);
			return 2;
		}

		convert(r_lua_State, 0);
		return 1;
	}

	r_lua_pushnil(r_lua_State);
	r_lua_pushstring(r_lua_State, "bad argument");

	return 2;
}

int require_impl(int r_lua_State) {
	r_lua_pushstring(r_lua_State, "require is not supported");
	return 1;
}

int getrawmetatable_impl(int r_lua_State) {
	if (!r_lua_gettop(r_lua_State) || !r_lua_getmetatable(r_lua_State)) {
		r_lua_pushnil(r_lua_State);
		return 1;
	}

	return 1;
}

int setrawmetatable_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) < 2) {
		r_lua_pushboolean(r_lua_State, 0);
		r_lua_pushstring(r_lua_State, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* obj1 = top - 1;
	r_TValue* obj0 = top - 2;

	if (!(obj0->tt == RLUA_TTABLE || obj0->tt == RLUA_TUSERDATA) || !(obj1->tt == RLUA_TNIL || obj1->tt == RLUA_TTABLE)) {
		r_lua_pushboolean(r_lua_State, 0);
		r_lua_pushstring(r_lua_State, "bad argument types");
		return 2;
	}

	r_lua_setmetatable(r_lua_State, 1);
	r_lua_pushboolean(r_lua_State, 1);
	return 1;
}

int setreadonly_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) < 1) {
		r_lua_pushboolean(r_lua_State, 0);
		r_lua_pushstring(r_lua_State, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* b = top - 1;
	r_TValue* obj = top - 2;

	if (obj->tt != RLUA_TTABLE) {
		r_lua_pushboolean(r_lua_State, 0);
		r_lua_pushstring(r_lua_State, "table expected");
		return 2;
	}

	if (b->tt != RLUA_TBOOLEAN) {
		r_lua_pushboolean(r_lua_State, 0);
		r_lua_pushstring(r_lua_State, "boolean expected");
		return 2;
	}

	*(unsigned char*)(obj->value_0 + 6) = b->value_0;
	r_lua_pushboolean(r_lua_State, 1);
	return 1;
}

int isreadonly_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) < 1) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "1 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* obj = top - 1;

	if (obj->tt != RLUA_TTABLE) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "table expected");
		return 2;
	}


	r_lua_pushboolean(r_lua_State, (int)*(unsigned char*)(obj->value_0 + 6));
	return 1;
}

int printconsole_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) < 1) {
		LIMPL_ERR_RTN("1 or more arguments expected")
	}

	r_TValue* obj = r_getobjat(r_lua_State, 1);

	if (obj->tt != RLUA_TSTRING) {
		LIMPL_ERR_RTN("string expected")
	}

	COLORREF col;
	if (r_lua_gettop(r_lua_State) >= 4) {
		int r = (int)r_tonumber(r_lua_State, 2);
		int g = (int)r_tonumber(r_lua_State, 3);
		int b = (int)r_tonumber(r_lua_State, 4);

		col = RGB(r, g, b);
	}
	else {
		col = RGB(0, 0, 200);
	}

	r_TString* tsv = (r_TString*)(obj->value_0);

	form->print(col, "%s\n", (const char*)(tsv + 1));

	r_lua_pushboolean(r_lua_State, 1);
	return 1;
}

int getobjects_impl(int r_lua_State) {
	LIMPL_ERR_RTN("not available")
		/*
	if (r_lua_gettop(r_lua_State) < 1) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "1 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* obj = top - 1;

	if (obj->tt != RLUA_TSTRING) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "string expected");
		return 2;
	}

	r_TString* tsv = (r_TString*)(obj->value_0);
	const char* str = (const char*)(tsv + 1);
	if (!tsv->len) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "empty string");
		return 2;
	}

	int buff = 0;
	int unk0[6];
	int unk1[6];
	int unk2[6];
	memset(unk0, 0, sizeof(unk0));
	memset(unk1, 0, sizeof(unk1));
	memset(unk2, 0, sizeof(unk2));
	std::string contentstr = str;
	int rtn = getContent(rLua->GetChildByClass(rLua->GetGameInstance(), "ContentProvider"), &buff, &contentstr, (int)unk0, (int)unk1);
	std::istream* content = *(std::istream**)(rtn);

	std::vector<RobloxInstance>* instances = new std::vector < RobloxInstance >;
	unk2[0] = (int)contentstr.c_str();
	loadInstances((int)unk2, content, instances);

	for (std::vector<RobloxInstance>::iterator it = instances->begin(); it != instances->end(); it++) {
		wrap_object(r_lua_State, it->instance, it->unk_0);
	}

	int sz = instances->size();
	delete instances;
	return sz;*/
}

int readfile_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 1) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "1 argument expected");
		return 2;
	}

	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* obj = r_getobjat(r_lua_State, 1);

	if (obj->tt != RLUA_TSTRING) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "string expected");
		return 2;
	}

	r_TString* strobj = (r_TString*)obj->value_0;
	const char* str = (const char*)(strobj + 1);

	if (!PathFileExists(str)) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "path doesn't exist");
		return 2;
	}

	std::string filecontents;
	if (!util::ReadFile(str, filecontents, 0)) {
		r_lua_pushnil(r_lua_State);
		r_lua_pushstring(r_lua_State, "couldn't open file");
		return 2;
	}

	r_lua_pushstring(r_lua_State, filecontents.c_str());
	return 1;
}

int writefile_impl(int r_lua_State) {
	int nargs = r_lua_gettop(r_lua_State);
	if (nargs < 2) {
		LIMPL_ERR_RTN("2 or more arguments expected")
	}

	r_TValue* obj = r_getobjat(r_lua_State, 1);
	r_TValue* data = r_getobjat(r_lua_State, 2);
	unsigned char binary = false;

	if (!(obj->tt == RLUA_TSTRING && data->tt == RLUA_TSTRING)) {
		LIMPL_ERR_RTN("string(s) expected");
	}

	if (nargs > 2) {
		r_TValue* binaryobj = r_getobjat(r_lua_State, 3);
		if (binaryobj->tt != RLUA_TBOOLEAN) {
			LIMPL_ERR_RTN("boolean expected");
		}

		binary = binaryobj->value_0;
	}

	r_TString* tsvobj = (r_TString*)obj->value_0;
	const char* strobj = (const char*)(tsvobj + 1);

	r_TString* tsvdata = (r_TString*)data->value_0;
	const char* strdata = (const char*)(tsvdata + 1);

	if (!util::WriteFile(strobj, strdata, binary)) {
		LIMPL_ERR_RTN("failed to write file");
	}

	r_lua_pushboolean(r_lua_State, true);
	return 1;
}

int getelysianpath_impl(int r_lua_State) {
	char path[MAX_PATH];
	util::GetFile("elysian.dll", "", path, MAX_PATH);

	r_lua_pushstring(r_lua_State, path);
	return 1;
}

int getgenv_impl(int r_lua_State) {
	r_TValue* top = r_gettop(r_lua_State);

	r_setobj(top, (r_TValue*)(r_L + 104));
	r_incr_top(r_lua_State);

	return 1;
}

int clipboard_set_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 1) {
		LIMPL_ERR_RTN("1 argument expected");
	}

	r_TValue* val = r_getobjat(r_lua_State, 1);
	if (val->tt != RLUA_TSTRING) {
		LIMPL_ERR_RTN("string expected");
	}

	if (!OpenClipboard(NULL)) {
		LIMPL_ERR_RTN("failed to open the clipboard");
	}

	if (!EmptyClipboard()) {
		CloseClipboard();
		LIMPL_ERR("failed to empty clipboard");
		return 2;
	}

	r_TString* tsv = (r_TString*)(val->value_0);
	const char* str = (const char*)(tsv + 1);

	HGLOBAL galloc = GlobalAlloc(GMEM_FIXED, tsv->len + 1);
	strcpy((char*)galloc, str);

	if (SetClipboardData(CF_TEXT, galloc) == NULL) {
		CloseClipboard();
		GlobalFree(galloc);
		LIMPL_ERR("failed to set clipboard");
		return 2;
	}

	CloseClipboard();
	r_lua_pushboolean(r_lua_State, true);
	return 1;
}

int mouse1press_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1release_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1click_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2press_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2release_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2click_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int keypress_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 1) {
		LIMPL_ERR_RTN("1 argument expected");
	}

	r_TValue* keycode = r_getobjat(r_lua_State, 1);
	if (keycode->tt != RLUA_TNUMBER) {
		LIMPL_ERR_RTN("number expected");
	}

	double n = rtvalue_getn(keycode);

	printf("n: %f\n", n);

	INPUT input;
	input.type = INPUT_KEYBOARD;
	memset(&input.ki, 0, sizeof(KEYBDINPUT));
	input.ki.wScan = MapVirtualKey(n, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	SendInput(1, &input, sizeof(input));
}

int keyrelease_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 1) {
		LIMPL_ERR_RTN("1 argument expected");
	}

	r_TValue* keycode = r_getobjat(r_lua_State, 1);
	if (keycode->tt != RLUA_TNUMBER) {
		LIMPL_ERR_RTN("number expected");
	}

	double n = rtvalue_getn(keycode);

	printf("n: %f\n", n);

	INPUT input;
	input.type = INPUT_KEYBOARD;
	memset(&input.ki, 0, sizeof(KEYBDINPUT));
	input.ki.wScan = MapVirtualKey(n, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

int mousemoverel_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 2) {
		LIMPL_ERR_RTN("2 arguments expected");
	}

	r_TValue* xtv = r_getobjat(r_lua_State, 1);
	r_TValue* ytv = r_getobjat(r_lua_State, 2);

	if (!(xtv->tt == RLUA_TNUMBER && ytv->tt == RLUA_TNUMBER)) {
		LIMPL_ERR_RTN("number(s) expected");
	}

	double x = rtvalue_getn(xtv);
	double y = rtvalue_getn(ytv);

	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));
}

int mousescroll_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 1) {
		LIMPL_ERR_RTN("1 argument expected");
	}

	r_TValue* scrolltv = r_getobjat(r_lua_State, 1);

	if (scrolltv->tt != RLUA_TNUMBER) {
		LIMPL_ERR_RTN("number(s) expected");
	}

	double scroll = rtvalue_getn(scrolltv);

	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.mouseData = scroll;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	SendInput(1, &input, sizeof(INPUT));
}

int getfgwindowtitle_impl(int r_lua_State) {
	char title[256];
	GetWindowText(GetForegroundWindow(), title, 256);

	r_lua_pushstring(r_lua_State, title);
	return 1;
}

int saveinstance_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 2) {
		LIMPL_ERR_RTN("2 arguments expected");
	}

	const char* path = r_tostring(r_lua_State, 1);
	if (!path) {
		LIMPL_ERR_RTN("string expected");
	}

	RBXInstance* instance = (RBXInstance*)r_getudata(r_lua_State, 2);
	if (!instance) {
		LIMPL_ERR_RTN("instance expected");
	}

	printf("saving %X to %s\n", instance->instance, path);

	std::fstream out(path, std::ios::out | std::ios::binary);
	if (!out.is_open()) {
		LIMPL_ERR_RTN("failed to open file");
	}

	std::vector<RBXInstance> instances;
	instances.push_back(*instance);

	SerializeInstance(out, instances, 0);

	out.close();

	r_lua_pushboolean(r_lua_State, true);
	return 1;
}

// detour stuff

typedef struct detourobj {
	rlua_CFunction f;
	r_CClosure* closure;
	const std::string fname;
	r_LClosure* callback;
};

std::vector<detourobj> detourlist;

int method_hook(int r_lua_State) {
	r_TValue* funcobj = r_getobjat(r_lua_State, 0); // func
	r_CClosure* closure = (r_CClosure*)(funcobj->value_0);

	detourobj* dobj = 0;
	for (std::vector<detourobj>::iterator it = detourlist.begin(); it != detourlist.end(); it++) {
		if (it->closure == closure)
			dobj = &*it;
	}

	if (!dobj) {
		printf("DETERR: no detour obj found!\n");
		return 0; // no func to call
	}

	//printf("closure: %x, name: %s, f: %x\n", closure, dobj->fname.c_str(), dobj->f);

	int nargs = r_lua_gettop(r_lua_State);
	if (nargs < 1) {
		printf("DETERR: not enough args!\n");
		return dobj->f(r_lua_State);
	}

	r_lua_pushLclosure(r_L, dobj->callback);				//	func

	r_TValue* instobj = r_getobjat(r_lua_State, 1);
	if (instobj->tt != RLUA_TUSERDATA) {
		r_lua_pushnil(r_L);
	}
	else {
		int* inst = (int*)r_getudata(r_lua_State, 1);
		if (!inst)
			r_lua_pushnil(r_L);
		else
			wrap_object(r_L, inst[0], inst[1]);				// func inst
	}

	r_snapshot(r_lua_State, r_L, 2, nargs);					// func inst ...

	//r_luaD_call(r_L, r_getobjat(r_L, 1), 0);



	//r_lua_settop(r_L, 0);
	return dobj->f(r_lua_State);
}

int detourmethod_impl(int r_lua_State) {
	if (r_lua_gettop(r_lua_State) != 3) {
		LIMPL_ERR_RTN("3 arguments expected");
	}

	r_TValue* objclass = r_getobjat(r_lua_State, 1);
	r_TValue* objmethod = r_getobjat(r_lua_State, 2);
	r_TValue* callback = r_getobjat(r_lua_State, 3);

	if (!(objclass->tt == RLUA_TSTRING && objmethod->tt == RLUA_TSTRING)) {
		LIMPL_ERR_RTN("string expected");
	}

	if (callback->tt != RLUA_TFUNCTION) {
		LIMPL_ERR_RTN("function expected");
	}

	const char* objclass_str = (const char*)((r_TString*)(objclass->value_0) + 1);
	const char* objmethod_str = (const char*)((r_TString*)(objmethod->value_0) + 1);

	int mainstate = getstateindex(script_context, 0);

	r_lua_getfield(mainstate, -10002, "Instance");		// Instance
	r_lua_getfield(mainstate, -1, "new");				// Instance new 
	r_lua_pushstring(mainstate, objclass_str);			// Instance new class
	r_luaD_call(mainstate, r_gettop(mainstate) - 2, 1);	// Instance object

	r_TValue* obj = r_gettop(mainstate) - 1;
	if (obj->tt != RLUA_TUSERDATA) {
		r_lua_pop(mainstate, 2);
		LIMPL_ERR_RTN("invalid class");
	}

	try {
		r_lua_getfield(mainstate, -1, objmethod_str);	// Instance object function
	}
	catch (...) {
		r_lua_pop(mainstate, 2);
		LIMPL_ERR_RTN("given method does not exist");
	}
	r_TValue* method = r_gettop(mainstate) - 1;
	if (method->tt != RLUA_TFUNCTION) {
		r_lua_pop(mainstate, 3);
		LIMPL_ERR_RTN("given method is not a function")
	}

	r_CClosure* closure = (r_CClosure*)(method->value_0);
	rlua_CFunction f = (rlua_CFunction)decrypt_ptr((int)&closure->f);

	encrypt_ptr((int)&closure->f, method_hook);

	detourlist.push_back({ f, closure, (std::string)objclass_str + "." + (std::string)objmethod_str, (r_LClosure*)callback->value_0 });

	r_lua_pop(mainstate, 3);
	r_lua_pushboolean(r_lua_State, true);

	return 1;
}

// --- hooks --- \\

GetGlobalState_Def GetGlobalStateT;
int GetGlobalState_Hook(int r_lua_State) {
	/* check if thread belongs to elysian (check global states) */
	if (decrypt_ptr(r_lua_State + 8) == decrypt_ptr(r_L + 8))
		return r_L;

	return GetGlobalStateT(r_lua_State);
}

// located at ud_hook_location
// also known as: luaL_loadprotected
typedef int(__cdecl *LoadScript_Def)(int r_lua_State, std::string* code, const char* source, int unk);
LoadScript_Def LoadScriptT;

int LoadScript_Hook(int r_lua_State, std::string* code, const char* source, int unk) {
	/*
	** code[0] = source code (empty when loading a roblox script)
	** code[1] = byte code
	*/

	if (!code[0].empty()) {
		/* not bytecode, let's add it to the thread scheduler */
		std::string buffer = "spawn(function()\n";
		buffer += code[0];
		buffer += "\nend)";

		int* inst_storage = (int*)(r_lua_State - 44 + 32);
		tsched.Queue(r_L, buffer, source, inst_storage[0], inst_storage[1]);

		/* return with an error */
		char error[7];
		random_string(error, 7);
		r_lua_pushstring(r_lua_State, (const char*)error);

		return 1; /* non-zero means error */
	}

	return LoadScriptT(r_lua_State, code, source, unk);
}

// retcheck hook
int retcheck_hook_rtn;
int ret_check_flag_1_value;
void __declspec(naked) retcheck_hook() {
	__asm {
		mov eax, ret_check_flag_0
		mov ecx, [eax + 10h]
		mov [eax], ecx
		mov eax, ret_check_flag_1
		mov ecx, ret_check_flag_1_value
		mov [eax], ecx
		add esp, 4
		ret
	}
}

void LuaVMHook(int r_lua_State, lua_Debug* ar) {
	int* inst_storage = (int*)(r_lua_State - 44 + 32);
	//printf("%X, %X\n", r_lua_State, inst_storage[0]);
	//printf("%s\n", rLua->GetFullName(inst_storage[0]).c_str());

	r_lua_getfield(r_lua_State, -10002, "Standing");

	static int debounce = 0;
	r_TValue* top = r_gettop(r_lua_State) - 1;

	if (!top->value_0 && debounce == 0) {
		printf("JUMPED\n");
		debounce = 1;
	}
	else if (top->value_0 && debounce == 1) {
		printf("LANDED\n");
		debounce = 0;
	}
}

// vm hook
int vm_hook_jumploc;
int vm_hook_moveloc;
void __declspec(naked) vm_hook() {
	__asm {
		mov vm_hook_jumploc, eax
		push ecx					// save registers
		push edx
		
		//push [ebp-30h]
		//push esi
		//call VMHookTest
		//add esp, 8

		push esi
		call GetGlobalState
		add esp, 4
		pop edx
		pop ecx
		cmp eax, r_L				// check if r_L
		jne vm_hook_exit
		mov eax, edx
		shr eax, 26					// restore eax & get opcode from instruction
	vm_hook_OPCALL:
	cmp eax, 0x1F
		jne vm_hook_OPTAILCALL
		/* OP_CALL */
		mov eax, edx
		add dword ptr vm_hook_jumploc, 40
		jmp vm_hook_jumploc
	vm_hook_OPTAILCALL :
	cmp eax, 0x1B
		jne vm_hook_OPCLOSURE
		/* OP_TAILCALL */
		mov eax, edx
		add dword ptr vm_hook_jumploc, 40
		jmp vm_hook_jumploc
	vm_hook_OPCLOSURE :
	cmp eax, 0x25
		jne vm_hook_OPRETURN
		/* OP_CLOSURE */
		mov esi, edx
		mov edx, [ebp - 30h]
		add dword ptr vm_hook_jumploc, 45
		push dword ptr[edx + 0Ch]
		jmp vm_hook_jumploc
	vm_hook_OPRETURN :
		cmp eax, 0x22
		jne vm_hook_exit
		/* OP_RETURN */
		mov[ebp - 24h], eax	// not needed?
		mov eax, edx
		add dword ptr vm_hook_jumploc, 75
		jmp vm_hook_jumploc
	vm_hook_exit:
		mov eax, vm_hook_moveloc
		cmp vm_hook_jumploc, eax	// check if jump location is OP_MOVE
		jne vm_hook_final
		mov eax, edx
		shr eax, 26
		and edx, 0x000001FF
		add dword ptr vm_hook_jumploc, 5
		jmp vm_hook_jumploc
	vm_hook_final:
		mov eax, edx
		shr eax, 26					// restore eax
		jmp vm_hook_jumploc			// jump
	}
}

// --- core api --- \\

int luaA_execute(std::string buffer) {
	char str[7];
	random_string(str, 7);

	std::string buff;
	buff = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = 'jH:245G(2l]' script.Parent = nil\r\n";
	buff += buffer;
	buff += "\r\nend)";

	tsched.Queue(r_L, buff, (const char*)str, 0, 0);

	return 1;
}

int luaA_executeraw(std::string buffer, std::string source) {
	tsched.Queue(r_L, buffer, source, 0, 0);
	return 1;
}

void luaA_runautoexec() {
	/* fetch autoexec folder path */
	char cPath[MAX_PATH];
	util::GetFile(ELYSIAN_DLL, "scripts\\autoexec\\", cPath, MAX_PATH);

	std::string aePath = cPath;
	std::string iPath = aePath + "init.lua";

	if (!PathFileExists(iPath.c_str()))
		throw std::exception("file path 'scripts\\autoexec\\init.lua' doesnt exist");

	/* run init.lua */
	std::string initbuffer;
	if (!util::ReadFile(iPath, initbuffer, 0))
		throw std::exception("failed to execute init.lua");

	luaA_execute(initbuffer.c_str());

	/* run rest of autoexec files */
	std::vector<std::string> files;
	util::GetFilesInDirectory(files, aePath, 0);

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
		if (*it == "init.lua") continue;

		std::string fpath = aePath + *it;
		std::string buffer;
		if (util::ReadFile(fpath, buffer, 0)) {
			luaA_execute(buffer.c_str());
			form->print(RGB(0, 0, 0), "File '%s' executed\r\n", (*it).c_str());
		}
		else {
			form->print(RGB(255, 0, 0), "Unable to read file '%s'", (*it).c_str());
		}
	}
}

int luaA_setinit(int b) {
	if (r_L) {
		r_lua_pushboolean(r_L, b);
		r_lua_setfield(r_L, -10002, "ELYSIAN_INITIATED");
		return true;
	}

	return false;
}

void luaA_executeTK() {
	return;
}

int luaA_init(int scontext) {
	script_context = scontext;

	/* create lua state */
	L = luaL_newstate();
	if (!L)
		throw std::exception("failed to create lua state");

	luaL_openlibs(L);

	form->print(RGB(0, 0, 0), "Scanning... ");

	/* scan for all signatures */
	VMProtectBeginMutation("scan");
	SignatureScanner sigscan;
	sigscan.Queue("openstate", (int*)&openState, "\x0F\x95\xC1\x0F\xB6\xC9\x51\x8B\xC8\xE8", "xxxxxxxxxx", -174, 0);											// 0F 95 C1 0F B6 C9 51 8B C8 E8
	sigscan.Queue("newlstr", (int*)&r_luaS_newlstr, "\x55\x8B\xEC\x51\x8B\x45\x10\x53\x56\x8B\xF0\xC1\xEE\x05\x46", "xxxxxxxxxxxxxxx", 0, 0);					// 55 8B EC 51 8B 45 10 53 56 8B F0 C1 EE 05 46
	sigscan.Queue("realloc", (int*)&r_luaM_realloc_, "\x55\x8B\xEC\x53\x8B\x5D\x08\x56\x8B\x75\x14\x57\x8B\x7B\x08", "xxxxxxxxxxxxxxx", 0, 0);					// 55 8B EC 53 8B 5D 08 56 8B 75 14 57 8B 7B 08
	sigscan.Queue("setmetatable", (int*)&r_lua_setmetatable, "\x8B\x7E\x10\x83\xC4\x08\x83\x7F\xF8\x00\x8B\xD8", "xxxxxxxxxxxx", -18, 0);						// 8B 7E 10 83 C4 08 83 7F F8 00 8B D8
	sigscan.Queue("getfield", (int*)&r_lua_getfield, "\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x8B\xCA\x83\xC4\x08\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\xFF\x76\x10", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0, 0); // 55 8B EC 83 EC 10 53 56 8B 75 08 57 FF 75 0C 56 E8 ?? ?? ?? ?? 8B 55 10 8B CA 83 C4 08 8B F8 8D 59 01 8A 01 41 84 C0 75 F9 2B CB 51 52 56 E8 ?? ?? ?? ?? FF 76 10
	sigscan.Queue("newproto", (int*)&r_luaF_newproto, "\x55\x8B\xEC\x57\x6A\x4C\x6A\x00\x6A\x00\xFF\x75\x08", "xxxxxxxxxxxxx", 0, 0);							// 55 8B EC 57 6A 4C 6A 00 6A 00 FF 75 08
	sigscan.Queue("newthread", (int*)&r_lua_newthread, "\x51\x56\x8B\x75\x08\x57\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44\x31\x54", "xxxxxxxxxxxxxxxxx", -24, 0);	// 51 56 8B 75 08 57 8B 4E 08 8B 44 31 60 3B 44 31 54
	sigscan.Queue("resume", (int*)&r_lua_resume, "\x55\x8B\xEC\x56\x8B\x75\x08\x8A\x46\x06\x3C\x01\x74", "xxxxxxxxxxxxx", 0, 0);								// 55 8B EC 56 8B 75 08 8A 46 06 3C 01 74
	sigscan.Queue("pushCc", (int*)&r_lua_pushcclosure, "\x8B\x47\x10\x8B\x4D\x08\x89\x08\xC7\x40\x08\x06\x00\x00\x00", "xxxxxxxxxxxxxxx", -174, 0);				// 8B 47 10 8B 4D 08 89 08 C7 40 08 06 00 00 00
	sigscan.Queue("getContent", (int*)&getContent, "\x83\xEC\x30\x53\x33\xDB\x56\x89\x5D\xF0\x57\x8B\xF9", "xxxxxxxxxxxxx", -24, 0);							// 83 EC 30 53 33 DB 56 89 5D F0 57 8B F9
	sigscan.Queue("loadInstances", (int*)&loadInstances, "\x8D\x45\xE8\x53\x56\x8B\x75\x08\x6A\x00\x6A\x08\x8B\xD9\x50\x8B\xCE", "xxxxxxxxxxxxxxxxx", -27, 0);	// 8D 45 E8 53 56 8B 75 08 6A 00 6A 08 8B D9 50 8B CE
	sigscan.Queue("newudata", (int*)&r_lua_newuserdata, "\x8B\x4E\x10\x8B\xF8\x89\x39\xC7\x41\x08\x08\x00\x00\x00", "xxxxxxxxxxxxxx", -61, 0);					// 8B 4E 10 8B F8 89 39 C7 41 08 08 00 00 00
	sigscan.Queue("ggs", (int*)&GetGlobalState, "\x8B\x71\x08\x83\xC1\x08\x8D\x53\x08", "xxxxxxxxx", -45, 0);														// 8B 71 08 83 C1 08 8D 53 08
	VMProtectEnd();

	sigscan.Run();
	map_queue results = sigscan.GetResults();
	for (map_queue::iterator it = results.begin(); it != results.end(); it++) {
		SSEntry* entry = &it->second;
		if (entry->state == SSRESULT_FAILURE) {
			form->print(RGB(255, 0, 0), "Failed to find signature '%s'\n", it->first);
		}
	}

	if (sigscan.ErrorCount() > 0)
		throw std::exception("unable to find all signatures");

	form->print(RGB(0, 150, 0), "OK\r\n");

	/* adjust offsets */
	reg_offset(ud_hook_location);
	reg_offset(lua_number_key);
	reg_offset(render_step_location);
	reg_offset(ret_check);
	reg_offset(ret_check_flag_0);
	reg_offset(ret_check_flag_1);
	reg_offset(vm_shutdown_bp);
	reg_offset(vm_hook_location);
	reg_offset(trust_check_bp);

	reg_offset(SerializeInstance);
	reg_offset(r_luaD_call);
	reg_offset(r_lua_sethook);

	vm_hook_moveloc = vm_hook_location + 7;

	/* start thread scheduler */
	tsched.Start();

	/* create roblox lua state */
	try {
		int old_data = getstateindex(scontext, 2);
		openState(scontext, 2);
		r_L = getstateindex(scontext, 2);
		getstateindex(scontext, 2) = old_data;

		encrypt_ptr(decrypt_ptr(r_L + 8) + 28, (void*)1); // + 28?
	}
	catch (std::exception e) {
		throw std::exception("failed to create roblox lua state");
	}
	catch (...) {
		throw std::exception("crash occured while creating roblox lua state");
	}

	GetGlobalStateT = (GetGlobalState_Def)memutil->CreateDetour(GetGlobalState, GetGlobalState_Hook, 3);
	LoadScriptT = (LoadScript_Def)memutil->CreateDetour((void*)ud_hook_location, LoadScript_Hook, 0);
	memutil->CodeInjection(ret_check, 0, retcheck_hook, 0, false, CALLHOOK);

	memutil->WriteMemory(trust_check_bp, "\xFE\xC0", 2, 0, true); // inc al
	memutil->WriteMemory(vm_shutdown_bp + 2, "\x00\x00\x00\x00", 4, 0, true);
	memutil->WriteMemory(vm_hook_location, "\x8B\x04", 2, 0, true); // jmp dword ptr [eax*4+table] -> mov eax, dword ptr [eax*4+table]
	memutil->CodeInjection(vm_hook_moveloc, 1, vm_hook, 0, false, JUMPHOOK);

	VMProtectBeginMutation("impl");

	addfunction("loadstring", loadstring_impl);
	addfunction("require", require_impl);
	addfunction("getrawmetatable", getrawmetatable_impl);
	addfunction("setrawmetatable", setrawmetatable_impl);
	addfunction("isreadonly", isreadonly_impl);
	addfunction("setreadonly", setreadonly_impl);
	addfunction("printconsole", printconsole_impl);
	addfunction("getobjects", getobjects_impl);
	addfunction("readfile", readfile_impl);
	addfunction("getgenv", getgenv_impl);
	addfunction("clipboard_set", clipboard_set_impl);
	addfunction("detourmethod", detourmethod_impl);
	addfunction("mouse1press", mouse1press_impl);
	addfunction("mouse1release", mouse1release_impl);
	addfunction("mouse2press", mouse2press_impl);
	addfunction("mouse2release", mouse2release_impl);
	addfunction("keypress", keypress_impl);
	addfunction("keyrelease", keyrelease_impl);
	addfunction("mousemoverel", mousemoverel_impl);
	addfunction("mousescroll", mousescroll_impl);
	addfunction("getfgwindowtitle", getfgwindowtitle_impl);
	addfunction("writefile", writefile_impl);
	addfunction("getelysianpath", getelysianpath_impl);
	addfunction("saveinstance", saveinstance_impl);

	VMProtectEnd();

	r_lua_pushstring(r_L, ELYSIAN_VERSION);
	r_lua_setfield(r_L, -10002, "ELYSIAN_VERSION");
	luaA_runautoexec();

	return r_L;
}

// --- scheduler --- \\

ThreadScheduler::ThreadScheduler() {
	queue = new std::queue < ThreadRef >;
	//scheduler_thread = new util::athread(SchedulerRoutine, this);

	initiated = true;
}

ThreadScheduler::~ThreadScheduler() {
	std::queue<ThreadRef> empty;
	std::swap(*queue, empty);

	delete queue;
	//delete scheduler_thread;
	initiated = false;
}

int ThreadScheduler::GetQueueSize() {
	if (initiated)
		return queue->size();

	return 0;
}
std::queue<ThreadRef>* ThreadScheduler::GetQueue() {
	return queue;
}

void ThreadScheduler::Queue(int r_lua_State, std::string code, std::string source, int inst, int ext) {
	ThreadRef ref;
	ref.code = code;
	ref.source = source;
	ref.state = r_lua_State;
	ref.instance.instance = inst;
	ref.instance.unk_0 = ext;

	queue->push(ref);
}

/* render step hook */
typedef int(__thiscall *RenderStep_Def)(int self, float step);
RenderStep_Def RenderStep_T;

int __fastcall RenderStep_Hook(int self, int edx, float step) {
	tsched.Step();
	return RenderStep_T(self, step);
}

void ThreadScheduler::Start() {
	ret_check_flag_1_value = *(int*)ret_check_flag_1;

	RenderStep_T = (RenderStep_Def)memutil->CreateDetour((void*)render_step_location, RenderStep_Hook, 4);
}


void ThreadHook(int r_lua_State, int ar) {
	if (*(int*)ar == LUA_HOOKCOUNT && *(int*)(script_context + 372)) {
		throw std::exception("Game script timeout");
	}
}

void ThreadScheduler::Step() {
	auto queue = GetQueue();

	unsigned int queue_sz = GetQueueSize();
	if (queue_sz > 0) {
		ThreadRef* ref = &queue->front();

		if (luaL_loadstring(L, ref->code.c_str())) {
			form->print(RGB(255, 0, 0), "compile error: %s\r\n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		else {
			int thread = r_lua_newthread(ref->state);
			r_lua_sethook(thread, (void*)ThreadHook, 8, 1000);
			SetThreadIdentity(thread, 7, ref->instance.instance, ref->instance.unk_0);

			if (ref->instance.instance) {
				wrap_object(thread, ref->instance.instance, ref->instance.unk_0);
				r_lua_setfield(thread, -10002, "script");
			}

			convert(thread, ref->source.c_str()); // move and convert function on `L` to `thread`

			try {
				if (r_lua_resume(thread, 0) == LUA_ERRRUN) {
					r_TValue* err = r_gettop(thread) - 1;
					if (err->tt == RLUA_TSTRING)
						form->print(RGB(255, 0, 0), "runtime error: %s\n", (const char*)((r_TString*)(err->value_0) + 1));

					r_lua_pop(thread, 1);
				}
			}
			catch (...) {

			}

			r_lua_pop(ref->state, 1); // pop thread from ref->state
		}

		queue->pop();
	}

	ret_check_flag_1_value = *(int*)ret_check_flag_1;
}