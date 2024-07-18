#include "../stdafx.h"

typedef unsigned char lu_byte;
typedef void* GCObject;

#define CommonHeader    GCObject *next; lu_byte tt; lu_byte marked

#define ClosureHeader \
        CommonHeader; lu_byte isC; lu_byte nupvalues; GCObject *gclist; \
        void *env

#define LUAI_USER_ALIGNMENT_T   union { double u; void *s; long l; }
typedef LUAI_USER_ALIGNMENT_T L_Umaxalign;

typedef union {
	GCObject *gc;
	void *p;
	lua_Number n;
	int b;
} Value;

#define TValuefields    Value value; int tt

typedef struct lua_TValue {
	TValuefields;
} TValue;

typedef union TString {
	L_Umaxalign dummy;  /* ensures maximum alignment for strings */
	struct {
		CommonHeader;
		lu_byte reserved;
		unsigned int hash;
		size_t len;
	} tsv;
} TString;

typedef struct UpVal {
	CommonHeader;
	TValue *v;  /* points to stack or to its own value */
	union {
		TValue value;  /* the value (when closed) */
		struct {  /* double linked list (when open) */
			struct UpVal *prev;
			struct UpVal *next;
		} l;
	} u;
} UpVal;

typedef struct Proto {
	CommonHeader;
	TValue *k;  /* constants used by the function */
	DWORD *code;
	struct Proto **p;  /* functions defined inside the function */
	int *lineinfo;  /* map from opcodes to source lines */
	struct LocVar *locvars;  /* information about local variables */
	TString **upvalues;  /* upvalue names */
	TString  *source;
	int sizeupvalues;
	int sizek;  /* size of `k' */
	int sizecode;
	int sizelineinfo;
	int sizep;  /* size of `p' */
	int sizelocvars;
	int linedefined;
	int lastlinedefined;
	GCObject *gclist;
	lu_byte nups;  /* number of upvalues */
	lu_byte numparams;
	lu_byte is_vararg;
	lu_byte maxstacksize;
} Proto;

typedef struct CClosure {
	ClosureHeader;
	lua_CFunction f;
	TValue upvalue[1];
} CClosure;

typedef struct LClosure {
	ClosureHeader;
	struct Proto *p;
	UpVal *upvals[1];
} LClosure;

typedef union Closure {
	CClosure c;
	LClosure l;
} Closure;

using StkId = TValue*;
using Instruction = LUAI_UINT32;

typedef struct CallInfo {
	StkId base;  /* base for this function */
	StkId func;  /* function index in the stack */
	StkId top;  /* top for this function */
	const Instruction *savedpc;
	int nresults;  /* expected number of results from this function */
	int tailcalls;  /* number of tail calls lost under this entry */
} CallInfo;

struct lua_State {
	CommonHeader;
	lu_byte status;
	TValue *top;  /* first free slot in the stack */
	TValue *base;  /* base of current function */
	void *l_G;
	CallInfo *ci;  /* call info for current function */
	const Instruction *savedpc;  /* `savedpc' of current function */
	TValue *stack_last;  /* last free slot in the stack */
	TValue *stack;  /* stack base */
	void *end_ci;  /* points after end of ci array*/
	void *base_ci;  /* array of CallInfo's */
	int stacksize;
	int size_ci;  /* size of array `base_ci' */
	unsigned short nCcalls;  /* number of nested C calls */
	unsigned short baseCcalls;  /* nested C calls when resuming coroutine */
	lu_byte hookmask;
	lu_byte allowhook;
	int basehookcount;
	int hookcount;
	lua_Hook hook;
	TValue l_gt;  /* table of globals */
	TValue env;  /* temporary place for environments */
	GCObject *openupval;  /* list of open upvalues in this stack */
	GCObject *gclist;
	struct lua_longjmp *errorJmp;  /* current error recover point */
	ptrdiff_t errfunc;  /* current error handling function (stack index) */
};

typedef struct LocVar {
	TString *varname;
	int startpc;  /* first point where variable is active */
	int endpc;    /* first point where variable is dead */
} LocVar;

#define luaLS_newstr(L,str) luaLS_newlstr(L, str, strlen(str))
#define luaLM_malloc(RL,t)  luaLM_realloc_(RL, NULL, 0, (t))