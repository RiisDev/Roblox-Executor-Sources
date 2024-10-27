#ifndef EYESTEP_UTIL
#define EYESTEP_UTIL
#include "Eyestepold.h"
#define pmread	 ReadProcessMemory
#define pmwrite	 WriteProcessMemory
#define vcast(x) reinterpret_cast<void*>(x)

namespace eyestep {
	typedef uint8_t conv;
	conv conv_cdecl = 0;
	conv conv_stdcall = 1;
	conv conv_thiscall = 2;
	conv conv_fastcall = 3;

	namespace exeutil {
		const int MAX_STR_READ = 1024;

		namespace edit {
			struct PROTECT_INFO {
				uint32_t address;
				size_t size;
				DWORD oldProtect;
			} last_unprotect;

			PROTECT_INFO unprotect(uint32_t address, size_t size) {
				PROTECT_INFO x;
				x.address = address;
				x.size = size;
				VirtualProtectEx(handle, vcast(address), size, 0x40, &x.oldProtect);
				last_unprotect = x;
				return x;
			}

			// restores page protection after calling unprotect()
			void restore(PROTECT_INFO x) {
				DWORD nothing = 0;
				VirtualProtectEx(handle, vcast(x.address), x.size, x.oldProtect, &nothing);
			}
			void restore() { restore(last_unprotect); }
		}

		struct cbyte {
			std::vector<uint8_t>bytes;
			void add(uint8_t b) {
				bytes.push_back(b);
			}
			size_t size() {
				return bytes.size();
			}
			// constructors
			cbyte() {
				bytes = std::vector<uint8_t>();
			}
			cbyte(const char* str) {
				bytes = std::vector<uint8_t>();
				std::string newstr = "";
				for (int i = 0; i < lstrlenA(str); i++) {
					if (str[i] != 0x20) {
						newstr += str[i];
					}
				}
				if ((newstr.length() / 2) > 0 && (newstr.length() % 2) == 0) {
					for (int i = 0; i < newstr.length(); i += 2) {
						char s[2];
						s[0] = newstr[i];
						s[1] = newstr[i + 1];
						add(convert::to_hex(s));
					}
				}
			}
			cbyte(uint8_t* byte_array, size_t count) {
				bytes = std::vector<uint8_t>();
				if (sizeof(byte_array) > 0) {
					for (int i = 0; i < count; i++) {
						add(byte_array[i]);
					}
				}
			}
			// cool functions
			uint8_t at(int index) {
				if (index < bytes.size() && index >= 0) {
					return bytes[index];
				}
				else {
					throw std::exception("BAD CBYTE INDEX\n");
					return 0;
				}
			}
			std::string to_string() {
				std::string str = "";
				for (int i = 0; i < bytes.size(); i++) {
					str += convert::to_str(bytes[i]);
					if (i != (size() - 1))
						str += ", ";
					else
						str += ".";
				}
				return str;
			};
		};

		uint8_t* readb(uint32_t addr, size_t count) { uint8_t* x = new uint8_t[count]; pmread(handle, vcast(addr), x, count, 0); return x; }
		uint8_t readb(uint32_t addr) { uint8_t		x = 0; pmread(handle, vcast(addr), &x, 1, 0); return x; }
		uint16_t readus(uint32_t addr) { uint16_t	x = 0; pmread(handle, vcast(addr), &x, 2, 0); return x; }
		uint32_t readui(uint32_t addr) { uint32_t	x = 0; pmread(handle, vcast(addr), &x, 4, 0); return x; }
		uint64_t readull(uint32_t addr) { uint64_t	x = 0; pmread(handle, vcast(addr), &x, 8, 0); return x; }
		char	readc(uint32_t addr) { char			x = 0; pmread(handle, vcast(addr), &x, 1, 0); return x; }
		int16_t reads(uint32_t addr) { int16_t		x = 0; pmread(handle, vcast(addr), &x, 2, 0); return x; }
		int32_t readi(uint32_t addr) { int32_t		x = 0; pmread(handle, vcast(addr), &x, 4, 0); return x; }
		int64_t readll(uint32_t addr) { int64_t		x = 0; pmread(handle, vcast(addr), &x, 8, 0); return x; }
		float readf(uint32_t addr) { float			x = 0; pmread(handle, vcast(addr), &x, 4, 0); return x; }
		double readd(uint32_t addr) { double		x = 0; pmread(handle, vcast(addr), &x, 8, 0); return x; }
		void write(uint32_t addr, uint8_t* v, size_t count) { pmwrite(handle, vcast(addr), v, count, 0); }
		void write(uint32_t addr, uint8_t v) { pmwrite(handle, vcast(addr), &v, 1, 0); }
		void write(uint32_t addr, uint16_t v) { pmwrite(handle, vcast(addr), &v, 2, 0); }
		void write(uint32_t addr, uint32_t v) { pmwrite(handle, vcast(addr), &v, 4, 0); }
		void write(uint32_t addr, uint64_t v) { pmwrite(handle, vcast(addr), &v, 8, 0); }
		void write(uint32_t addr, char v) { pmwrite(handle, vcast(addr), &v, 1, 0); }
		void write(uint32_t addr, int16_t v) { pmwrite(handle, vcast(addr), &v, 2, 0); }
		void write(uint32_t addr, int32_t v) { pmwrite(handle, vcast(addr), &v, 4, 0); }
		void write(uint32_t addr, int64_t v) { pmwrite(handle, vcast(addr), &v, 8, 0); }
		void write(uint32_t addr, float v) { pmwrite(handle, vcast(addr), &v, 4, 0); }
		void write(uint32_t addr, double v) { pmwrite(handle, vcast(addr), &v, 8, 0); }
		void write(uint32_t addr, cbyte v) { pmwrite(handle, vcast(addr), v.bytes.data(), v.bytes.size(), 0); }


		std::string sreadb(uint32_t addr, size_t count) {
			std::string str = "";
			if (count != 0) {
				uint8_t* x = new uint8_t[count];
				pmread(handle, vcast(addr), x, count, 0);
				for (int i = 0; i < count; i++) {
					str += convert::to_str(x[i]);
				}
				delete x;
			}
			return str;
		}

		std::string readstring(uint32_t addr) {
			uint32_t ptr = readui(addr);
			if (!(ptr % 4 == 0 && ptr > base && ptr < 0x2FFFFFFF)) {
				ptr = addr;
			}
			std::string str = "";
			char c[MAX_STR_READ];
			pmread(handle, vcast(ptr), &c, MAX_STR_READ, 0);
			int i = 0;
			while (i < MAX_STR_READ && c[i] >= 0x20 && c[i] <= 0x7E) {
				str += c[i++];
			}
			return str;
		}

		bool isepilogue(uint32_t address) {
			uint8_t* b = readb(address, 2);
			bool x = ((b[0] == 0x5D || b[0] == 0x5E) && // pop ebp, or pop esi,
				(b[1] == 0xC2 || b[1] == 0xC3));  // with a retn or ret XX
			delete b;
			return x;
		}

		bool isprologue(uint32_t address) {
			if (!(address % 16 == 0)) return false;
			std::string s = sreadb(address, 3);
			if (s == "558BEC") return true; // standard prologue
			if (s == "568BF1") return true; // some prologues use ESI
			return false;
		}

		uint32_t nextprologue(uint32_t address, direction d, bool aligned = true) {
			uint32_t at = address, count = 0;
			// Skip this prologue if we're already at one
			if (isprologue(at)) {
				if (d == behind) at -= 16;
				if (d == ahead)  at += 16;
			}
			while (!isprologue(at)) {
				if (count++ > 0xFFFF) break;
				if (d == ahead)  if (!aligned) at++; else at += 16;
				if (d == behind) if (!aligned) at--; else at -= 16;
			}
			return at;
		}

		uint32_t nextepilogue(uint32_t address, direction d) {
			uint32_t at = address, count = 0;
			while (!isepilogue(at)) {
				if (count++ > 0xFFFF) break;
				if (d == ahead)  at++;
				if (d == behind) at--;
			}
			return (at + 1); // Return the functions retn address
		}

		int fsize(uint32_t func) {
			uint32_t eof = nextprologue(func, ahead, true);
			return static_cast<int>(eof - func);
		}

		uint32_t getprologue(uint32_t addr) {
			return nextprologue(addr, behind, false);
		}

		std::vector<DWORD> getprologues(uint32_t func, direction d, int count) {
			std::vector<DWORD> result_list = std::vector<DWORD>();
			uint32_t addr = func, current = 0;
			while (current < count) {
				addr = nextprologue(addr, d, true);
				result_list.push_back(addr);
				current++;
			}
			return result_list;
		}

		uint32_t getepilogue(uint32_t func) {
			return nextepilogue(nextprologue(func, ahead, true), behind);
		}

		std::vector<uint32_t> getepilogues(uint32_t func) {
			std::vector<uint32_t> result_list = std::vector<uint32_t>();
			uint32_t start = func;
			uint32_t end = (start + fsize(func));
			while (start < end) {
				if (isepilogue(start)) {
					result_list.push_back(start + 1);
				}
				start++;
			}
			return result_list;
		}

		short fretn(uint32_t func) {
			for (uint32_t addr : getepilogues(func)) {
				if (readb(addr) == 0xC2) {
					return reads(addr + 1);
				}
			}
			return 0;
		}

		std::vector<DWORD> getcalls(uint32_t func) {
			std::vector<DWORD> result_list = std::vector<DWORD>();
			uint32_t funcSize = fsize(func);
			for (int i = 0; i < funcSize; i++) {
				if (readb(func + i) == 0xE8) {
					uint32_t o = (func + i) + readui(func + i + 1) + 5;
					if (o % 16 == 0 && o > base && o < base + base_size) {
						result_list.push_back(o);
					}
				}
			}
			return result_list;
		}

		std::vector<DWORD> getpointers(uint32_t func) {
			std::vector<DWORD> result_list = std::vector<DWORD>();
			uint32_t start = func;
			uint32_t end = (func + fsize(func));
			while (start < end) {
				inst i = read(start);
				uint32_t p = 0;
				if (i.flags & Fl_src_disp32)	p = i.src.disp32;
				if (i.flags & Fl_dest_disp32)	p = i.dest.disp32;
				if (i.flags & Fl_src_imm32)		p = i.src.imm32;
				if (i.flags & Fl_dest_imm32)	p = i.dest.imm32;
				if (p != 0) {
					if (p % 4 == 0 && p > base && p < base + base_size) result_list.push_back(p);
					if (p % 4 == 0 && p > base && p < base + base_size) result_list.push_back(p);
				}
				start += i.len;
			}
			return result_list;
		}

		uint32_t nextcall(uint32_t func, direction d, bool loc = false) {
			uint32_t start = func;
			// Skip current call if we're already at one
			if (readb(start) == 0xE8) {
				if (d == ahead)  start++;
				if (d == behind) start--;
			}
			while (readb(start) != 0xE8) {
				if (d == ahead)  start++;
				if (d == behind) start--;
			}
			uint32_t o = (start + readui(start + 1) + 5);
			if (o % 16 == 0 && o > base && o < base + base_size)
				if (!loc)
					return o;
				else
					return start;
			return 0;
		}

		uint32_t getcall(uint32_t addr) {
			uint32_t o = (addr + readui(addr + 1) + 5);
			if (o % 16 == 0 && o > base && o < base + base_size)
				return o;
			return 0;
		}

		conv getconv(uint32_t func) {
			conv c = conv_cdecl;
			uint32_t eof = nextprologue(func, ahead);
			uint32_t at = func;
			bool not_fastcall = false;
			bool neither = false;
			if (fretn(func))  c = conv_stdcall;

			while (at < eof) {
				inst i = eyestep::read(at);
				at += i.len;
				// if edx or ecx was pushed thats an instant indication
				if (strcmp(i.opcode, "push") == 0) {
					if (i.src.r32 == ecx) neither = true;
					if (i.src.r32 == edx) not_fastcall = true;
				}
				// completely ignore a test instruction, with ecx,ecx or edx,edx
				if (strcmp(i.opcode, "test") == 0) {
					if ((i.src.r32 == ecx && i.dest.r32 == ecx) || (i.src.r32 == edx && i.dest.r32 == edx)) {
						continue;
					}
				}
				// DONT ignore a xor instruction, with ecx,ecx or edx,edx
				if (strcmp(i.opcode, "xor") == 0) {
					if (i.src.r32 == ecx && i.dest.r32 == ecx) {
						neither = true;
						continue;
					}
					else if (i.src.r32 == edx && i.dest.r32 == edx) {
						not_fastcall = true;
						continue;
					}
				}
				if (i.flags & Fl_src_r32 || i.flags & Fl_src_rm32) {
					if (strcmp(i.opcode, "mov") == 0 || strcmp(i.opcode, "lea") == 0 ||
						strcmp(i.opcode, "movzx") == 0 || strcmp(i.opcode, "or") == 0 ||
						strcmp(i.opcode, "xor") == 0
						) {
						// because something like a mov ecx,edx
						// can also throw things off
						if (i.dest.r32 == edx && !neither && !not_fastcall) {
							return conv_fastcall;
						}
						// it will take a mov or lea instruction with ecx or edx
						// to indicate it is NOT a fastcall or thiscall.
						// But, something like "or ecx,1" has ecx in the src operand,
						// and its using it as if it was passed to the function
						if (i.src.r32 == ecx) neither = true;
						if (i.src.r32 == edx) not_fastcall = true;
					}
				}
				// now, we check if an instruction USES edx or ecx after
				// all that has passed, now that it's determined whether
				// it was or wasn't passed to this function
				if (i.flags & Fl_dest_r32 || i.flags & Fl_dest_rm32) {
					if (i.dest.r32 == edx) {
						if (!neither && !not_fastcall) {
							return conv_fastcall;
						}
					}
					if (i.dest.r32 == ecx) {
						if (c != conv_fastcall && !neither) {
							c = conv_thiscall;
						}
					}
					if (not_fastcall && c == conv_thiscall) {
						return conv_thiscall;
					}
				}
			}

			return c;
		}

		const char* getsconv(conv c) {
			return (c == 0) ? "cdecl" : (c == 1) ? "stdcall" : (c == 2) ? "thiscall" : (c == 3) ? "fastcall" : "";
		}

		// go from [start] location, and search for a call to "func"
		// in the specificied direction.
		// Returns the location of the call.
		// if parentfunc is true it returns the function
		// that has this call in it
		uint32_t nextxref(uint32_t start, direction d, uint32_t func, bool parentfunc = false) {
			uint32_t at = start;
			while (getcall(at) != func) {
				uint32_t old_at = at;
				at = nextcall(at, d, true);
				if (at == 0) if (d == behind) at = --old_at; else at = ++old_at;
			}
			if (!parentfunc) return at;
			return nextprologue(at, behind, false);
		}

		std::vector<DWORD> __fastcall scan(const char* aob, const char* _mask, uint32_t begin = 0, uint32_t to = 0) {
			uint32_t size = 0, at = 0, start = begin, end = to;
			if (!(start && end)) {
				start = base;
				end = base + base_size;
			}

			uint32_t chunksize = ((end - start) < (64 * 64 * 64)) ? (end - start) : (64 * 64 * 64);
			uint8_t* buffer = new uint8_t[chunksize];
			char match = 1;
			char wildchar = 'x'; // 'x' means it can be any byte; put '.' for the rest

			std::vector<DWORD>	results_list = std::vector<DWORD>();
			std::vector<uint8_t>	bytes = cbyte(aob).bytes;
			size = bytes.size();

			// create mask for AOB if string is empty
			char mask[256];
			mask[0] = '\0';
			if (lstrlenA(_mask) == 0) {
				for (int i = 0; i < size; i++) {
					strcat_s(mask, ".");
				}
			}
			else {
				strcpy_s(mask, _mask);
			}
			uint32_t padding = (size - 1);
			while (start < end){
				bool read = pmread(handle, vcast(begin), buffer, chunksize, 0);
				if (read){
					__asm push edi
					__asm xor edi, edi
					__asm jmp L2
				L1: __asm inc edi
					__asm mov at, edi
					match=1;
					for (uint32_t x=0; x<size; x++)
					if (buffer[at+x]!=bytes.at(x) && mask[x]!=wildchar)
					match=0;
					if (match) results_list.push_back(start+at);
				L2:	__asm cmp edi, chunksize
					__asm jb L1
					__asm sub edi, padding
					__asm add start, edi
					__asm pop edi
				} else {
					printf("[!] Failed to read process region [0x%08X]\n", start);
					start += chunksize;
					continue;
				}
			}
			delete buffer;
			return results_list;
		}

		std::vector<DWORD> __fastcall scanpointer(uint32_t address) {
			return scan(convert::to_abytes(address).c_str(), "....");
		}

		// strings are located onward from .rdata
		// roblox detects this and you're kicked, and I never got
		// around to a bypass for it; thats why eyecrawl never worked as a DLL
		std::vector<DWORD> __fastcall scanxrefs(const char* str, int result_number = 0) {
			char* mask = new char[lstrlenA(str)];
			for (int i = 0; i < lstrlenA(str); i++) {
				mask[i] = '.';
			}
			std::vector<DWORD>strlocs = scan(convert::to_bytes(str).c_str(), mask);
			delete mask;
			if (strlocs.size() == 0) return strlocs;
			return scanpointer(strlocs[result_number]);
		}

		// xrefs made to a function
		std::vector<uint32_t> __fastcall scanxrefs(uint32_t func, uint32_t begin = 0, uint32_t to = 0) {
			uint32_t size = 0, at = 0, start = begin, end = to;
			if (!(start && end)) {
				start = eyestep::base;
				end = eyestep::base + eyestep::base_size;
			}

			uint32_t chunksize = ((end - begin) < (64 * 64 * 32)) ? (end - begin) : (64 * 64 * 32);
			uint8_t* buffer = new uint8_t[chunksize];
			bool found = false;
			std::vector<uint32_t>	xrefs = std::vector<uint32_t>();

			while (start < end && start < base + base_size) {
				bool read = pmread(handle, vcast(start), buffer, chunksize, 0);
				if (read) {
					for (int i = 0; i < chunksize; i++) {
						if (readui(start + i) == 0xE8) { // call instruction
							// calculate relative offset
							uint32_t o = readui(start + i + 1);
							std::cout << "CHUNK: " << i << " / SUB: " << std::hex << eyestep::unaslr(start + i) << "\n";
							if ((start + i + 5 + o) == func) {
								xrefs.push_back(start + i);
								found = true;
								break;
							}
							i += 4;
						}
					}
				}
				if (found) break;
				start += chunksize - 5;
			}
			delete buffer;
			return xrefs;
		}

		// places a hook that removes itself, after
		// reading an offset of a register
		uint32_t debug(uint32_t address, reg_32 r32, int offset = 0) {
			uint32_t size = 5, nop, isize = 0, value = 0, at = 0;
			uint32_t code_loc = reinterpret_cast<uint32_t>(VirtualAllocEx(handle, nullptr, 64, 0x1000, 0x40));
			uint32_t trace_loc = reinterpret_cast<uint32_t>(VirtualAllocEx(handle, nullptr, 8, 0x1000, 0x4));
			uint32_t hit_loc = trace_loc + 4;

			// Interpret size of original instruction(s)
			inst i = read(address);
			while (i.address < (address + size)) {
				isize += i.len;
				nop = ((i.address + i.len) - (address + size));
				i = read(address + isize);
			}

			// store original bytes
			uint8_t* old_bytes = new uint8_t[size + nop];
			pmread(handle, vcast(address), old_bytes, size + nop, 0);

			// Make a JMP from the address to our own code
			uint8_t* code_data = new uint8_t[64];
			uint8_t* inject = new uint8_t[5];
			*(uint8_t*)(inject) = 0xE9;
			*(uint32_t*)(inject + 1) = (code_loc - (address + 5));

			if (offset == 0) { // simply place one instruction to capture 
				// the value of the register to our readout location
				*(uint8_t*)(code_data + at++) = (0x50 + r32); // push (r32)
				switch (r32) {
				case reg_32::eax:
					*(uint8_t*)(code_data + at++) = 0xA3;
					break;
				default:
					*(uint8_t*)(code_data + at++) = 0x89; // ecx-edi (0xD,0x15,0x1D,0x25,0x2D . . .)
					*(uint8_t*)(code_data + at++) = (0x5 + (r32 * 8));
					break;
				}
				// trace the register to our location
				*(uint32_t*)(code_data + at) = trace_loc; at += 4;
				// hit detected
				*(uint8_t*)(code_data + at++) = 0xC7;
				*(uint8_t*)(code_data + at++) = 0x5;
				*(uint32_t*)(code_data + at) = hit_loc;	at += 4;
				*(uint32_t*)(code_data + at) = 1;		at += 4;
				// pop (r32)
				*(uint8_t*)(code_data + at++) = (0x58 + r32);
			}
			else { // or, if we want an offset of a register ...
			 // move the offset into EAX and show EAX
				*(uint8_t*)(code_data + at++) = 0x50; // push eax
				*(uint8_t*)(code_data + at++) = 0x8B;
				if (offset > UCHAR_MAX) {	// 32 bit offset
					if (r32 != reg_32::esp)
						*(uint8_t*)(code_data + at++) = (0x80 + r32);
					else {
						*(uint8_t*)(code_data + at++) = 0x84;
						*(uint8_t*)(code_data + at++) = 0x24;
					}
					*(uint32_t*)(code_data + at++) = offset;
				}
				else {					// 8 bit offset
					if (r32 != reg_32::esp)
						*(uint8_t*)(code_data + at++) = (0x40 + r32);
					else {
						*(uint8_t*)(code_data + at++) = 0x44;
						*(uint8_t*)(code_data + at++) = 0x24;
					}
					*(uint8_t*)(code_data + at++) = offset;
				}
				// Trace register to our trace location
				*(uint8_t*)(code_data + at++) = 0xA3;
				*(uint32_t*)(code_data + at) = trace_loc; at += 4;
				// hit detected
				*(uint8_t*)(code_data + at++) = 0xC7;
				*(uint8_t*)(code_data + at++) = 0x5;
				*(uint32_t*)(code_data + at) = hit_loc;	at += 4;
				*(uint32_t*)(code_data + at) = 1;		at += 4;
				// pop eax
				*(uint8_t*)(code_data + at++) = 0x58;
			}

			// Put overwritten bytes back (full instruction(s))
			pmwrite(handle, vcast(code_loc), code_data, at, 0);
			pmwrite(handle, vcast(code_loc + at), old_bytes, size + nop, 0);
			at += (size + nop);

			// Place our JMP back
			uint8_t* jmp_back = new uint8_t[5];
			*(uint8_t*)(jmp_back + at) = 0xE9;
			*(uint32_t*)(jmp_back + at + 1) = ((address + 5) - (code_loc + at + 5));
			pmwrite(handle, vcast(code_loc + at), jmp_back, 5, 0);
			delete jmp_back;
			delete code_data;

			edit::unprotect(address, size + nop);
			// Inject the JMP to our own code
			pmwrite(handle, vcast(address), inject, size, 0);
			for (int i = 0; i < nop; i++) {
				write(address + size + i, static_cast<uint8_t>(0x90));
			}
			edit::restore();
			delete inject;

			// wait for our masked value to be modified
			// this means something wrote to our location
			uint32_t d = 0;
			while (readb(hit_loc) != 1) {
				Sleep(1); if (d++ > 0xFFFF) break;
			} value = readui(trace_loc);

			edit::unprotect(address, size + nop);
			pmwrite(handle, vcast(address), old_bytes, size + nop, 0);
			edit::restore();
			delete old_bytes;

			VirtualFreeEx(handle, vcast(code_loc), 64, MEM_RELEASE);
			VirtualFreeEx(handle, vcast(trace_loc), 8, MEM_RELEASE);
			return value;
		}
	}
}


#endif