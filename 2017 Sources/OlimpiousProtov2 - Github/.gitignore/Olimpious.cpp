#include "Convert.h"
#include "gui.h"
#include "Scan.h"

static int callEncryption_(Instruction inst, int pc)
{
	int encrypt_number = 1;

	for (int i = 0; i < 37; i++)
	{
		printf("loop.\n");
		DWORD a = inst + 4;
		DWORD v = a ^ *(DWORD *)a;
		unsigned int x = ((inst + 32757935 * v) ^ (a - 29799480 * pc - 5512095));
		unsigned int y = (inst >> 26 << 26) | ((1 + 5) ^ pc) & 0x3FFFFFF;
		unsigned int bit = x ^ y;

		printf("bit.\n");

		if (bit != x && y)
		{
			x |= y;
			y <<= 1;
			printf("statements.\n");
		}
	}
	return 0;
}

// Olimpious uses protoconversion, an edit of seven's (removed it's LuaTranslator, more stabled, etc.)
// Yes, it's made by Jayden (Astrica) :P
// UI Credit goes to (Louka @V3rmillion)

int getRLuaState(DWORD ScriptContext)
{
	try
	{
		return *(DWORD*)(ScriptContext + 164) - (ScriptContext + 164);
	}
	catch (std::exception ex)
	{
		std::cout << "Failed to create LuaState: " << ex.what() << "\n";
	}

}

// -- Instruction decryption -- //

int jmpEncryption(int i, int pc) {
	return i << pc ^ 142702329 - (pc + 12892396233);
}

#define LUAVM_DAX_ME 0x1C6B438 
#define LUAVM_DAX_AO 0x1A7D575
#define LUAVM_DAX_MO 0x1451AFB 

static uint32_t rbxDaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd)
{
	uint32_t result = 0;
	uint32_t mask = 1;
	for (size_t i = 0; i < 8 * sizeof(uint32_t); ++i)
	{
		uint32_t bitDesired = mask & x;
		uint32_t bitOdd = mask & (result*mulOdd + addOdd);
		uint32_t bitEven = mask & (result*mulEven + addEven);
		if ((bitEven ^ bitOdd) != bitDesired)
		{
			result |= mask;
		}
		mask <<= 1;
	}
	return result;
}



#if defined(LUAVM_SECURE)
#define LUAVM_ENCODEINSN(insn, key) ((insn) * key)
#define LUAVM_DECODEINSN(insn, key) ((insn).v * key)
#else
#define LUAVM_ENCODEINSN(insn, key) (insn)
#define LUAVM_DECODEINSN(insn, key) (insn).v
#endif

static long long multiplicativeInverse(long long a, long long n)
{
	long long t = 0;
	long long newt = 1;

	long long r = n;
	long long newr = a;

	while (newr != 0)
	{
		long long q = r / newr;

		long long curt = t;
		t = newt;
		newt = curt - q * newt;

		long long curr = r;
		r = newr;
		newr = curr - q * newr;
	}

	//RBXASSERT(r == 1);

	return (t < 0) ? t + n : t;
}


UINT32 Encrypt(UINT32 instruction, int increment)
{
	UINT32 linstruction = _rotl(instruction, 7);
	UINT32 rinstruction = _rotr(instruction, 3);

	UINT32 key = (32757935 * increment) ^ (rinstruction ^ linstruction);
	return (key ^ instruction);
}

#define set_arg(i,o,n,c)        ((i) = (((i) & MASK0(n, c) | \
                                (((Instruction)o << c) & MASK1(n, c)))))
const char rbx_opcode[NUM_OPCODES] = { 0x06, 0x04, 0x00, 0x07, 0x02, 0x08, 0x01, 0x03, 0x05, 0x0F, 0x0D, 0x09, 0x10, 0x0B, 0x11, 0x0A, 0x0C, 0x0E, 0x18, 0x16, 0x12, 0x19, 0x14, 0x1A, 0x13, 0x15, 0x17, 0x21, 0x1F, 0x1B, 0x22, 0x1D, 0x23, 0x1C, 0x1E, 0x20, 0x25, 0x24 };

int ConvertInstruction(Instruction i) {
	OpCode op = GET_OPCODE(i);
	int instruction = i & MASK0(6, 26) | ((Instruction)rbx_opcode[op] << 26 & MASK1(6, 26));

	switch (getOpMode(op)) {
	case iABC: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_B(i), 9, 0);
		set_arg(instruction, GETARG_C(i), 9, 9);
		break;
	}
	case iABx: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_Bx(i), 18, 0);
		break;
	}
	case iAsBx: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_sBx(i) + MAXARG_sBx, 18, 0);
		break;
	}
	}

	if (op == OP_MOVE)
		set_arg(instruction, 1, 9, 9);
	return instruction;
}

std::uint32_t decrypt(std::uint32_t instruction, int increment)
{
	instruction = instruction ^ (0x1F3D8AF * increment);

	for (int i = 0; i != 15; ++i) {
		instruction = Encrypt(instruction, 0);
	}
	printf("encrypted.\n");
	return instruction;
}

unsigned int InstructionBypass(unsigned int i, int pc, unsigned int key) {
	Instruction enc = i;
	Instruction op = GET_OPCODE(i);
	switch (op) {
	case OP_CALL:
		printf("call.\n");
	case OP_TAILCALL:
		printf("tailcall.\n");
	case OP_RETURN:
		printf("return.\n");
	case OP_CLOSURE:
		printf("closure.\n");
		enc = rbxDaxEncodeOp(i, LUAVM_DAX_ME, pc, LUAVM_DAX_MO, LUAVM_DAX_AO);
		SET_OPCODE(enc, op);
		break;
	case OP_MOVE:
		SETARG_C(enc, (pc | 1)); // non-zero
		break;
	default:
		break;
	}
	return LUAVM_ENCODEINSN(enc, key);
}

unsigned int callInstructionDecryption_(Instruction i, int curConvInst)
{
	Instruction curEncInst = curConvInst;
	OpCode curOP = GET_OPCODE(curConvInst);
	switch (curOP) {
	case 0x06: /* Move */
		printf("jmp.\n");
		SETARG_C(curEncInst, 1);
		break;
	case 0x14: /* Jmp */
		curEncInst = jmpEncryption(curConvInst, i);
		SET_OPCODE(curEncInst, curOP);
		printf("jmp.\n");
		break;
	case 0x1B: /* Tailcall (Not used) */
	case 0x1F: /* Call */
	case 0x22: /* Return */
	case 0x25: /* Closure*/
		printf("closure.\n");
		curEncInst = callEncryption_(curConvInst, i);
		SET_OPCODE(curEncInst, curOP);
		break;
	default:
		break;
	}
	return 0;
}

// -- End of instruction decryption -- //


void Console(const char* A)
{
	DWORD Null;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &Null);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(A);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	//SetWindowLong(GetConsoleWindow(), GWL_STYLE, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

DWORD ScriptContext;

BOOL Olimpious_luaA_execute(lua_State* L, const char* src)
{ 
	try {
		std::string fscript = std::string("spawn(function() local script = Instance.new('LocalScript') script.Name = 'Olimpious' script.Disabled = true end)" + std::string(src) + "\r\n");
		if (luaL_loadbuffer(L, src, strlen(src), "@Olimpious") == 0)
		{
			TValue* cl = (L->top - 1);
			LClosure* lcl = &(((Closure*)((L->top - 1)->value.p))->l);
			Proto* fptn = lcl->p;
			DWORD RL = rlua_newthread(luaState);
			typedef DWORD(__cdecl *rlua_initthread)(DWORD RL, int identity, DWORD script, DWORD z);
			((rlua_initthread)(ADDR_INITTHRD))(RL, 7, NULL, 0); // set identity to 7, this also sandboxs the thread (gives it's own globals)
			rlua_setfield(RL, -10002, "script");
			try {
				InstructionBypass(OP_CALL, 0x3FFFFFF, 29799480);
				convert(RL, lcl);
				printf("called.\n");
			}
			catch (std::exception e) {
				MessageBoxA(NULL, e.what(), "Olimpious Error", MB_OK);
			}
			catch (...) {
				MessageBoxA(NULL, "An Unhandled Error Has Occured!", "Olimpious Error", MB_OK);
			}
			lua_pcall(L, 0, 0, 0);
			MessageBoxA(0, "converted proto", "check", 0);
			return TRUE;
		}
		else {
			std::cout << lua_tostring(L, -1) << "\n";
		}
	}
	catch (std::exception e) {
		MessageBoxA(NULL, e.what(), "Oimpious Error", MB_OK);
	}
	catch (...) {
		MessageBoxA(NULL, "An Unhandled Error Has Occured!", "autism - Uh Oh", MB_OK);
	}
}

std::string Input()
{
	std::string input;
	getline(std::cin, input);
	return input;
}

__forceinline unsigned int callEncryption(Instruction inst, unsigned int pc) {
	unsigned int result = 0;
	unsigned int mask = 1;

	for (unsigned int i = 0; i < 32; i++) {
		unsigned int x = (mask & (result * pc - 0x541B9F));
		unsigned int y = (mask & (result * (pc + 0x1F3D8AF) - 0x1C0BCC3));
		unsigned int bit = x ^ y;

		if (bit != (mask & inst))
			result |= mask;
		mask <<= 1;
	}

	return result;
}

void Main()
{
	Console("Olimpious");
	printf("Scanning, Please wait...\n");
	DWORD ScriptContextPtr = rbxaddy(0x1010330);
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextPtr, "xxxx");
	luaState = getRLuaState(ScriptContext);
	printf("OK.\n");
	ShowForm();
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
	}
	return TRUE;
}
