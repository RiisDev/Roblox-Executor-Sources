// 
// static/threaded/threadings' x86 disassembler, copyright 2019
// 
#ifndef H_EYESTEP
#define H_EYESTEP

#include <Windows.h>
#include <string>
#include <vector>

#define Fl_none					0x00000000
#define Fl_src_only				0x10000000
#define Fl_src_dest				0x20000000

#define Fl_src_r8				0x00000001
#define Fl_src_r16				0x00000002
#define Fl_src_r32   			0x00000004
#define Fl_src_imm8				0x00000008
#define Fl_src_imm16			0x00000010
#define Fl_src_imm32   			0x00000020
#define Fl_src_rm8				0x00000040
#define Fl_src_rm16				0x00000080
#define Fl_src_rm32				0x00000100
#define Fl_src_rxmm				0x00000200
#define Fl_src_disp8			0x00000400
#define Fl_src_disp16			0x00000800
#define Fl_src_disp32			0x00001000

#define Fl_dest_r8				0x00002000
#define Fl_dest_r16				0x00004000
#define Fl_dest_r32   			0x00008000
#define Fl_dest_imm8			0x00010000
#define Fl_dest_imm16			0x00020000
#define Fl_dest_imm32   		0x00040000
#define Fl_dest_rm8				0x00080000
#define Fl_dest_rm16			0x00100000
#define Fl_dest_rm32			0x00200000
#define Fl_dest_rxmm			0x00400000
#define Fl_dest_disp8			0x00800000
#define Fl_dest_disp16			0x40000000
#define Fl_dest_disp32			0x80000000

#define Fl_rel8					0x01000000
#define Fl_rel16				0x02000000
#define Fl_rel32				0x04000000

#define Fl_condition			0x08000000

#define PRE_BYTE_PTR			0x1
#define PRE_WORD_PTR			0x2
#define PRE_DWORD_PTR			0x4
#define PRE_QWORD_PTR			0x8

#define pre_repne   			0x1
#define pre_repe   				0x2
#define pre_66   				0x4
#define pre_67   				0x8
#define pre_lock 				0x10
#define pre_seg  				0x20
#define seg_cs					0x2E
#define seg_ss					0x36
#define seg_ds					0x3E
#define seg_es					0x26
#define seg_fs					0x64
#define seg_gs					0x65


#define addoff8(s,b,l,v)		char m[16]; v=*(uint8_t*)(b+l); (v<=0x7F)		? sprintf_s(m,"+%02X",v) : sprintf_s(m,"-%02X",(0xFF+1)-v);			strcat_s(s,m); l+=1
#define addoff16(s,b,l,v)		char m[16]; v=*(uint16_t*)(b+l);(v<=0x7FFF)		? sprintf_s(m,"+%04X",v) : sprintf_s(m,"-%04X",(0xFFFF+1)-v);		strcat_s(s,m); l+=2
#define addoff32(s,b,l,v)		char m[16]; v=*(uint32_t*)(b+l);(v<=0x7FFFFFFF)	? sprintf_s(m,"+%08X",v) : sprintf_s(m,"-%08X",(0xFFFFFFFF+1)-v);	strcat_s(s,m); l+=4
#define getr1(b,l)				(b[l] % 64 / 8)
#define getr2(b,l)				(b[l] % 64 % 8)
#define getmode20(b,l)			(b[l] / 0x20)
#define getmode40(b,l)			(b[l] / 0x40)

#define asm_out_none			0x0
#define asm_out_strings			0x1
#define asm_out_offs_1b			0x2
#define asm_out_offs_2b			0x4
#define asm_out_offs_4b			0x10

enum direction {
	behind,
	ahead
};

namespace eyestep {
	struct inst {
		UINT address;

		BYTE len; // overall length of the instruction in bytes (defaults to 1)
		BYTE p_rep; // rep
		BYTE p_lock; // lock
		BYTE p_seg; // seg
		BYTE r_mod; // register mode

		UINT pref; // global instruction prefix (cs/ds/lock/repne/etc.)
		UINT flags; // global instruction flags (which also determine src/dest values)

		BYTE rel8; // 8 bit relative value (for jmp or call)
		USHORT rel16; // 16 bit relative value
		UINT rel32; // 32 bit relative value

		BYTE condition; // if Fl_condition flag is set, so is this (if its a ja/jne/jnl, this will be set to the condition enum a/ne/nl)

		struct {
			BYTE r8; // 8 bit register
			BYTE r16; // 16 bit register
			BYTE r32; // 32 bit register
			BYTE rxmm; // 64 bit register
			BYTE r_2; // second register in source operand (same bit type as others) (use discretion/no flag indicator yet)
			BYTE mul; // multiplier (0 by default, if there is a multiplier it wont be 0)
			BYTE imm8; // 8 bit offset value
			USHORT imm16; // 16 bit offset value
			UINT imm32; // 32 bit offset value
			BYTE disp8;
			USHORT disp16;
			UINT disp32; // a fixed value (not an 'offset' (+/-))
			BYTE pref; // prefix for this operand (byte/dword/qword ptr, etc.)
		} src;

		struct {
			BYTE r8;
			BYTE r16;
			BYTE r32;
			BYTE rxmm;
			BYTE r_2;
			BYTE mul;
			BYTE imm8;
			USHORT imm16;
			UINT imm32;
			BYTE disp8;
			USHORT disp16;
			UINT disp32; // a fixed value (not an 'offset' (+/-))
			BYTE pref;
		} dest;

		char data[128]; // readable output/translation of the instruction
		char opcode[16]; // just the instruction opcode

		void set_op(const char* x) {
			strcpy_s(opcode, x);
			strcat_s(data, x);
			strcat_s(data, " ");
		}

		// Create a fully blank instruction template
		inst() {
			data[0] = '\0';
			opcode[0] = '\0';

			address = 0;
			len = 0; // defaults to 1 if it cant translate instruction
			pref = 0;
			flags = 0;
			rel8 = 0;
			rel16 = 0;
			rel32 = 0;
			p_rep = 0;
			p_lock = 0;
			p_seg = 0;
			r_mod = 0;
			condition = 0;

			src.r8 = 0;
			src.r16 = 0;
			src.r32 = 0;
			src.rxmm = 0;
			src.r_2 = 0;
			src.mul = 0;
			src.imm8 = 0;
			src.imm16 = 0;
			src.imm32 = 0;
			src.disp8 = 0;
			src.disp16 = 0;
			src.disp32 = 0;
			src.pref = 0;

			dest.r8 = 0;
			dest.r16 = 0;
			dest.r32 = 0;
			dest.rxmm = 0;
			dest.r_2 = 0;
			dest.mul = 0;
			dest.imm8 = 0;
			dest.imm16 = 0;
			dest.imm32 = 0;
			dest.disp8 = 0;
			dest.disp16 = 0;
			dest.disp32 = 0;
			dest.pref = 0;
		}
	};

	// for conversion (used in util headers)
	const char c_ref1[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	const int c_ref2[16] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15 };

	bool DLL_MODE = 0;
	void* handle;
	int base;
	int base_size;

	int from_base(int addr) { return base + addr; }
	int base_start() { return base; }
	int base_end() { return base + base_size; }
	int aslr(int addr) { return (addr - 0x400000 + base); } // resolves an address from IDA pro(0x400000 base) to current process
	int unaslr(int addr) { return (addr + 0x400000 - base); } // does vice versa of the aslr() function

	void use(void* x) {
		if (x == 0)
			throw std::exception("Invalid handle for disassembler");
		else if (x == GetCurrentProcess()) {
			DLL_MODE = true;

			__asm {
				push eax;
				mov eax, fs: [0x30] ;
				mov eax, [eax + 8];
				mov base, eax;
				pop eax;
			}

			MEMORY_BASIC_INFORMATION mbi = { 0 };
			VirtualQuery(reinterpret_cast<void*>(base + 0x1000), &mbi, 0x2C);

			char path[MAX_PATH];
			HMODULE hm = 0;
			GetModuleFileNameA(hm, path, sizeof(path));
			if (std::string(path).find("Roblox") != std::string::npos) {
				base_size = (0x10DF000 - 0x400000); // only go up to the .rdata section
			}
			else {
				base_size = static_cast<UINT>(mbi.RegionSize);
			}
		}
		else { // if were not using a dll . . .
			DLL_MODE = false;

			// read the PEB header anyway, externally.
			// this way we don't need to use psapi.h
			void* code_loc = VirtualAllocEx(x, nullptr, 64, 0x1000, 0x40);
			void* output_loc = (void*)((int)code_loc + 60);

			BYTE PEB_offset = 8;
			BYTE data[] = { 0x55, 0x8B, 0xEC, 0x50, 0x64, 0x8B, 0x5, 0x30, 0, 0, 0, 0x8B, 0x40, PEB_offset, 0xA3, 0xFF, 0xFF, 0xFF, 0xFF, 0x58, 0x5D, 0xC3 };
			*(UINT*)(data + 15) = reinterpret_cast<UINT>(output_loc);

			WriteProcessMemory(x, code_loc, &data, sizeof(data) / sizeof(BYTE), 0);

			HANDLE thread = CreateRemoteThread(x, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(code_loc), 0, 0, NULL);
			WaitForSingleObject(thread, 1000);
			CloseHandle(thread);

			LPVOID buffer = nullptr;
			ReadProcessMemory(x, output_loc, &buffer, sizeof(UINT), NULL);
			base = reinterpret_cast<UINT>(buffer);

			VirtualFreeEx(x, code_loc, 64, MEM_RELEASE);

			MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (VirtualQueryEx(x, reinterpret_cast<void*>(base + 0x1000), &mbi, 0x2C)) {
				base_size = static_cast<UINT>(mbi.RegionSize) - 0x1000;
			}
			else {
				printf("Failed to query process base address..\n");
			}

		}
		handle = x;
	}

	// label can either point to an existing location in memory to be
	// used in the ASM interpreter, or it will allocate new memory at
	// a random location with the provided size, if the address is NULL.
	// this will be used eventually for the ASM writer.
	// 
	struct label {
		char name[64];
		int address;

		label() {
			name[0] = '\0';
			address = 0;
		}

		label(char* x, UINT y, USHORT size = 0) {
			name[0] = '\0';
			strcpy_s(name, x);
			address = y;
			if (address == 0) {
				if (DLL_MODE) {
					address = reinterpret_cast<int>(VirtualAlloc(nullptr, size, 0x1000 | 0x2000, 0x40));
				}
				else {
					address = reinterpret_cast<int>(VirtualAllocEx(handle, nullptr, size, 0x1000 | 0x2000, 0x40));
				}
			}
		}
	};

	enum reg_8 { al, cl, dl, bl, ah, ch, dh, bh };
	const char* c_reg_8[] = { "al",  "cl",  "dl",  "bl",  "ah",  "ch",  "dh",  "bh" };

	enum reg_16 { ax, bx, cx, dx, sp, bp, si, di };
	const char* c_reg_16[] = { "ax",  "bx",  "cx",  "dx",  "sp",  "bp",  "si",  "di" };

	enum reg_32 { eax, ecx, edx, ebx, esp, ebp, esi, edi };
	const char* c_reg_32[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };

	enum reg_xmm { xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7 };
	const char* c_reg_xmm[8] = { "xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7" };

	enum conds { o, no, b, nb, e, ne, na, a, s, ns, p, np, l, nl, le, g };
	const char* c_conds[16] = { "o", "no", "b", "nb", "e", "ne", "na", "a", "s", "ns", "p", "np", "l", "nl", "le", "g" };

	const BYTE mults[] = { 0, 2, 4, 8 };

	inst read(int address) {
		inst p = inst();
		BYTE c = 0;
		BYTE l = 0;
		BYTE* b = new BYTE[16];
		p.address = address;

		if (DLL_MODE) {
			memcpy(b, reinterpret_cast<void*>(address), 16);
		}
		else {
			if (!ReadProcessMemory(handle, reinterpret_cast<void*>(address), b, 16, 0)) {
				free(b);
				throw std::exception("Failed to read process memory");
			}
		}

		if (*b == seg_cs || *b == seg_ss || *b == seg_ds || *b == seg_es || *b == seg_fs || *b == seg_gs) {
			p.p_seg = b[l++];
			p.pref |= pre_seg;
		}
		else if (*b == 0x66) {
			p.pref |= pre_66;
			l++;
		}
		else if (*b == 0x67) {
			p.pref |= pre_67;
			l++;
		}
		else if (*b == 0xF0) {
			p.p_lock = b[l++];
			p.pref |= pre_lock;
			strcat_s(p.data, "lock ");
		}
		else if (*b == 0xF2) {
			p.p_lock = b[l++];
			p.pref |= pre_repne;
			strcat_s(p.data, "repne ");
		}
		else if (*b == 0xF3) {
			p.p_lock = b[l++];
			p.pref |= pre_repe;
			strcat_s(p.data, "repe ");
		}

		if (b[l] == 0x0) {
			p.set_op("add");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x1) {
			p.set_op("add");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x2) {
			p.set_op("add");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x3) {
			p.set_op("add");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x4) {
			p.set_op("add");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x5) {
			p.set_op("add");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x8) {
			p.set_op("or");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x9) {
			p.set_op("or");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0xA) {
			p.set_op("or");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0xB) {
			p.set_op("or");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0xC) {
			p.set_op("or");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0xD) {
			p.set_op("or");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0xF) {
			l++;
			if (b[l] == 0x10) {
				// check for changes all through-out with PRE_F2 and PRE_F3 prefixes
				if (p.pref & pre_repne)	p.set_op("movsd");
				else if (p.pref & pre_repe)		p.set_op("movss");
				else							p.set_op("movups");
				p.dest.pref |= PRE_QWORD_PTR;
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x11) {
				if (p.pref & pre_repne)	p.set_op("movsd");
				else if (p.pref & pre_repe)		p.set_op("movss");
				else							p.set_op("movups");
				p.src.pref |= PRE_QWORD_PTR;
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm;  // swapped
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x12) {
				p.set_op("movhlps");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x13) {
				p.set_op("movlps");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm;
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x14) {
				p.set_op("unpcklps");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x15) {
				p.set_op("unpckhps");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x16) {
				p.set_op("movhps");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x17) {
				p.set_op("movhps");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm; // swapped
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x18) {
				// mode-based conditional prefetch instruction (can throw in later)
			}
			else if (b[l] == 0x1F) { // test this more
				l++;
				if (b[l] % 0x40 < 8) {
					if (p.pref & pre_66)		p.src.pref |= PRE_WORD_PTR;
					else						p.src.pref |= PRE_WORD_PTR;
					p.set_op("nop");
					p.flags |= Fl_src_only | Fl_src_rm32;
					p.r_mod = getmode40(b, l);
					p.src.r32 = getr1(b, l);
				}
			}
			else if (b[l] == 0x28) {
				p.set_op("movaps");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x29) {
				p.set_op("movaps");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm; // swapped
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x2C) {
				p.dest.pref |= PRE_QWORD_PTR;
				p.set_op("cvttsd2si");
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l); // note to self...update all r32 src's for these
			}
			else if (b[l] == 0x2E) {
				p.dest.pref |= PRE_QWORD_PTR;
				p.set_op("ucomisd");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x2F) {
				//p.dest.pref |= PRE_QWORD_PTR;
				p.set_op("comiss");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if ((c = (b[l] & 0x40)) && b[l] < c + 16) {
				p.condition = (b[l] - c);
				char opc[8];
				sprintf_s(opc, "cmov");
				strcat_s(opc, c_conds[p.condition]);
				p.set_op(opc);
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if ((c = (b[l] & 0x50)) && b[l] < c + 16) {
				const char* select_opcodes[] = { "movmskps", "sqrtps", "rsqrtps", "rcpps", "andps", "andnps", "orps", "xorps", "addps", "mulps", "cvtps2pd", "cvtdq2ps", "subps", "minps", "divps", "maxps" };
				p.dest.pref |= PRE_QWORD_PTR;
				p.set_op(select_opcodes[b[l] - c]);
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x6E) {
				p.dest.pref |= PRE_DWORD_PTR;
				p.set_op("movd");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x7E) {
				p.dest.pref |= PRE_QWORD_PTR;
				p.set_op("movq");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.rxmm = getr1(b, l);
			}
			else if (b[l] == 0x7F) {
				p.src.pref |= PRE_QWORD_PTR;
				p.set_op("movdqu");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm;
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if ((c = (b[l] & 0x80)) && b[l] < c + 16) {
				p.condition = (b[l] - c);
				char opc[8];
				sprintf_s(opc, "j");
				strcat_s(opc, c_conds[p.condition]);
				p.set_op(opc);
				p.flags |= Fl_src_only | Fl_rel32;
				l++;
			}
			else if ((c = (b[l] & 0x90)) && b[l] < c + 16) {
				char opc[8];
				sprintf_s(opc, "set");
				strcat_s(opc, c_conds[p.condition]);
				p.set_op(opc);
				p.flags |= Fl_src_only | Fl_src_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if (b[l] == 0xB6) {
				p.dest.pref |= PRE_BYTE_PTR;
				p.set_op("movzx");
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if (b[l] == 0xB7) {
				p.dest.pref |= PRE_WORD_PTR;
				p.set_op("movzx");
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if (b[l] == 0xBE) {
				p.dest.pref |= PRE_BYTE_PTR;
				p.set_op("movsx");
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if (b[l] == 0xBF) {
				p.dest.pref |= PRE_WORD_PTR;
				p.set_op("movsx");
				p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
				p.r_mod = getmode40(b, ++l);
				p.src.r32 = getr1(b, l);
			}
			else if (b[l] == 0xD6) {
				p.src.pref |= PRE_QWORD_PTR;
				p.set_op("movq");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_rxmm;
				p.r_mod = getmode40(b, ++l);
				p.dest.rxmm = getr1(b, l);
			}
			else if (b[l] == 0xE6) {
				// let's break down what we're doing (if you happen
				// to be reading this).
				// The current instruction could be cvtdq2pd eax,xmm0.
				// it's a cvtdq2pd instruction. simple.
				// The bytes will be F3 0F E6 C0.
				// `F3` determines what form of cvtdq2pd to use; (32-bit/16-bit)
				// we'll skip this for now. (I dont even include too many
				// 16-bit instructions; just the most common ones)
				// `0F` implies it's one of these instructions in this set. ignore
				// `E6` implies it's the cvtdq2pd instruction.
				// `C0` implies everything else; it's the `mod` byte.
				// It determines both the src and dest operand, however,
				// if it were below C0 it `might` have an extending 
				// byte that determines more about the src or dest operand.
				// There is alot that goes into this; just know it's
				// the `mod` byte.
				p.dest.pref |= PRE_QWORD_PTR;
				// ^ Prefix flag for a src or dest operand;
				// This implies that, if the `mod` byte is over
				// 0xC0, rather than being		cvtdq2pd xmm0,eax
				// it will instead turn into	cvtdq2pd xmm0,xmm0
				// Many instructions have to have this recognized
				// It also implies that offsets should have a mark;
				// cvtdq2pd xmm0, [eax+04]		will instead show:
				// cvtdq2pd xmm0, QWORD PTR [eax+04].
				// So, this flag is important, but easy to recognize.
				p.set_op("cvtdq2pd");
				p.flags |= Fl_src_dest | Fl_src_rxmm | Fl_dest_rm32;
				// Normal flags; our instruction contains a src and dest
				// so we include Fl_src_dest.
				// First half of the opcode(src) is supposed to be a
				// a floating point register; xmm0-xmm7. Always.
				// so we use Fl_src_rxmm.
				// The second half of the opcode(dest) has to be a
				// 32-bit register(eax,ecx,edx, . . .), but it can also
				// contain a long offset; [eax+4*4], [ebx+ebx], etc.
				// so we would do Fl_dest_rm32 rather than Fl_dest_r32.
				p.r_mod = getmode40(b, ++l);
				// move onto the next byte to interpret, while storing
				// the `mod` identifier byte into r_mod, for help in
				// calculating the rest of the instruction.
				// this is important, since we have to move onto the
				// next byte and allow the system below to finish up
				// computing the instruction.
				p.src.rxmm = getr1(b, l);
				// ^ I can't remember the exact reason for this line,
				// except that it'll guarantee src.rxmm has the correct value.
				// We typically set src.rxmm (or whatever we used for src),
				// IF the `dest` flag is an rm32.
				// And we would set dest.rxmm if the `src` flag is an rm32.
				// This is because the rm32 mode can cause the bytes
				// to be kind of shifted around for src operand
			}
		}
		else if (b[l] == 0x10) {
			p.set_op("adc");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x11) {
			p.set_op("adc");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x12) {
			p.set_op("adc");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x13) {
			p.set_op("adc");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x14) {
			p.set_op("adc");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x15) {
			p.set_op("adc");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x18) {
			p.set_op("sbb");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x19) {
			p.set_op("sbb");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x1A) {
			p.set_op("sbb");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x1B) {
			p.set_op("sbb");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x1C) {
			p.set_op("sbb");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x1D) {
			p.set_op("sbb");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x20) {
			p.set_op("and");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x21) {
			p.set_op("and");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x22) {
			p.set_op("and");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x23) {
			p.set_op("and");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x24) {
			p.set_op("and");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x25) {
			p.set_op("and");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x28) {
			p.set_op("sub");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x29) {
			p.set_op("sub");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x2A) {
			p.set_op("sub");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x2B) {
			p.set_op("sub");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x2C) {
			p.set_op("sub");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x2D) {
			p.set_op("sub");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x30) {
			p.set_op("xor");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x31) {
			p.set_op("xor");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x32) {
			p.set_op("xor");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x33) {
			p.set_op("xor");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x34) {
			p.set_op("xor");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x35) {
			p.set_op("xor");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if (b[l] == 0x38) {
			p.set_op("cmp");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x39) {
			p.set_op("cmp");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x3A) {
			p.set_op("cmp");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x3B) {
			p.set_op("cmp");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x3C) {
			p.set_op("cmp");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = 0;
		}
		else if (b[l] == 0x3D) {
			p.set_op("cmp");
			p.flags |= Fl_src_only | Fl_src_disp32;
			l++;
		}
		else if ((c = (b[l] & 0x40)) && b[l] < c + 0x8) {
			p.set_op("inc");
			p.src.r32 = b[l++] - c;
			p.flags |= Fl_src_only | Fl_src_r32;
		}
		else if ((c = (b[l] & 0x48)) && b[l] < c + 0x8) {
			p.set_op("dec");
			p.src.r32 = b[l++] - c;
			p.flags |= Fl_src_only | Fl_src_r32;
		}
		else if ((c = (b[l] & 0x50)) && b[l] < c + 0x8) {
			p.set_op("push");
			p.src.r32 = b[l++] - c;
			p.flags |= Fl_src_only | Fl_src_r32;
		}
		else if ((c = (b[l] & 0x58)) && b[l] < c + 0x8) {
			p.set_op("pop");
			p.src.r32 = b[l++] - c;
			p.flags |= Fl_src_only | Fl_src_r32;
		}
		else if (b[l] == 0x60)
			l++, p.set_op("pushad");
		else if (b[l] == 0x61)
			l++, p.set_op("popad");
		else if (b[l] == 0x68) {
			l++, p.set_op("push");
			p.flags |= Fl_src_only | Fl_src_disp32;
		}
		else if (b[l] == 0x6A) {
			l++, p.set_op("push");
			p.flags |= Fl_src_only | Fl_src_disp8;
		}
		else if ((c = (b[l] & 0x70)) && b[l] < c + 16) {
			p.condition = (b[l] - c);
			if (p.condition >= 16) throw std::exception("Bad jmp condition index");
			char opc[16];
			sprintf_s(opc, "j");
			strcat_s(opc, c_conds[p.condition]);
			p.set_op(opc);
			p.flags |= Fl_src_only | Fl_rel8;
			l++;
		}
		else if (b[l] == 0x80 || b[l] == 0x82) {
			p.src.pref |= PRE_BYTE_PTR; // unique identifier; dest is imm8 and not r8
			// we have it automatically use r8 if dest is r8 and its in 3rd mode but not in this case
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("add");
			else if (mode == 1) p.set_op("or");
			else if (mode == 2) p.set_op("adc");
			else if (mode == 3) p.set_op("sbb");
			else if (mode == 4) p.set_op("and");
			else if (mode == 5) p.set_op("sub");
			else if (mode == 6) p.set_op("xor");
			else if (mode == 7) p.set_op("cmp");

			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
			p.r_mod = getmode40(b, l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x81) {
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("add");
			else if (mode == 1) p.set_op("or");
			else if (mode == 2) p.set_op("adc");
			else if (mode == 3) p.set_op("sbb");
			else if (mode == 4) p.set_op("and");
			else if (mode == 5) p.set_op("sub");
			else if (mode == 6) p.set_op("xor");
			else if (mode == 7) p.set_op("cmp");

			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp32;
			p.r_mod = getmode40(b, l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x83) {
			p.src.pref |= PRE_DWORD_PTR;
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("add");
			else if (mode == 1) p.set_op("or");
			else if (mode == 2) p.set_op("adc");
			else if (mode == 3) p.set_op("sbb");
			else if (mode == 4) p.set_op("and");
			else if (mode == 5) p.set_op("sub");
			else if (mode == 6) p.set_op("xor");
			else if (mode == 7) p.set_op("cmp");

			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
			p.r_mod = getmode40(b, l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x84) {
			p.src.pref |= PRE_BYTE_PTR;
			p.set_op("test");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x85) {
			p.set_op("test");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x86) {
			p.src.pref |= PRE_BYTE_PTR;
			p.set_op("xchg");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x87) {
			p.set_op("xchg");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x88) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r8;
			p.r_mod = getmode40(b, ++l);
			p.dest.r8 = getr1(b, l);
		}
		else if (b[l] == 0x89) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_r32;
			p.r_mod = getmode40(b, ++l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0x8A) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r8 = getr1(b, l);
		}
		else if (b[l] == 0x8B) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x8D) {
			p.set_op("lea");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_rm32;
			p.r_mod = getmode40(b, ++l);
			p.src.r32 = getr1(b, l);
		}
		else if (b[l] == 0x90) {
			p.set_op("nop");
			p.flags |= Fl_none;
			l++;
		}
		else if (b[l] == 0x9F) {
			p.set_op("lahf");
			p.flags |= Fl_none;
			l++;
		}
		else if (b[l] == 0xA0) {
			p.dest.pref |= PRE_DWORD_PTR;
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp32;
			p.src.r8 = 0;
			l++;
		}
		else if (b[l] == 0xA1) {
			p.dest.pref |= PRE_DWORD_PTR;
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_disp32;
			p.src.r32 = 0;
			l++;
		}
		else if (b[l] == 0xA2) {
			p.src.pref |= PRE_DWORD_PTR;
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_disp32 | Fl_dest_r8;
			p.dest.r8 = 0;
			l++;
		}
		else if (b[l] == 0xA3) {
			p.src.pref |= PRE_DWORD_PTR;
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_disp32 | Fl_dest_r32;
			p.dest.r32 = 0;
			l++;
		}
		else if (b[l] == 0xA8) {
			p.set_op("test");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.src.r8 = 0;
			l++;
		}
		else if (b[l] == 0xA9) {
			p.set_op("test");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_disp32;
			p.src.r32 = 0;
			l++;
		}
		else if ((c = (b[l] & 0xB0)) && b[l] < c + 0x8) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r8 | Fl_dest_disp8;
			p.src.r8 = b[l] - c;
			l++;
		}
		else if ((c = (b[l] & 0xB8)) && b[l] < c + 0x8) {
			p.set_op("mov");
			p.flags |= Fl_src_dest | Fl_src_r32 | Fl_dest_disp32;
			p.src.r32 = b[l] - c;
			l++;
		}
		else if (b[l] == 0xC0) {
			p.src.pref |= PRE_BYTE_PTR;
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("rol");
			else if (mode == 1) p.set_op("ror");
			else if (mode == 2) p.set_op("rcl");
			else if (mode == 3) p.set_op("rcr");
			else if (mode == 4) p.set_op("shl");
			else if (mode == 5) p.set_op("shr");
			else if (mode == 6) p.set_op("rol");
			else if (mode == 7) p.set_op("sar");

			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
			p.r_mod = getmode40(b, l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0xC1) {
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("rol");
			else if (mode == 1) p.set_op("ror");
			else if (mode == 2) p.set_op("rcl");
			else if (mode == 3) p.set_op("rcr");
			else if (mode == 4) p.set_op("shl");
			else if (mode == 5) p.set_op("shr");
			else if (mode == 6) p.set_op("rol");
			else if (mode == 7) p.set_op("sar");

			p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
			p.r_mod = getmode40(b, l);
			p.dest.r32 = getr1(b, l);
		}
		else if (b[l] == 0xC2) {
			p.set_op("ret");
			p.flags |= Fl_src_only | Fl_src_disp16;
			l++;
		}
		else if (b[l] == 0xC3) {
			p.set_op("retn");
			p.flags |= Fl_none;
			l++;
		}
		else if (b[l] == 0xC6) {
			p.src.pref |= PRE_BYTE_PTR;
			int mode = getr1(b, ++l);
			p.r_mod = getmode40(b, l);
			if (mode == 0) {
				p.set_op("mov");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
			}
		}
		else if (b[l] == 0xC7) {
			p.src.pref |= PRE_DWORD_PTR;
			int mode = getr1(b, ++l);
			p.r_mod = getmode40(b, l);
			if (mode == 0) {
				p.set_op("mov");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp32;
			}
		}
		else if (b[l] == 0xCC) {
			p.set_op("align");
			p.flags |= Fl_none;
			l++;
		}
		else if (b[l] == 0xE8) {
			p.set_op("call");
			p.flags |= Fl_src_only | Fl_rel32;
			l++; // start at relative value
		}
		else if (b[l] == 0xE9) {
			p.set_op("jmp");
			p.flags |= Fl_src_only | Fl_rel32;
			l++; // start at relative value
		}
		else if (b[l] == 0xEB) {
			p.set_op("jmp short");
			p.flags |= Fl_src_only | Fl_rel8;
			l++; // start at relative value
		}
		else if (b[l] == 0xF6) {
			int mode = getr1(b, ++l);
			if (mode == 0) { // only a test instruction
				p.src.pref |= PRE_BYTE_PTR;
				p.set_op("test");
				p.flags |= Fl_src_dest | Fl_src_rm32 | Fl_dest_disp8;
				p.r_mod = getmode40(b, l);
				p.dest.r8 = getr1(b, l);
			}
		}
		else if (b[l] == 0xFF) {
			int mode = getr1(b, ++l);
			if (mode == 0)		p.set_op("inc");
			else if (mode == 1) p.set_op("dec");
			else if (mode == 2) p.set_op("call");
			else if (mode == 3) p.set_op("call");
			else if (mode == 4) p.set_op("jmp");
			else if (mode == 5) p.set_op("jmp far");
			else if (mode == 6) p.set_op("push");

			p.src.pref |= PRE_DWORD_PTR;
			p.flags |= Fl_src_only | Fl_src_rm32;
			p.r_mod = getmode40(b, l);
		}








		// Append first operand (source)
		if (p.flags & Fl_src_imm8) { // not fullproof (havent used in src operand yet)
			addoff8(p.data, b, l, p.src.imm8); // 8 bit offset or signed value
		}
		else if (p.flags & Fl_src_imm16) {
			addoff16(p.data, b, l, p.src.imm16); // 16 bit offset or signed value
		}
		else if (p.flags & Fl_src_imm32) {
			addoff32(p.data, b, l, p.src.imm32); // 32 bit offset or signed value
		}
		else if (p.flags & Fl_src_disp8) { // constant unsigned byte value
			p.src.disp8 = *(BYTE*)(b + l);
			l += 1;
			char m[4];
			sprintf_s(m, "%02X", p.src.disp8);
			strcat_s(p.data, m);
		}
		else if (p.flags & Fl_src_disp16) { // constant unsigned short value
			p.src.disp16 = *(USHORT*)(b + l);
			l += 2;
			char m[8];
			sprintf_s(m, "%04X", p.src.disp16);
			strcat_s(p.data, m);
		}
		else if (p.flags & Fl_src_disp32) { // constant unsigned int value
			p.src.disp32 = *(UINT*)(b + l);
			l += 4;
			char m[16];
			sprintf_s(m, "%08X", p.src.disp32);
			strcat_s(p.data, m);
		} if (p.flags & Fl_rel8) {
			p.rel8 = *(BYTE*)(b + l);
			char m[64];
			sprintf_s(m, "base+%08X", ((address + l + 1) + p.rel8) - base);
			l += 1;
			strcat_s(p.data, m);
		}
		else if (p.flags & Fl_rel16) {
			p.rel16 = *(USHORT*)(b + l);
			char m[64];
			sprintf_s(m, "base+%08X", ((address + l + 2) + p.rel16) - base);
			l += 2;
			strcat_s(p.data, m);
		}
		else if (p.flags & Fl_rel32) {
			p.rel32 = *(UINT*)(b + l);
			char m[64];
			sprintf_s(m, "base+%08X", ((address + l + 4) + p.rel32) - base);
			l += 4;
			strcat_s(p.data, m);
		}
		else if (p.flags & Fl_src_r8)
			strcat_s(p.data, c_reg_8[p.src.r8]);
		else if (p.flags & Fl_src_r16)
			strcat_s(p.data, c_reg_16[p.src.r16]);
		else if (p.flags & Fl_src_r32)
			strcat_s(p.data, c_reg_32[p.src.r32]);
		else if (p.flags & Fl_src_rxmm)
			strcat_s(p.data, c_reg_xmm[p.src.rxmm]);
		else if (p.flags & Fl_src_rm32) {
			if (p.r_mod != 3) {
				if (p.src.pref & PRE_BYTE_PTR)
					strcat_s(p.data, "byte ptr");
				else if (p.src.pref & PRE_WORD_PTR)
					strcat_s(p.data, "word ptr");
				else if (p.src.pref & PRE_DWORD_PTR)
					strcat_s(p.data, "dword ptr");
				else if (p.src.pref & PRE_QWORD_PTR)
					strcat_s(p.data, "qword ptr");
				else if (p.pref & pre_seg) {
					switch (p.p_seg) {
					case seg_cs: strcat_s(p.data, "cs:"); break;
					case seg_ss: strcat_s(p.data, "ss:"); break;
					case seg_ds: strcat_s(p.data, "ds:"); break;
					case seg_es: strcat_s(p.data, "es:"); break;
					case seg_fs: strcat_s(p.data, "fs:"); break;
					case seg_gs: strcat_s(p.data, "gs:"); break;
					default: break;
					}
				}

				strcat_s(p.data, "[");
			}

			int mode = b[l++] % 8;
			switch (p.r_mod) {
			case 0: // 0x00-0x3F
				if (mode == 5) {
					p.flags |= Fl_src_disp8;
					p.src.disp32 = *(UINT*)(b + l);
					char m[16];
					sprintf_s(m, "%08X", p.src.disp32);
					strcat_s(p.data, m);
					l += 4;
				}
				else if (mode != 4) {
					p.src.r32 = mode;
					strcat_s(p.data, c_reg_32[p.src.r32]);
				}
				else { // Extended mode with a possible 32-bit offset
					mode = getmode20(b, l);
					p.src.r32 = b[l] % 8;

					// The order that this is done is extremely
					// precise to work for the weird-ass modes
					// intel has in place -- do not change
					if (!(b[l] - (32 * mode) < 8 && (mode % 2 == 1))) {
						if (b[l] % 8 != 5) {
							strcat_s(p.data, c_reg_32[p.src.r32]);
							strcat_s(p.data, "+");
						}

						p.src.r_2 = getr1(b, l);
						strcat_s(p.data, c_reg_32[p.src.r_2]);

						p.src.mul = mults[getmode40(b, l)];
						if (p.src.mul != 0) {
							char m[4];
							sprintf_s(m, "*%i", p.src.mul);
							strcat_s(p.data, m);
						}

						if (b[l] % 8 == 5) {
							p.flags |= Fl_src_imm32;
							l++; addoff32(p.data, b, l, p.src.imm32); l--;
						}
					}
					else {
						if (b[l] - (32 * mode) == 5) {
							p.flags |= Fl_src_imm32;
							l++; addoff32(p.data, b, l, p.src.imm32); l--;
						}
						else
							strcat_s(p.data, c_reg_32[p.src.r32]);
					}
					l++;
				}
				break;
			case 1: case 2: // 0x40-0x7F, 0x80-0xBF
				if (p.r_mod == 1)
					p.flags |= Fl_src_imm8;
				else if (p.r_mod == 2)
					p.flags |= Fl_src_imm32;

				if (mode != 4) {
					p.src.r32 = mode;
					strcat_s(p.data, c_reg_32[p.src.r32]);

					if (p.r_mod == 1) {
						addoff8(p.data, b, l, p.src.imm8);
					}
					else if (p.r_mod == 2) {
						addoff32(p.data, b, l, p.src.imm32);
					}
				}
				else { // Extended mode with a 8-bit or 32-bit offset
					mode = getmode20(b, l);;
					p.src.r32 = b[l] % 8;
					strcat_s(p.data, c_reg_32[p.src.r32]);

					if (!(b[l] - (32 * mode) < 8 && (mode % 2 == 1))) {
						p.src.r_2 = getr1(b, l);
						strcat_s(p.data, "+");
						strcat_s(p.data, c_reg_32[p.src.r_2]);

						p.src.mul = mults[getmode40(b, l)];
						if (p.src.mul != 0) {
							char m[4];
							sprintf_s(m, "*%i", p.src.mul);
							strcat_s(p.data, m);
						}
					}

					l++;
					if (p.r_mod == 1) {
						addoff8(p.data, b, l, p.src.imm8);
					}
					else if (p.r_mod == 2) {
						addoff32(p.data, b, l, p.src.imm32);
					}
				}
				break;
			default: // 0xC0-0xFF
				if (p.flags & Fl_dest_r8 || p.src.pref & PRE_BYTE_PTR) {
					p.src.r8 = mode;
					strcat_s(p.data, c_reg_8[p.src.r8]);
				}
				else if (p.flags & Fl_dest_r16 || p.src.pref & PRE_WORD_PTR) {
					p.src.r16 = mode;
					strcat_s(p.data, c_reg_16[p.src.r16]);
				}
				else if (p.flags & Fl_dest_rxmm || p.src.pref & PRE_QWORD_PTR) {
					p.src.rxmm = mode;
					strcat_s(p.data, c_reg_xmm[p.src.rxmm]);
				}
				else {
					p.src.r32 = mode;
					strcat_s(p.data, c_reg_32[p.src.r32]);
				}
				break;
			}

			if (p.r_mod != 3) strcat_s(p.data, "]");
		}



		// Instruction uses both operands?
		if (p.flags & Fl_src_dest) {
			strcat_s(p.data, ",");

			// Append second operand (destination)
			// Some things are exclusive to the source operand
			// and not applied here
			if (p.flags & Fl_dest_imm8) {
				addoff8(p.data, b, l, p.dest.imm8);
			}
			else if (p.flags & Fl_dest_imm16) {
				addoff16(p.data, b, l, p.dest.imm16);
			}
			else if (p.flags & Fl_dest_imm32) {
				addoff32(p.data, b, l, p.dest.imm32);
			}
			else if (p.flags & Fl_dest_disp8) { // constant unsigned byte value
				p.dest.disp8 = *(BYTE*)(b + l);
				l += 1;
				char m[4];
				sprintf_s(m, "%02X", p.dest.disp8);
				strcat_s(p.data, m);
			}
			else if (p.flags & Fl_dest_disp16) { // constant unsigned short value
				p.dest.disp16 = *(USHORT*)(b + l);
				l += 2;
				char m[8];
				sprintf_s(m, "%04X", p.dest.disp16);
				strcat_s(p.data, m);
			}
			else if (p.flags & Fl_dest_disp32) { // constant unsigned int value
				p.dest.disp32 = *(UINT*)(b + l);
				l += 4;
				char m[16];
				sprintf_s(m, "%08X", p.dest.disp32);
				strcat_s(p.data, m);
			}
			else if (p.flags & Fl_dest_r8)
				strcat_s(p.data, c_reg_8[p.dest.r8]);
			else if (p.flags & Fl_dest_r16)
				strcat_s(p.data, c_reg_16[p.dest.r16]);
			else if (p.flags & Fl_dest_r32)
				strcat_s(p.data, c_reg_32[p.dest.r32]);
			else if (p.flags & Fl_dest_rxmm)
				strcat_s(p.data, c_reg_xmm[p.dest.rxmm]);
			else if (p.flags & Fl_dest_rm32) {
				if (p.r_mod != 3) {
					if (p.dest.pref & PRE_BYTE_PTR)
						strcat_s(p.data, "byte ptr");
					else if (p.dest.pref & PRE_WORD_PTR)
						strcat_s(p.data, "word ptr");
					else if (p.dest.pref & PRE_DWORD_PTR)
						strcat_s(p.data, "dword ptr");
					else if (p.dest.pref & PRE_QWORD_PTR)
						strcat_s(p.data, "qword ptr");
					else if (p.pref & pre_seg) {
						switch (p.p_seg) {
						case seg_cs: strcat_s(p.data, "cs:"); break;
						case seg_ss: strcat_s(p.data, "ss:"); break;
						case seg_ds: strcat_s(p.data, "ds:"); break;
						case seg_es: strcat_s(p.data, "es:"); break;
						case seg_fs: strcat_s(p.data, "fs:"); break;
						case seg_gs: strcat_s(p.data, "gs:"); break;
						default: break;
						}
					}

					strcat_s(p.data, "[");
				}

				int mode = b[l++] % 8;
				switch (p.r_mod) {
				case 0: // 0x00-0x3F
					if (mode == 5) {
						p.flags |= Fl_dest_disp32;
						p.dest.disp32 = *(UINT*)(b + l);
						char m[16];
						sprintf_s(m, "%08X", p.dest.disp32);
						strcat_s(p.data, m);
						l += 4;
					}
					else if (mode != 4) {
						p.dest.r32 = mode;
						strcat_s(p.data, c_reg_32[p.dest.r32]);
					}
					else { // Extended mode with a possible 32-bit offset
						mode = getmode20(b, l);
						p.dest.r32 = b[l] % 8;

						// The order that this is done is extremely precise
						// and important for the computation
						if (!(b[l] - (32 * mode) < 8 && (mode % 2 == 1))) {
							if (b[l] % 8 != 5) {
								strcat_s(p.data, c_reg_32[p.dest.r32]);
								strcat_s(p.data, "+");
							}

							p.dest.r_2 = getr1(b, l);
							strcat_s(p.data, c_reg_32[p.dest.r_2]);

							p.dest.mul = mults[getmode40(b, l)];
							if (p.dest.mul != 0) {
								char m[4];
								sprintf_s(m, "*%i", p.dest.mul);
								strcat_s(p.data, m);
							}

							if (b[l] % 8 == 5) {
								p.flags |= Fl_dest_imm32;
								l++; addoff32(p.data, b, l, p.dest.imm32); l--;
							}
						}
						else {
							if (b[l] - (32 * mode) == 5) {
								p.flags |= Fl_dest_imm32;
								l++; addoff32(p.data, b, l, p.dest.imm32); l--;
							}
							else
								strcat_s(p.data, c_reg_32[p.dest.r32]);
						}
						l++;
					}
					break;
				case 1: case 2: // 0x40-0x7F, 0x80-0xBF
					if (p.r_mod == 1)
						p.flags |= Fl_dest_imm8;
					else if (p.r_mod == 2)
						p.flags |= Fl_dest_imm32;

					if (mode != 4) {
						p.dest.r32 = mode;
						strcat_s(p.data, c_reg_32[p.dest.r32]);

						if (p.r_mod == 1) {
							addoff8(p.data, b, l, p.dest.imm8);
						}
						else if (p.r_mod == 2) {
							addoff32(p.data, b, l, p.dest.imm32);
						}
					}
					else { // Extended mode with a 8-bit or 32-bit offset
						mode = getmode20(b, l);
						p.dest.r32 = b[l] % 8;
						strcat_s(p.data, c_reg_32[p.dest.r32]);

						if (!(b[l] - (32 * mode) < 8 && (mode % 2 == 1))) {
							p.dest.r_2 = getr1(b, l);
							strcat_s(p.data, "+");
							strcat_s(p.data, c_reg_32[p.dest.r_2]);

							p.dest.mul = mults[getmode40(b, l)];
							if (p.dest.mul != 0) {
								char m[4];
								sprintf_s(m, "*%i", p.dest.mul);
								strcat_s(p.data, m);
							}
						}

						l++;
						if (p.r_mod == 1) {
							addoff8(p.data, b, l, p.dest.imm8);
						}
						else if (p.r_mod == 2) {
							addoff32(p.data, b, l, p.dest.imm32);
						}
					}
					break;
				default: // 0xC0-0xFF
					if (p.flags & Fl_src_r8 || p.dest.pref & PRE_BYTE_PTR) {
						p.dest.r8 = mode;
						strcat_s(p.data, c_reg_8[p.dest.r8]);
					}
					else if (p.flags & Fl_dest_r16 || p.dest.pref & PRE_WORD_PTR) {
						p.dest.r16 = mode;
						strcat_s(p.data, c_reg_16[p.dest.r16]);
					}
					else if (p.flags & Fl_dest_rxmm || p.dest.pref & PRE_QWORD_PTR) {
						p.dest.rxmm = mode;
						strcat_s(p.data, c_reg_xmm[p.dest.rxmm]);
					}
					else {
						p.dest.r32 = mode;
						strcat_s(p.data, c_reg_32[p.dest.r32]);
					}
					break;
				}

				if (p.r_mod != 3) strcat_s(p.data, "]");
			}
		}

		p.len = l;
		if (p.len == 0) {
			p.len++; // skip if unidentified
			strcpy_s(p.data, "?");
		}

		free(b);
		return p;
	}

	// if "end" exceeds the base address of the process,
	// it reads all the instructions from begin, to end.
	// if "end" is below the base address of the process,
	// it reads that many instructions starting at "begin"
	//
	// returns all of the instructions in an std::vector
	//
	std::vector<inst> read(int begin, int end) {
		std::vector<inst> o = std::vector<inst>();
		int start = begin;
		if (end > base) {
			while (start < end) {
				inst i = read(start);
				o.push_back(i);
				start += i.len;
			}
		}
		else {
			for (int j = 0; j < end; j++) {
				inst i = read(start);
				o.push_back(i);
				start += i.len;
			}
		}
		return o;
	}

	// if "end" exceeds the base address of the process,
	// it reads all the instructions from begin, to end.
	// if "end" is below the base address of the process,
	// it reads that many instructions starting at "begin"
	//
	// returns all the instructions as a viewable string.
	//
	std::string sread(int begin, int end) {
		std::string o = "";
		int start = begin;
		if (end > base) {
			while (start < end) {
				inst i = read(start);
				o += i.data;
				o += "\n";
				start += i.len;
			}
		}
		else {
			for (int j = 0; j < end; j++) {
				inst i = read(start);
				o += i.data;
				o += "\n";
				start += i.len;
			}
		}
		return o;
	}

	namespace convert {
		short	to_short(BYTE a, BYTE b) { short v = b; v <<= 8; v |= a; return v; }
		int		to_int(BYTE a, BYTE b, BYTE c, BYTE d) { return int(a << 24 | b << 16 | c << 8 | d); }
		int		pbtodw(BYTE* b) { return (b[0]) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24); }
		BYTE* dwtopb(UINT v) { BYTE* data = new BYTE[4]; memcpy(data, &v, 4); return data; }

		// translation
		BYTE to_hex(char* x) {
			if (lstrlenA(x) < 2) return 0;
			if (x[0] == '?' && x[1] == '?') return 0;
			BYTE b = 0;
			for (int i = 0; i < 16; i++) {
				if (x[0] == c_ref1[i]) b += c_ref2[i] * 16;
				if (x[1] == c_ref1[i]) b += i;
			}
			return b;
		}

		std::string to_str(BYTE b) {
			std::string x = "";
			x += c_ref1[b / 16];
			x += c_ref1[b % 16];
			return x;
		}

		std::string to_str(int address) {
			std::string str = "";
			char c[16];
			sprintf_s(c, "%08X", address);
			str += c;
			return str;
		}

		// converts a string representation of an address to a UINT/hex address
		int to_addr(char* addr) {
			if (lstrlenA(addr) < 8) return 0;

			char c1[2], c2[2], c3[2], c4[2];
			c1[0] = addr[0], c1[1] = addr[1];
			c2[0] = addr[2], c2[1] = addr[3];
			c3[0] = addr[4], c3[1] = addr[5];
			c4[0] = addr[6], c4[1] = addr[7];

			return static_cast<int>(to_int(to_hex(c1), to_hex(c2), to_hex(c3), to_hex(c4)));
		}

		std::string to_bytes(int addr) {
			BYTE* x = dwtopb(addr);
			char y[16];
			sprintf_s(y, "%02X%02X%02X%02X", x[0], x[1], x[2], x[3]);
			std::string res(y);
			free(x);
			return res;
		}

		std::string to_bytes(const char* str) {
			std::string x = "";
			for (int i = 0; i < lstrlenA(str); i++) {
				BYTE c = static_cast<BYTE>(str[i]);
				if (i == lstrlenA(str) - 1)
					x += to_str(c);
				else {
					x += to_str(c);
					x += 0x20;
				}
			}
			return x;
		}
	}

	bool isnumber(char c) {
		return (c >= 0x30 && c <= 0x39);
	}
	bool isalphabet(char c) {
		return ((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A));
	}

	std::string nextword(std::string str, int& at) {
		std::string word = "";
		if (!isalphabet(str[at]))	at++;
		if (!isalphabet(str[at]))	return word;
		while (isalphabet(str[at]))	word += str[at++];
		return word;
	}

	// Extremely limited for now (just like most text parsers)
	// Handles basic instructions.. like:
	// write(0xDEADBEEF, "push [ebp+8]");
	// write(0xDEADBEEF+3, "call base+44FE60");
	// https://github.com/thedoomed/EyeStep/blob/master/README.txt
	// 
	inst write(int address, std::string str, std::vector<label>labels = std::vector<label>()) {
		inst i = inst();
		BYTE bytes[16];

		std::string s_opcode = "";
		int at = 0, old_at, bits, sign;
		UINT old_flags;
		while (at < str.length()) {
			if (str[at] == 0x20) break;
			s_opcode += str[at++];
		}

		// No source operand; stand-alone instructions (one-byte in size)
		if (at >= str.length()) {
			i.flags |= Fl_none;

			// -------------------------------------------
			// COMPARE SINGLE OPCODES; CONVERT TO BYTECODE
			// -------------------------------------------
			if (s_opcode == "nop") {
				bytes[i.len++] = 0x90;
			}
			else if (s_opcode == "retn") {
				bytes[i.len++] = 0xC3;
			}
			else if (s_opcode == "int3") {
				bytes[i.len++] = 0xCC;
			}

		}
		else { // Contains either a source, or both source and destination operands
			int sep = str.find(",");
			if (sep == std::string::npos) { // No separator = Single operand instructions
				i.flags |= Fl_src_only;

				// Capture the source operand
				at = str.length();
				while (str[at] != 0x20) at--;
				std::string src_operand = str.substr(++at, str.length() - at);

				at = 0, old_flags = i.flags, old_at = 0;
				std::string r_src = nextword(src_operand, at);
				for (int j = 0; j < 8; j++) {
					if (r_src == std::string(c_reg_8[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm8;
						}
						else {
							i.flags |= Fl_src_r8;
						}
						i.src.r8 = j;
					}
					if (r_src == std::string(c_reg_16[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm16;
						}
						else {
							i.flags |= Fl_src_r16;
						}
						i.src.r16 = j;
					}
					if (r_src == std::string(c_reg_32[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm32;
						}
						else {
							i.flags |= Fl_src_r32;
						}
						i.src.r32 = j;
					}
				}
				if (i.flags == old_flags) at = old_at; // no register? restore position
				if (src_operand.front() == '[' && src_operand.back() == ']' && at == old_at) at++; // no register so maybe a quoted disp32 value? (if so, skip '[')

				// check if we're at a offset after a register (if there /was/ a register)
				sign = 0;
				if (src_operand[at] == '+') sign = 1;
				if (src_operand[at] == '-') sign = -1;

				// (will support multiple registers ("reg+reg*...") soon)
				old_at = at;
				std::string next = nextword(src_operand, at);
				if (next.length() == 3) {
					// [eax+eax...]
					// continue with offset check
				}
				else if (next == "base") {
					// ... ignore (for a jmp or call basically 
					// its easier to do base+)
					if (str[at] == '+') {
						sign = 0; // technically we want a disp32 value not an 'offset'
						at++;	  // even with the '+' after "base"
					}
				}
				else {
					at = old_at; // otherwise skip
				}

				// skip sign (+ or -) or whatever else 
				if (sign != 0) at++;
				// if its not an offset value, read a number value here either way
				// (nothing else would belong in a src operand)
				bits = 0; // get size of value
				while (isnumber(src_operand[at]) || isalphabet(src_operand[at])) bits++, at++;

				if (bits == 2) {
					char x[2];
					x[0] = src_operand[(at - bits) + 0];
					x[1] = src_operand[(at - bits) + 1];
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_src_disp8;
						i.src.disp8 = convert::to_hex(x);
					}
					else {
						i.flags |= Fl_src_imm8;
						i.src.imm8 = convert::to_hex(x);
						if (sign == -1) {
							i.src.imm8 = (0x7F + 1) - i.src.imm8;
						}
					}
				}
				else if (bits == 4) {
					char x[2];
					x[0] = src_operand[(at - bits) + 0];
					x[1] = src_operand[(at - bits) + 1];
					BYTE b1 = convert::to_hex(x);
					x[0] = src_operand[(at - bits) + 2];
					x[1] = src_operand[(at - bits) + 3];
					BYTE b2 = convert::to_hex(x);
					//printf("adding: %02X, %02X.\n", b1, b2);
					if (sign == 0) {
						i.flags |= Fl_src_disp16;
						i.src.disp16 = convert::to_short(b2, b1);
					}
					else {
						i.flags |= Fl_src_imm16;
						i.src.imm16 = convert::to_short(b2, b1);
						if (sign == -1) {
							i.src.imm16 = (0x7FFF + 1) - i.src.imm16;
						}
					}
				}
				else if (bits == 8 || bits == 6) {
					char x[8];
					if (bits == 6) x[0] = '0', x[1] = '0';
					for (int i = 0; i < bits; i++) {
						x[i + (8 - bits)] = src_operand[(at - bits) + i];
					}
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_src_disp32;
						i.src.disp32 = convert::to_addr(x);
						if (next == "base") {
							i.src.disp32 += base;
						}
					}
					else {
						i.flags |= Fl_src_imm32;
						i.src.imm32 = convert::to_addr(x);
						if (sign == -1) {
							i.src.imm32 = (0x7FFFFFFF + 1) - i.src.imm32;
						}
					}
				}


				// ----------------------------------------------
				// COMPARE OPCODES AND FLAGS; CONVERT TO BYTECODE
				// ----------------------------------------------
				if (s_opcode == "inc" && i.flags & Fl_src_r32) {
					bytes[i.len++] = 0x40 + i.src.r32;
				}
				else if (s_opcode == "dec" && i.flags & Fl_src_r32) {
					bytes[i.len++] = 0x48 + i.src.r32;
				}
				else if (s_opcode == "push" && i.flags & Fl_src_r32) {
					bytes[i.len++] = 0x50 + i.src.r32;
				}
				else if (s_opcode == "pop" && i.flags & Fl_src_r32) {
					bytes[i.len++] = 0x58 + i.src.r32;
				}
				else if (s_opcode == "call" && i.flags & Fl_src_disp32) {
					// calculate 32-bit relative call
					i.flags |= Fl_rel32;
					i.rel32 = i.src.disp32 - (address + 5);
					bytes[i.len++] = 0xE8;
					*(UINT*)(bytes + i.len) = i.rel32;
					i.len += 4;
				}
				else if (s_opcode == "jmp" && i.flags & Fl_src_disp32) {
					// calculate an 8-bit relative jmp
					if ((i.src.disp32 - (address + 2)) <= 0x7F || (address - 2) - i.src.disp32 <= 0x7F) {
						i.flags |= Fl_rel8;
						i.rel8 = static_cast<BYTE>(i.src.disp32 - (address + 2));
						bytes[i.len++] = 0xEB;
						bytes[i.len++] = i.rel8;
					}
					else { // otherwise, calculate a 32-bit relative jmp
						i.flags |= Fl_rel32;
						i.rel32 = i.src.disp32 - (address + 5);
						bytes[i.len++] = 0xE9;
						*(UINT*)(bytes + i.len) = i.rel32;
						i.len += 4;
					}
				}
				else if (s_opcode == "ret" && (i.flags & Fl_src_disp16 || i.flags & Fl_src_disp8)) {
					USHORT v = i.src.disp16;
					if (i.flags & Fl_src_disp8) v = static_cast<USHORT>(i.src.disp8);
					bytes[i.len++] = 0xC2;
					*(USHORT*)(bytes + i.len) = v;
					i.len += 2;
				}
				else if (s_opcode == "push" && !(i.flags & Fl_src_r32)) {
					int mode = 0x30;
					bytes[i.len++] = 0xFF;
					if (i.flags & Fl_src_imm32) {
						bytes[i.len++] = mode + 0x80 + i.src.r32;
						*(UINT*)(bytes + i.len) = i.src.imm32;
						i.len += 4;
					}
					else if (i.flags & Fl_src_imm8) {
						bytes[i.len++] = mode + 0x40 + i.src.r32;
						bytes[i.len++] = i.src.imm8;
					}
					else if (i.flags & Fl_src_rm32) {
						bytes[i.len++] = mode + 0x00 + i.src.r32;
					}
				}

				// Check for all conditional jmps
				if (i.flags & Fl_src_disp32) {
					for (int j = 0; j < 16; j++) {
						if (s_opcode == ("j" + std::string(c_conds[j]))) {
							// calculate short jmp instruction
							if ((i.src.disp32 - (address + 2)) <= 0x7F || (address - 2) - i.src.disp32 <= 0x7F) {
								i.flags |= Fl_rel8;
								i.rel8 = static_cast<BYTE>(i.src.disp32) - (address + 2);
								bytes[i.len++] = 0x70 + j;
								bytes[i.len++] = i.rel8;
								break;
							}
							else { // or long jmp instruction
								i.flags |= Fl_rel32;
								i.rel32 = i.src.disp32 - (address + 6);
								bytes[i.len++] = 0x0F;
								bytes[i.len++] = 0x80 + j;
								*(UINT*)(bytes + i.len) = i.rel32;
								i.len += 4;
								break;
							}
						}
					}
				}

			}
			else { // Contains separator; Two-or-more operand instructions
				i.flags |= Fl_src_dest;

				// Capture the source operand
				at = sep;
				while (str[at] != 0x20) at--;
				std::string src_operand = str.substr(++at, sep - at); // start after space

				// Start at 0th character in source operand
				at = 0, old_flags = i.flags, old_at = 0;

				// Get next register
				std::string r_src = nextword(src_operand, at);
				for (int j = 0; j < 8; j++) {
					if (r_src == std::string(c_reg_8[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm8;
						}
						else {
							i.flags |= Fl_src_r8;
						}
						i.src.r8 = j;
					}
					if (r_src == std::string(c_reg_16[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm16;
						}
						else {
							i.flags |= Fl_src_r16;
						}
						i.src.r16 = j;
					}
					if (r_src == std::string(c_reg_32[j])) {
						if (src_operand.front() == '[' && src_operand.back() == ']') {
							i.flags |= Fl_src_rm32;
						}
						else {
							i.flags |= Fl_src_r32;
						}
						i.src.r32 = j;
					}
				}
				if (i.flags == old_flags) at = old_at; // no register? restore position
				if (src_operand.front() == '[' && src_operand.back() == ']' && at == old_at) at++; // no register so maybe a quoted disp32 value? (if so, skip '[')

				// check if we're at a offset after a register (if there /was/ a register)
				sign = 0;
				if (src_operand[at] == '+') sign = 1;
				if (src_operand[at] == '-') sign = -1;

				// (will support multiple registers ("reg+reg*...") soon)
				old_at = at;
				std::string next = nextword(src_operand, at);
				if (next.length() == 3) {
					// [eax+eax...]
					// continue with offset check
				}
				else {
					at = old_at; // otherwise skip
				}

				// skip sign (+ or -) or whatever else 
				if (sign != 0) at++;
				// if its not an offset value, read a number value here either way
				// (nothing else would belong in a src operand)
				bits = 0; // get size of value
				while (isnumber(src_operand[at]) || isalphabet(src_operand[at])) bits++, at++;

				if (bits == 2) {
					char x[2];
					x[0] = src_operand[(at - bits) + 0];
					x[1] = src_operand[(at - bits) + 1];
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_src_disp8;
						i.src.disp8 = convert::to_hex(x);
					}
					else {
						i.flags |= Fl_src_imm8;
						i.src.imm8 = convert::to_hex(x);
						if (sign == -1) {
							i.src.imm8 = (0x7F + 1) - i.src.imm8;
						}
					}
				}
				else if (bits == 4) {
					char x[2];
					x[0] = src_operand[(at - bits) + 0];
					x[1] = src_operand[(at - bits) + 1];
					BYTE b1 = convert::to_hex(x);
					x[0] = src_operand[(at - bits) + 2];
					x[1] = src_operand[(at - bits) + 3];
					BYTE b2 = convert::to_hex(x);
					//printf("adding: %02X, %02X.\n", b1, b2);
					if (sign == 0) {
						i.flags |= Fl_src_disp16;
						i.src.disp16 = convert::to_short(b2, b1);
					}
					else {
						i.flags |= Fl_src_imm16;
						i.src.imm16 = convert::to_short(b2, b1);
						if (sign == -1) {
							i.src.imm16 = (0x7FFF + 1) - i.src.imm16;
						}
					}
				}
				else if (bits == 8 || bits == 6) {
					char x[8];
					if (bits == 6) x[0] = '0', x[1] = '0';
					for (int i = 0; i < bits; i++) {
						x[i + (8 - bits)] = src_operand[(at - bits) + i];
					}
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_src_disp32;
						i.src.disp32 = convert::to_addr(x);
					}
					else {
						i.flags |= Fl_src_imm32;
						i.src.imm32 = convert::to_addr(x);
						if (sign == -1) {
							i.src.imm32 = (0x7FFFFFFF + 1) - i.src.imm32;
						}
					}
				}


				// Capture the destination operand
				at = sep;
				std::string dest_operand = str.substr(++at, str.length() - at);

				// Start at 0th character in destination operand
				at = 0, old_flags = i.flags, old_at = 0;

				// Get next register
				std::string r_dest = nextword(dest_operand, at);
				for (int j = 0; j < 8; j++) {
					if (r_dest == std::string(c_reg_8[j])) {
						if (dest_operand.front() == '[' && dest_operand.back() == ']') {
							i.flags |= Fl_dest_rm8;
						}
						else {
							i.flags |= Fl_dest_r8;
						}
						i.dest.r8 = j;
					}
					if (r_dest == std::string(c_reg_16[j])) {
						if (dest_operand.front() == '[' && dest_operand.back() == ']') {
							i.flags |= Fl_dest_rm16;
						}
						else {
							i.flags |= Fl_dest_r16;
						}
						i.dest.r16 = j;
					}
					if (r_dest == std::string(c_reg_32[j])) {
						if (dest_operand.front() == '[' && dest_operand.back() == ']') {
							i.flags |= Fl_dest_rm32;
						}
						else {
							i.flags |= Fl_dest_r32;
						}
						i.dest.r32 = j;
					}
				}
				if (i.flags == old_flags) at = old_at; // no register? restore position
				if (dest_operand.front() == '[' && dest_operand.back() == ']' && at == old_at) at++; // no register so maybe a quoted disp32 value? (if so, skip '[')

				// check if we're at a offset after a register (if there /was/ a register)
				sign = 0;
				if (dest_operand[at] == '+') sign = 1;
				if (dest_operand[at] == '-') sign = -1;

				// (will support multiple registers ("reg+reg*...") soon)
				old_at = at;
				next = nextword(dest_operand, at);
				if (next.length() == 3) {
					// [eax+eax...]
					// continue with offset check
				}
				else {
					at = old_at; // otherwise skip
				}

				// skip sign (+ or -) or whatever else 
				if (sign != 0) at++;
				// if its not an offset value, read a number value here either way
				// (nothing else would belong in a src operand)
				bits = 0; // get size of value
				while (isnumber(dest_operand[at]) || isalphabet(dest_operand[at])) bits++, at++;

				if (bits == 2) {
					char x[2];
					x[0] = dest_operand[(at - bits) + 0];
					x[1] = dest_operand[(at - bits) + 1];
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_dest_disp8;
						i.dest.disp8 = convert::to_hex(x);
					}
					else {
						i.flags |= Fl_dest_imm8;
						i.dest.imm8 = convert::to_hex(x);
						if (sign == -1) {
							i.dest.imm8 = (0x7F + 1) - i.dest.imm8;
						}
					}
				}
				else if (bits == 4) {
					char x[2];
					x[0] = dest_operand[(at - bits) + 0];
					x[1] = dest_operand[(at - bits) + 1];
					BYTE b1 = convert::to_hex(x);
					x[0] = dest_operand[(at - bits) + 2];
					x[1] = dest_operand[(at - bits) + 3];
					BYTE b2 = convert::to_hex(x);
					//printf("adding: %02X, %02X.\n", b1, b2);
					if (sign == 0) {
						i.flags |= Fl_dest_disp16;
						i.dest.disp16 = convert::to_short(b2, b1);
					}
					else {
						i.flags |= Fl_dest_imm16;
						i.dest.imm16 = convert::to_short(b2, b1);
						if (sign == -1) {
							i.dest.imm16 = (0x7FFF + 1) - i.dest.imm16;
						}
					}
				}
				else if (bits == 8 || bits == 6) {
					char x[8];
					if (bits == 6) x[0] = '0', x[1] = '0';
					for (int i = 0; i < bits; i++) {
						x[i + (8 - bits)] = dest_operand[(at - bits) + i];
					}
					//printf("adding: %s.\n", x);
					if (sign == 0) {
						i.flags |= Fl_dest_disp32;
						i.dest.disp32 = convert::to_addr(x);
					}
					else {
						i.flags |= Fl_dest_imm32;
						i.dest.imm32 = convert::to_addr(x);
						if (sign == -1) {
							i.dest.imm32 = (0x7FFFFFFF + 1) - i.dest.imm32;
						}
					}
				}

				// ----------------------------------------------
				// COMPARE OPCODES AND FLAGS; CONVERT TO BYTECODE
				// ----------------------------------------------
				bool found = false;
				std::vector<std::string>opcodes = { // basic opcodes
					"add","or", "adc","sbb",
					"and","sub","xor","cmp",
					"mov","lea",
				};
				for (int j = 0; j < opcodes.size(); j++)
					if (s_opcode == opcodes[j]) { found = true; break; }
				if (found) {
					if (i.flags & Fl_dest_disp8) {
						int x = 0x00;
						if (s_opcode != "mov") {
							bytes[i.len++] = 0x83;
							if (s_opcode == "add") x += 0x00;
							if (s_opcode == "or")  x += 0x08;
							if (s_opcode == "adc") x += 0x10;
							if (s_opcode == "sbb") x += 0x18;
							if (s_opcode == "and") x += 0x20;
							if (s_opcode == "sub") x += 0x28;
							if (s_opcode == "xor") x += 0x30;
							if (s_opcode == "cmp") x += 0x38;
							x += i.src.r32;

							if (i.flags & Fl_src_r32 || i.flags & Fl_src_r8) {
								bytes[i.len++] = 0xC0 + x;
							}
							else if (i.flags & Fl_src_imm32) {
								bytes[i.len++] = 0x80 + x;
								*(UINT*)(bytes + i.len) = i.src.imm32;
								i.len += 4;
							}
							else if (i.flags & Fl_src_imm8) {
								bytes[i.len++] = 0x40 + x;
								bytes[i.len++] = i.src.imm8;
							}
							else {
								bytes[i.len++] = 0x00 + x;
							}
						}
						else {
							bytes[i.len++] = 0xC6;
							if (i.flags & Fl_src_r32 || i.flags & Fl_src_r8) {
								bytes[i.len++] = 0xC0 + i.src.r32;
							}
							else if (i.flags & Fl_src_imm32) {
								bytes[i.len++] = 0x80 + i.src.r32;
								*(UINT*)(bytes + i.len) = i.src.imm32;
								i.len += 4;
							}
							else if (i.flags & Fl_src_imm8) {
								bytes[i.len++] = 0x40 + i.src.r32;
								bytes[i.len++] = i.src.imm8;
							}
							else {
								bytes[i.len++] = 0x00 + i.src.r32;
							}
						}
						bytes[i.len++] = i.dest.disp8;
					}
					// example: mov eax,[15000000] or, mov eax,15000000
					// probably the same opcode byte as the first mode
					else if (i.flags & Fl_dest_disp32) {
						if (dest_operand.front() == '[' && dest_operand.back() == ']') {
							if (s_opcode == "add") bytes[i.len++] = 0x03;
							if (s_opcode == "or")  bytes[i.len++] = 0x0B;
							if (s_opcode == "adc") bytes[i.len++] = 0x13;
							if (s_opcode == "sbb") bytes[i.len++] = 0x1B;
							if (s_opcode == "and") bytes[i.len++] = 0x23;
							if (s_opcode == "sub") bytes[i.len++] = 0x2B;
							if (s_opcode == "xor") bytes[i.len++] = 0x33;
							if (s_opcode == "cmp") bytes[i.len++] = 0x3B;
							if (s_opcode == "mov") bytes[i.len++] = 0x8B;

							i.src.r32 *= 8;
							i.src.r32 += 5;
							bytes[i.len++] = 0x00 + i.src.r32 + i.dest.r32;
							*(UINT*)(bytes + i.len) = i.dest.disp32;
							i.len += 4;
						}
						else {
							if (s_opcode == "add") bytes[i.len++] = 0x05;
							if (s_opcode == "or")  bytes[i.len++] = 0x0D;
							if (s_opcode == "adc") bytes[i.len++] = 0x15;
							if (s_opcode == "sbb") bytes[i.len++] = 0x1D;
							if (s_opcode == "and") bytes[i.len++] = 0x25;
							if (s_opcode == "sub") bytes[i.len++] = 0x2D;
							if (s_opcode == "xor") bytes[i.len++] = 0x35;
							if (s_opcode == "cmp") bytes[i.len++] = 0x3D;
							if (s_opcode == "mov") bytes[i.len++] = 0xB8 + i.src.r32;

							*(UINT*)(bytes + i.len) = i.dest.disp32;
							i.len += 4;
						}
						// example: mov eax,[eax] or, mov eax,[eax+??]
					}
					else if (i.flags & Fl_src_r32) {
						if (s_opcode == "add") bytes[i.len++] = 0x03;
						if (s_opcode == "or")  bytes[i.len++] = 0x0B;
						if (s_opcode == "adc") bytes[i.len++] = 0x13;
						if (s_opcode == "sbb") bytes[i.len++] = 0x1B;
						if (s_opcode == "and") bytes[i.len++] = 0x23;
						if (s_opcode == "sub") bytes[i.len++] = 0x2B;
						if (s_opcode == "xor") bytes[i.len++] = 0x33;
						if (s_opcode == "cmp") bytes[i.len++] = 0x3B;
						if (s_opcode == "mov") bytes[i.len++] = 0x8B;
						if (s_opcode == "lea") bytes[i.len++] = 0x8D;
						i.src.r32 *= 8;

						if (i.flags & Fl_dest_r32)
							bytes[i.len++] = 0xC0 + i.src.r32 + i.dest.r32;
						else if (i.flags & Fl_dest_imm32) {
							bytes[i.len++] = 0x80 + i.src.r32 + i.dest.r32;
							*(UINT*)(bytes + i.len) = i.dest.imm32;
							i.len += 4;
						}
						else if (i.flags & Fl_dest_imm8) {
							bytes[i.len++] = 0x40 + i.src.r32 + i.dest.r32;
							bytes[i.len++] = i.dest.imm8;
						}
						else if (i.flags & Fl_dest_rm32) {
							bytes[i.len++] = 0x00 + i.src.r32 + i.dest.r32;
						}
						// example: mov [eax],eax or, mov [eax+??],eax
					}
					else if (i.flags & Fl_src_rm32) {
						if (s_opcode == "add") bytes[i.len++] = 0x01;
						if (s_opcode == "or")  bytes[i.len++] = 0x09;
						if (s_opcode == "adc") bytes[i.len++] = 0x11;
						if (s_opcode == "sbb") bytes[i.len++] = 0x19;
						if (s_opcode == "and") bytes[i.len++] = 0x21;
						if (s_opcode == "sub") bytes[i.len++] = 0x29;
						if (s_opcode == "xor") bytes[i.len++] = 0x31;
						if (s_opcode == "cmp") bytes[i.len++] = 0x39;
						if (s_opcode == "mov") bytes[i.len++] = 0x89;
						i.dest.r32 *= 8;

						// dont use 0xC0 as the above takes care of that mode
						if (i.flags & Fl_src_imm32) {
							bytes[i.len++] = 0x80 + i.src.r32 + i.dest.r32;
							*(UINT*)(bytes + i.len) = i.src.imm32;
							i.len += 4;
						}
						else if (i.flags & Fl_src_imm8) {
							bytes[i.len++] = 0x40 + i.src.r32 + i.dest.r32;
							bytes[i.len++] = i.src.imm8;
						}
						else {
							bytes[i.len++] = 0x00 + i.src.r32 + i.dest.r32;
						}
						// example: mov [15000000],eax
					}
					else if (i.flags & Fl_src_disp32) {
						if (s_opcode == "add") bytes[i.len++] = 0x01;
						if (s_opcode == "or")  bytes[i.len++] = 0x09;
						if (s_opcode == "adc") bytes[i.len++] = 0x11;
						if (s_opcode == "sbb") bytes[i.len++] = 0x19;
						if (s_opcode == "and") bytes[i.len++] = 0x21;
						if (s_opcode == "sub") bytes[i.len++] = 0x29;
						if (s_opcode == "xor") bytes[i.len++] = 0x31;
						if (s_opcode == "cmp") bytes[i.len++] = 0x39;
						if (s_opcode == "mov") bytes[i.len++] = 0x89;

						i.dest.r32 *= 8;
						i.dest.r32 += 5;
						bytes[i.len++] = 0x00 + i.src.r32 + i.dest.r32;
						*(UINT*)(bytes + i.len) = i.src.disp32;
						i.len += 4;
					}
				}
			}
		}

		if (DLL_MODE) {
			memcpy(reinterpret_cast<void*>(address), &bytes, i.len);
		}
		else {
			if (!WriteProcessMemory(handle, reinterpret_cast<void*>(address), &bytes, i.len, 0)) {
				throw std::exception("Failed to write process memory");
			}
		}

		return i;
	}
}


#endif