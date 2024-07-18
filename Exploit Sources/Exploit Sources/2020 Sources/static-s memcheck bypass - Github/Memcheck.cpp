#pragma once
#include <Windows.h>
#include <vector>
#include <Psapi.h>
#include <string>
#include <iostream>



#define EXTERNAL_MODE FALSE // set this to true if using in an EXE
#define DEBUG_MODE TRUE
#define ONE_PAGE 0x3FF

class Breakpoint
{
	DWORD lastProtect;
public:
	int address;
	int size;
	int value;
	bool kill;

	void start(DWORD protection = 0)
	{
		DWORD nothing, newProtect = protection;
		if (newProtect == 0)
		{
			newProtect = lastProtect;
		}
		else {
			lastProtect = newProtect;
		}

		VirtualProtect((void*)address, size, newProtect | PAGE_GUARD, &nothing);
	}

	void restore()
	{
		VirtualProtect((void*)address, size, lastProtect, &lastProtect);
	}
};


namespace memcheck
{
	HANDLE proc; // if we're using this externally

	// this is used to find the memcheck function
	// and the active running hidden checker.
	// It can only be used if this is a dll
	std::vector<Breakpoint*>Breakpoints = {};

	void writeByte(int at, BYTE value)
	{
		if (proc != nullptr)
		{
			DWORD writes;
			WriteProcessMemory(proc, (void*)at, &value, sizeof(BYTE), &writes);
		}
		else {
			*(BYTE*)(at) = value;
		}
	}

	void writeInt(int at, DWORD value)
	{
		if (proc != nullptr)
		{
			DWORD writes;
			WriteProcessMemory(proc, (void*)at, &value, sizeof(DWORD), &writes);
		}
		else {
			*(DWORD*)(at) = value;
		}
	}

	BYTE readByte(int at)
	{
		if (proc != nullptr)
		{
			BYTE buffer[1];
			DWORD reads;
			ReadProcessMemory(proc, (void*)at, buffer, sizeof(BYTE), &reads);
			return buffer[0];
		}
		else {
			return *(BYTE*)(at);
		}
	}

	PBYTE readBytes(int at, size_t count)
	{
		if (proc != nullptr)
		{
			BYTE* buffer = new BYTE[count];
			DWORD reads;
			ReadProcessMemory(proc, (void*)at, buffer, count, &reads);
			return buffer;
		}
		else {
			return reinterpret_cast<PBYTE>(at);
		}
	}

	DWORD readInt(int at)
	{
		if (proc != nullptr)
		{
			DWORD buffer = 0;

			DWORD reads;
			ReadProcessMemory(proc, (void*)at, &buffer, sizeof(DWORD), &reads);

			return buffer;
		}
		else {
			return *(DWORD*)(at);
		}
	}

	HMODULE getBaseModule(HANDLE pHandle)
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;

		if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded))
		{
			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				char szModName[MAX_PATH];
				if (GetModuleFileNameExA(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(char)))
				{
					if (std::string(szModName).find("RobloxPlayerBeta.exe") != std::string::npos)
					{
						return hMods[i];
					}
				}
			}
		}

		return nullptr;
	}

	HANDLE getHandle(const char* windowName)
	{
		HWND window = FindWindowA(NULL, windowName);
		if (!window)
		{
			return nullptr;
		}

		DWORD pid;

		GetWindowThreadProcessId(window, &pid);
		if (pid == 0)
		{
			return nullptr;
		}

		return OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	}

	void placeHook(int at, int func, size_t nops)
	{
		DWORD old;

		if (proc == nullptr)
		{
			VirtualProtect((void*)at, 5 + nops, 0x40, &old);

			DWORD rel = (func - at) - 5;
			*(BYTE*)(at) = 0xE9;
			*(DWORD*)(at + 1) = rel;

			for (int i = 5; i < 5 + nops; i++)
			{
				*(BYTE*)(at + i) = 0x90;
			}

			VirtualProtect((void*)at, 5 + nops, old, &old);
		}
		else {
			VirtualProtectEx(proc, (void*)at, 5 + nops, 0x40, &old);

			DWORD rel = (func - at) - 5;
			writeByte(at, 0xE9);
			writeInt((at + 1), rel);

			for (int i = 5; i < 5 + nops; i++)
			{
				writeByte((at + i), 0x90);
			}

			VirtualProtectEx(proc, (void*)at, 5 + nops, old, &old);
		}
	}



	LONG WINAPI debug_handler(struct _EXCEPTION_POINTERS* ExceptionInfo)
	{
		DWORD location = ExceptionInfo->ContextRecord->Eip;

		// [9D] [0F 31] [90]
		// ignore this. it's something in .vmp 
		if (*(int*)(ExceptionInfo->ContextRecord->Eip) == 0x90310F9D)
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
		{
		case EXCEPTION_GUARD_PAGE:
		{
			if (Breakpoints.size() > 0)
			{
				for (Breakpoint* bp : Breakpoints)
				{
					MEMORY_BASIC_INFORMATION mbi = { 0 };
					VirtualQuery((void*)bp->address, &mbi, 0x2C);

					// Check whether this exception has its PAGE_GUARD
					// taken off; meaning the CPU tried to access this
					// address from virtual memory
					// (this is inefficient but it works for our situation)
					if (!(mbi.Protect & PAGE_GUARD))
					{
						bp->kill = true;
						bp->value = location;
						// printf("Breakpoint got: %08X\n", bp->value);
					}
				}
			}

			return EXCEPTION_CONTINUE_EXECUTION;

			break;
		}
		case EXCEPTION_SINGLE_STEP:
		{
			if (Breakpoints.size() > 0)
			{
				int index = 0;

				for (Breakpoint* bp : Breakpoints)
				{
					if (!bp->kill)
					{
						bp->start();
					}
					else
					{
						bp->restore();
						Breakpoints.erase(Breakpoints.begin() + index, Breakpoints.end());
					}

					index++;
				}
			}

			return EXCEPTION_CONTINUE_EXECUTION;

			break;
		}
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}


	int text_start;
	int text_end;
	int vmp0;
	int vmp1;
	int modified;
	int bytes_processed;
	int spoof;

	int text_clone;
	int vmp_clone;

	int memcheck_start;
	int memcheck_hash_start;
	int memcheck_hash_end;

	std::vector<int>patches = {};


	// This function will find the current memcheck
	// and hidden checker function, by debugging the
	// ROBLOX client
	std::vector<int> getActiveChecks()
	{
		std::vector<int>checks = { 0, 0 };

		int main_check = 0;
		int silent_check = 0;

		if (silent_check == 0)
		{
			PVOID debugger = AddVectoredExceptionHandler(0, debug_handler);
			if (debugger != nullptr)
			{
				Breakpoint* bp_silent_check = new Breakpoint();
				bp_silent_check->address = text_start; // silent checker(s) start checking here
				bp_silent_check->size = 16;
				Breakpoints.push_back(bp_silent_check);

				bp_silent_check->start(PAGE_READONLY);

				Breakpoint* bp_main_check = new Breakpoint();
				bp_main_check->address = text_start + 0x1000; // memcheck starts checking here
				bp_main_check->size = 16;
				Breakpoints.push_back(bp_main_check);

				bp_main_check->start(PAGE_EXECUTE_READ);

				while (!(bp_silent_check->kill && bp_main_check->kill))
				{
					Sleep(10);
				}


				silent_check = bp_silent_check->value;
				delete bp_silent_check;

				main_check = bp_main_check->value;
				delete bp_main_check;

				RemoveVectoredExceptionHandler(debugger);
				Breakpoints.clear();
			}
		}

		// go to the real beginning of the hidden checker
		while (!(*(BYTE*)(silent_check) == 0x2B
			&& *(BYTE*)(silent_check + 2) == 0x8D)
			) {
			silent_check++;
		}

		checks[0] = main_check;
		checks[1] = silent_check;

		return checks;
	}


	// gets the address of .text start, .text end,
	// .vmp0 and .vmp1 in both dll and exe
	void loadHeaders()
	{
		int base = 0;

		if (proc == nullptr)
		{
			__asm
			{
				push eax;
				mov eax, fs: [0x30] ;
				mov eax, [eax + 8];
				mov base, eax;
				pop eax;
			}
		}
		else {
			// use psapi to get the base address
			base = (int)getBaseModule(proc);
		}

		__asm
		{
			push eax
			mov eax, base;
			mov[text_start], eax;
			push edi;
			mov edi, eax;
		findheader1:
			inc edi;
			cmp dword ptr[edi], 0x30706D76
				jne findheader1;
			add edi, 0xB;
			mov edi, [edi];
			add edi, eax;
			mov[vmp0], edi;
			mov edi, eax;
		findheader2:
			inc edi;
			cmp dword ptr[edi], 0x31706D76
				jne findheader2;
			add edi, 0xB;
			mov edi, [edi];
			add edi, eax;
			mov[vmp1], edi;
			pop edi;
			pop eax;
		}

		MEMORY_BASIC_INFORMATION mbi = { 0 };

		if (proc == nullptr)
		{
			VirtualQuery((void*)(text_start + 0x1000), &mbi, 0x2C);
		}
		else
		{
			VirtualQueryEx(proc, (void*)(text_start + 0x1000), &mbi, 0x2C);
		}

		text_end = (int)mbi.BaseAddress + mbi.RegionSize;
	}


	int clone_section(uintptr_t start, size_t* length)
	{
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		uintptr_t sectionClone;

		if (proc == nullptr)
		{
			VirtualQuery(reinterpret_cast<void*>(start), &mbi, 0x2C);

			sectionClone = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, mbi.RegionSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			if (!sectionClone)
			{
				return 0;
			}

			std::memcpy(reinterpret_cast<void*>(sectionClone), reinterpret_cast<void*>(start), mbi.RegionSize);
		}
		else
		{
			VirtualQueryEx(proc, reinterpret_cast<void*>(start), &mbi, 0x2C);

			sectionClone = reinterpret_cast<uintptr_t>(VirtualAllocEx(proc, nullptr, mbi.RegionSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			if (!sectionClone)
			{
				return 0;
			}

			BYTE* buffer = new BYTE[mbi.RegionSize];
			DWORD reads, writes;
			ReadProcessMemory(proc, (void*)start, buffer, mbi.RegionSize, &reads);
			WriteProcessMemory(proc, (void*)sectionClone, buffer, mbi.RegionSize, &writes);
		}

		*length = mbi.RegionSize;
		return sectionClone;
	}

	// Don't expect to understand my "sloppy" code at all.
	// Rather than put my coding ethics down, why don't you
	// skin-flinted lards get off your ass and learn how to
	// read sloppy code better
	UINT bypassHiddenCheck(int silentchecker)
	{
		DWORD old;
		DWORD size = 0;
		BYTE patch[512];
		BYTE* bytesRead = readBytes(silentchecker, 512);
		DWORD at = (DWORD)bytesRead;
		DWORD jmpback = silentchecker;
		DWORD rel;

		// the place we actually write to
		DWORD patch_address;

		if (proc == nullptr)
		{
			patch_address = reinterpret_cast<DWORD>(VirtualAlloc(nullptr, ONE_PAGE, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		}
		else {
			patch_address = reinterpret_cast<DWORD>(VirtualAllocEx(proc, nullptr, ONE_PAGE, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		}

		patches.push_back(patch_address);


#if DEBUG_MODE == TRUE
		printf("Patch made [%08X] for silent checker %08X\n", patch_address, silentchecker);
#endif

		// Copy the bytes up till the mov instruction
		// to our patched version of this silent checker
		while (!(*(BYTE*)at == 0x89))
		{
			*(BYTE*)(patch + size++) = *(BYTE*)(at++);
			jmpback++;
		}

		// determine the registers used in this mov instruction
		int offset = *(BYTE*)(at + 2);
		int reg2 = (*(BYTE*)(at + 1) % 64) / 8;
		at += 3;
		jmpback += 3;

		// for example if this was:
		// mov [ebp+0C],ecx
		// we can put our spoofed address into [ebp+0C]
		// because this will be accessed throughout the silent
		// checker's hash. ironically this works like a charm
		if (reg2 != 0)
		{
			*(BYTE*)(patch + size++) = 0x50; // push eax
			*(BYTE*)(patch + size++) = 0x8B; // mov eax,reg2
			*(BYTE*)(patch + size++) = 0xC0 + reg2;
			*(BYTE*)(patch + size++) = 0x50; // push eax
		}
		else
		{
			*(BYTE*)(patch + size++) = 0x50; // push eax
		}

		rel = ((int)spoof - (patch_address + size)) - 5;
		*(BYTE*)(patch + size++) = 0xE8; // call spoof
		*(DWORD*)(patch + size) = rel;
		size += sizeof(DWORD);

		*(BYTE*)(patch + size++) = 0x89; // mov [ebp+offset],eax
		*(BYTE*)(patch + size++) = 0x45;
		*(BYTE*)(patch + size++) = offset;

		if (reg2 != 0)
		{
			*(BYTE*)(patch + size++) = 0x58; // pop eax (restore value)
		}

		*(BYTE*)(patch + size++) = 0x8B; // mov reg2,[ebp+offset]
		*(BYTE*)(patch + size++) = 0x45 + (reg2 * 8);
		*(BYTE*)(patch + size++) = offset;

		// jmp back after the mov [ebp+...],... instruction
		// essentially jump back to the original code
		// after our modified version of the hash
		rel = (jmpback - (patch_address + size + 5));
		*(BYTE*)(patch + size++) = 0xE9;
		*(DWORD*)(patch + size) = rel;
		size += 4;

		if (proc)
		{
			delete[] bytesRead;
		}

		memcpy(reinterpret_cast<void*>(patch_address), &patch, size);

		placeHook(silentchecker, patch_address, 0);

		return 1;
	}

	UINT bypassMemcheckCore(int memcheck_hook_start)
	{
		DWORD hook_size = 7; // number of bytes we replace (+ nops)
		DWORD size = 0;
		BYTE patch[1024];
		BYTE* bytesRead = readBytes(memcheck_hook_start, 1024);
		DWORD at = (DWORD)bytesRead;

		// the place we actually write to
		DWORD patch_address;

		if (proc == nullptr)
		{
			patch_address = (DWORD)VirtualAlloc(0, ONE_PAGE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		}
		else {
			patch_address = (DWORD)VirtualAllocEx(proc, 0, ONE_PAGE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		}

		patches.push_back(patch_address);

		// start writing....

		// place the original bytes first
		for (int i = 0; i < hook_size; i++)
		{
			*(BYTE*)(patch + size++) = *(BYTE*)(at++);
		}

		BYTE reg_memcheck_reading;
		DWORD hash_start = 0;
		DWORD hash_jb = 0;
		DWORD rel;

		// find where the hash really starts
		while (1)
		{
			// cmp ???,???
			// jb RobloxPlayerBeta.exe+??????
			if (*(BYTE*)(at) == 0x72 && *(BYTE*)(at - 2) == 0x3B)
			{
				hash_jb = at;
				hash_start = at + 2 + *(signed char*)(at + 1);
				break;
			}
			at++;
		}

		at = (DWORD)bytesRead + hook_size;

		while (hash_start && hash_jb)
		{
			if (at == hash_start)
			{
				// update it to where the hash starts in our copy
				// since our hash will vary in size
				hash_start = size;
			}

			// beginning of the memcheck loop
			// cmp ???,???
			// jae short RobloxPlayerBeta.exe+??????
			// we'll set things up here for the rest of our code
			if (*(BYTE*)(at) == 0x3B && *(BYTE*)(at + 2) == 0x73)
			{
				// the register memcheck is using to store
				// the address in memory it's currently viewing
				reg_memcheck_reading = (*(BYTE*)(at + 1) % 0xC0) / 8;

				*(BYTE*)(patch + size++) = 0x50; // push eax
				*(BYTE*)(patch + size++) = 0x50 + reg_memcheck_reading; // push reg

				rel = ((int)spoof - (patch_address + size)) - 5;
				*(BYTE*)(patch + size++) = 0xE8; // call spoof
				*(DWORD*)(patch + size) = rel;
				size += sizeof(DWORD);

				*(BYTE*)(patch + size++) = 0x3B; // cmp eax, ???
				*(BYTE*)(patch + size++) = 0xC0 + reg_memcheck_reading;
				*(BYTE*)(patch + size++) = 0x75; // jne +6
				*(BYTE*)(patch + size++) = 0x06;

				*(BYTE*)(patch + size++) = 0x58; // pop eax

				rel = ((int)memcheck_start - (patch_address + size)) - 5;
				*(BYTE*)(patch + size++) = 0xE9; // jmp/continue in the regular memcheck
				*(DWORD*)(patch + size) = rel;
				size += sizeof(DWORD);

				*(BYTE*)(patch + size++) = 0x83; // sub esp, 4
				*(BYTE*)(patch + size++) = 0xEC;
				*(BYTE*)(patch + size++) = 0x04;
				*(BYTE*)(patch + size++) = 0x89; // mov [esp], eax
				*(BYTE*)(patch + size++) = 0x04;
				*(BYTE*)(patch + size++) = 0x24;

				at += 4;
			}

			// add reg,??
			else if (*(BYTE*)at == 0x83 && *(BYTE*)(at + 1) == 0xC0 + reg_memcheck_reading)
			{
				// DONT skip the add instruction.
				// do this first
				*(BYTE*)(patch + size++) = *(BYTE*)(at);
				*(BYTE*)(patch + size++) = *(BYTE*)(at + 1);
				*(BYTE*)(patch + size++) = *(BYTE*)(at + 2);

				// edit
				// add to our copy, [esp]
				*(BYTE*)(patch + size++) = 0x83; // add dword ptr [esp], ???
				*(BYTE*)(patch + size++) = 0x04;
				*(BYTE*)(patch + size++) = 0x24;
				*(BYTE*)(patch + size++) = *(BYTE*)(at + 2);

				at += 3;
			}

			// mov eax,[reg]
			else if (*(BYTE*)at == 0x8B && *(BYTE*)(at + 1) == 0x00 + reg_memcheck_reading)
			{
				// edit
				// make it read from our copy, [esp]
				*(BYTE*)(patch + size++) = 0x8B; // mov eax, [esp]
				*(BYTE*)(patch + size++) = 0x04;
				*(BYTE*)(patch + size++) = 0x24;

				*(BYTE*)(patch + size++) = 0x8B; // mov eax, [eax]
				*(BYTE*)(patch + size++) = 0x00;

				at += 2; // skip the instruction `mov eax,[reg]`
			}

			// lea eax,[reg + ??]
			// sub eax,[reg + ??]
			else if ((*(BYTE*)at == 0x2B && *(BYTE*)(at + 1) == 0x40 + reg_memcheck_reading)
				&& (*(BYTE*)(at - 3) == 0x8D && *(BYTE*)(at - 2) == 0x40 + reg_memcheck_reading)
				) {
				*(BYTE*)(patch + size++) = 0x50; // push eax

				*(BYTE*)(patch + size++) = 0x8B; // mov eax, [esp + ??]
				*(BYTE*)(patch + size++) = 0x44;
				*(BYTE*)(patch + size++) = 0x24;
				*(BYTE*)(patch + size++) = *(BYTE*)(at - 1);

				*(BYTE*)(patch + size++) = 0x8B; // mov eax, [eax + ??]
				*(BYTE*)(patch + size++) = 0x40;
				*(BYTE*)(patch + size++) = *(BYTE*)(at + 2);

				*(BYTE*)(patch + size++) = 0x29; // sub [esp],eax
				*(BYTE*)(patch + size++) = 0x04;
				*(BYTE*)(patch + size++) = 0x24;

				*(BYTE*)(patch + size++) = 0x58; // pop eax

				at += 3; // skip the instruction `sub eax,[reg + ??]`
			}

			else if (at == hash_jb)
			{
				rel = (hash_start - (size + 6));
				*(BYTE*)(patch + size++) = 0x0F; // jb hash_start
				*(BYTE*)(patch + size++) = 0x82;
				*(DWORD*)(patch + size) = rel;
				size += sizeof(DWORD);

				*(BYTE*)(patch + size++) = 0x83; // add esp, 4
				*(BYTE*)(patch + size++) = 0xC4;
				*(BYTE*)(patch + size++) = 0x04;

				*(BYTE*)(patch + size++) = 0x58; // pop eax

				rel = ((int)memcheck_hash_end - (patch_address + size)) - 5;
				*(BYTE*)(patch + size++) = 0xE9; // jmp memcheck_hash_end
				*(DWORD*)(patch + size) = rel;
				size += sizeof(DWORD);

				break;
			}
			else {
				// in all other scenarios, just copy the bytes over.
				*(BYTE*)(patch + size++) = *(BYTE*)(at++);
			}
		}


		memcpy(reinterpret_cast<void*>(patch_address), reinterpret_cast<void*>(patch), size);

		// Place a hook at this part:
		// (8B D4				mov edx,esp
		// 5A					pop edx
		// 8B 64) 24 08			mov esp, [esp + 08]
		// 
		placeHook(memcheck_hook_start, patch_address, hook_size - 5);

#if DEBUG_MODE == TRUE
		printf("Patched memcheck core function. [Patch: %08X]\n", patch_address);
#endif

		return 1;
	}

	// creates and loads the function for spoofing an address
	// in allocated memory
	int loadSpoofer()
	{
		BYTE spoofer[512];
		int at = (int)spoofer;

		*(BYTE*)(at++) = 0x55; // push ebp
		*(BYTE*)(at++) = 0x8B; // mov ebp, esp
		*(BYTE*)(at++) = 0xEC;
		*(BYTE*)(at++) = 0x8B; // mov eax, [ebp + 8]
		*(BYTE*)(at++) = 0x45;
		*(BYTE*)(at++) = 0x08;

		*(BYTE*)(at++) = 0x81; // cmp eax, text_end
		*(BYTE*)(at++) = 0xF8;
		*(int*)(at) = text_end;
		at += sizeof(int);

		*(BYTE*)(at++) = 0x77; // ja check2
		int rel1 = ++at;

		*(BYTE*)(at++) = 0x81; // cmp eax, text_start
		*(BYTE*)(at++) = 0xF8;
		*(int*)(at) = text_start;
		at += sizeof(int);

		*(BYTE*)(at++) = 0x72; // jb check2
		int rel2 = ++at;

		*(BYTE*)(at++) = 0x81; // sub eax, text_start
		*(BYTE*)(at++) = 0xE8;
		*(int*)(at) = text_start;
		at += sizeof(int);
		*(BYTE*)(at++) = 0x81; // add eax, text_clone
		*(BYTE*)(at++) = 0xC0;
		*(int*)(at) = text_clone;
		at += sizeof(int);

		*(BYTE*)(at++) = 0xEB; // jmp eof
		int rel3 = ++at;

		// update the relative byte-sized offsets
		*(BYTE*)(rel1 - 1) = at - rel1;
		*(BYTE*)(rel2 - 1) = at - rel2;

		*(BYTE*)(at++) = 0x81; // cmp eax, vmp1
		*(BYTE*)(at++) = 0xF8;
		*(int*)(at) = vmp1;
		at += sizeof(int);

		*(BYTE*)(at++) = 0x77; // ja check2
		rel1 = ++at;

		*(BYTE*)(at++) = 0x81; // cmp eax, vmp0
		*(BYTE*)(at++) = 0xF8;
		*(int*)(at) = vmp0;
		at += sizeof(int);

		*(BYTE*)(at++) = 0x72; // jb check2
		rel2 = ++at;

		*(BYTE*)(at++) = 0x81; // sub eax, vmp1
		*(BYTE*)(at++) = 0xE8;
		*(int*)(at) = vmp1;
		at += sizeof(int);
		*(BYTE*)(at++) = 0x81; // add eax, vmp_clone
		*(BYTE*)(at++) = 0xC0;
		*(int*)(at) = vmp_clone;
		at += sizeof(int);

		// update the relative byte-sized offsets
		*(BYTE*)(rel1 - 1) = at - rel1;
		*(BYTE*)(rel2 - 1) = at - rel2;
		*(BYTE*)(rel3 - 1) = at - rel3;

		*(BYTE*)(at++) = 0x5D; // pop ebp
		*(BYTE*)(at++) = 0xC2; // ret 4
		*(BYTE*)(at++) = 0x04;
		*(BYTE*)(at++) = 0x00;

		int spoofer_address;

		if (proc == nullptr)
		{
			spoofer_address = (int)VirtualAlloc(0, ONE_PAGE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		}
		else {
			spoofer_address = (int)VirtualAllocEx(proc, 0, ONE_PAGE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		}

		if (spoofer_address == 0)
		{
			std::cout << "[Spoofer] Failed to allocate memory\n";
			system("PAUSE");

			return 0;
		}

		memcpy(reinterpret_cast<void*>(spoofer_address), &spoofer, at - (int)spoofer);

		return spoofer_address;
	}


	UINT bypass()
	{
#if EXTERNAL_MODE == TRUE
		proc = getHandle("Roblox");
#else
		proc = nullptr;
#endif

		loadHeaders();
		/*
		std::vector<int>checks = getActiveChecks();

		printf("Main check: RobloxPlayerBeta.exe+%08X\n", checks[0] - text_start);
		printf("Active hidden check: RobloxPlayerBeta.exe+%08X\n", checks[1] - text_start);
		*/

		// Make a copy of .text
		size_t text_size;
		text_clone = clone_section(text_start + 0x1000, &text_size) - 0x1000;

		// Make a copy of .vmp
		size_t vmp_size;
		vmp_clone = clone_section(vmp0, &vmp_size);

#if DEBUG_MODE == TRUE
		printf("cloned .text section: %08X. size: %i.\n", text_clone, text_size);
		printf("cloned .vmp0 section: %08X. size: %i.\n", vmp_clone, vmp_size);
#endif

		// Make sure that the .text and .vmp0 copies take
		// place BEFORE we call loadSpoofer().
		// This allows the spoof function to use non-static
		// values which will make it work externally
		spoof = loadSpoofer();
#if DEBUG_MODE == TRUE
		printf("Created spoofing function [%08X]\n", spoof);
#endif

		// Do a simple AOB scan to find the
		// memcheck core function.
		// It looks like this:
		// BA ?? ?? ?? ?? 8B D4 5A 8B
		// 
		int start = text_start + 0x1000; // start at the process's base address
		int main_check = NULL;

		// scan in chunks of 0x100000 so that if we're
		// running externally, we use WAY less ReadProcessMemory
		// calls
		for (int chunks = 0; chunks < 10; chunks++, start += 0x100000)
		{
			// read the large chunk
			PBYTE bytes = readBytes(start, 0x100000 + 0x100);

			// start at +0x10 so we can check bytes behind what we read..
			// also allow us to read a tiny bit into the next chunk.
			// I wanted this scan to function as closely to a DLL
			// as possible, even if it's being used in an EXE
			for (int i = 0x10; i < 0x100010; i++)
			{
				int at = ((int)bytes + i); // location in the byte array that we read
				int address = (start + i); // real location/address of `at` in the process
				//                \/ we place the hook here
				// BA ?? ?? ?? ?? 8B D4 5A 8B
				if (*(BYTE*)(at - 5) == 0xBA && *(DWORD*)at == 0x8B5AD48B)
				{
					main_check = address;
					break;
				}
			}

			if (proc)
			{
				// our readBytes will return an allocated copy of
				// bytes if we're running externally
				delete[] bytes;
			}

			if (main_check)
			{
				break;
			}
		}

		memcheck_start = main_check + 7; // skip our hook's jmp
		memcheck_hash_end = memcheck_start;

		// Determine the start of the hash
		// and the absolute end of the hash.
		// This part shouldn't need to be updated
		while (!(readByte(memcheck_hash_end) == 0x72	  // jb instruction
			&& readByte(memcheck_hash_end - 2) == 0x3B  // cmp instruction right before it
			&& readByte(memcheck_hash_end + 1) > 0x7F) // jb goes somewhere behind
			) {
			memcheck_hash_end++;
		}

		memcheck_hash_start = (memcheck_hash_end + 2 + (signed char)readByte(memcheck_hash_end + 1));
		memcheck_hash_end += 2;

#if DEBUG_MODE == TRUE
		printf("memcheck hash start: RobloxPlayerBeta.exe+%08X\n", memcheck_hash_start - text_start);
		printf("memcheck hash end: RobloxPlayerBeta.exe+%08X\n", memcheck_hash_end - text_start);
#endif


		std::vector<int>hidden_checkers = {};

		// Do the same thing now..
		//
		// scan from base to base + 0x1000000
		// for all of the silent checkers
		start = text_start + 0x1000;

		// scan in chunks of 0x100000 so that if we're
		// running externally, we use WAY less ReadProcessMemory
		// calls
		for (int chunks = 0; chunks < 10; chunks++, start += 0x100000)
		{
			PBYTE bytes = readBytes(start, 0x100000 + 0x100);

			for (int i = 0x10; i < 0x100010; i++)
			{
				bool match = false;

				// Look for a movsx instruction very close to the AOB
				for (int j = 0; j < 32; j++)
				{
					int at = ((int)bytes + i + j);

					if (*(WORD*)at == 0xBE0F)
					{
						match = true;
						break;
					}
				}

				if (match)
				{
					int at = ((int)bytes + i); // location in the byte array
					int address = (start + i); // true location/address in memory

					// Now that we know there is a movsx instruction nearby...
					// Look for the actual silent checker AOB at this spot
					// 73 ?? 2B ?? 8D 
					if (*(BYTE*)at == 0x2B && *(BYTE*)(at + 2) == 0x8D && *(BYTE*)(at - 2) == 0x73)
					{
						// do a double-check so that we didn't pick up
						// the same hidden checker..
						match = true;
						for (int x : hidden_checkers)
						{
							if (address == x)
							{
								match = false;
							}
						}
						if (match)
						{
							hidden_checkers.push_back(address);
							i += 0x120; // skip the entire checker in size (just an approximate)
						}
					}
				}
			}

			if (proc)
			{
				delete[] bytes;
			}
		}

		UINT result = bypassMemcheckCore(main_check);

		for (int hidden_check : hidden_checkers)
		{
			result += bypassHiddenCheck(hidden_check);
		}

		return result;
	}

	// call this at the very end of use
	void flush()
	{
		if (proc == nullptr)
		{
			VirtualFree((void*)spoof, 0, MEM_RELEASE);
			for (int patch : patches)
			{
				VirtualFree((void*)patch, 0, MEM_RELEASE);
			}
		}
		else {
			VirtualFreeEx(proc, (void*)spoof, 0, MEM_RELEASE);
			for (int patch : patches)
			{
				VirtualFreeEx(proc, (void*)patch, 0, MEM_RELEASE);
			}
		}
	}
}

