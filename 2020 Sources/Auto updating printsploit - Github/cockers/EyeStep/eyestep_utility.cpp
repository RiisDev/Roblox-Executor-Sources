#include "eyestep_utility.h"
#include <fstream>


namespace EyeStep
{
	const char* convs[] =
	{
		"__cdecl",
		"__stdcall",
		"__fastcall",
		"__thiscall",
		"[auto-generated]"
	};

	namespace util
	{
		auto savedRoutines = std::vector<uint32_t>();
		DWORD useless;

		DWORD setPageProtect(uint32_t address, DWORD protect, size_t size)
		{
			DWORD old_protect;
			if (external_mode)
			{
				VirtualProtectEx(current_proc, reinterpret_cast<void*>(address), size, protect, &old_protect);
			}
			else {
				VirtualProtect(reinterpret_cast<void*>(address), size, protect, &old_protect);
			}
			return old_protect;
		}

		DWORD getPageProtect(uint32_t address)
		{
			MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (external_mode)
			{
				VirtualQueryEx(current_proc, reinterpret_cast<void*>(address), &mbi, sizeof(mbi));
			}
			else {
				VirtualQuery(reinterpret_cast<void*>(address), &mbi, sizeof(mbi));
			}
			return mbi.Protect;
		}


		void writeByte(uint32_t address, uint8_t value)
		{
			if (!external_mode)
				*reinterpret_cast<uint8_t*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				bytes[0] = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}

		void writeBytes(uint32_t address, void* value, size_t count)
		{
			if (!external_mode)
				memcpy(reinterpret_cast<void*>(address), value, count);
			else
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), value, count, &useless);
		}

		void writeShort(uint32_t address, uint16_t value)
		{
			if (!external_mode)
				*reinterpret_cast<uint16_t*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				*reinterpret_cast<uint16_t*>(bytes) = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}

		void writeInt(uint32_t address, uint32_t value)
		{
			if (!external_mode)
				*reinterpret_cast<uint32_t*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				*reinterpret_cast<uint32_t*>(bytes) = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}

		void writeFloat(uint32_t address, float value)
		{
			if (!external_mode)
				*reinterpret_cast<float*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				*reinterpret_cast<float*>(bytes) = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}

		void writeQword(uint32_t address, uint64_t value)
		{
			if (!external_mode)
				*reinterpret_cast<uint64_t*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				*reinterpret_cast<uint64_t*>(bytes) = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}

		void writeDouble(uint32_t address, double value)
		{
			if (!external_mode)
				*reinterpret_cast<double*>(address) = value;
			else
			{
				uint8_t bytes[sizeof(value)];
				*reinterpret_cast<double*>(bytes) = value;
				WriteProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(value), &useless);
			}
		}


		uint8_t readByte(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<uint8_t*>(address);

			uint8_t bytes[sizeof(uint8_t)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(uint8_t), &useless);

			return bytes[0];
		}

		uint8_t* readBytes(uint32_t address, size_t count)
		{
			uint8_t* bytes = new uint8_t[count];

			if (!external_mode)
			{
				memcpy(bytes, reinterpret_cast<void*>(address), count);
				return bytes;
			}

			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), bytes, count, &useless);
			return bytes;
		}

		uint16_t readShort(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<uint16_t*>(address);

			uint8_t bytes[sizeof(uint16_t)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(uint16_t), &useless);

			return *reinterpret_cast<uint16_t*>(bytes);
		}

		uint32_t readInt(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<uint32_t*>(address);

			uint8_t bytes[sizeof(uint32_t)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(uint32_t), &useless);

			return *reinterpret_cast<uint32_t*>(bytes);
		}

		float readFloat(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<float*>(address);

			uint8_t bytes[sizeof(float)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(float), &useless);

			return *reinterpret_cast<float*>(bytes);
		}

		uint64_t readQword(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<uint64_t*>(address);

			uint8_t bytes[sizeof(uint64_t)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(uint64_t), &useless);

			return *reinterpret_cast<uint64_t*>(bytes);
		}

		double readDouble(uint32_t address)
		{
			if (!external_mode)
				return *reinterpret_cast<double*>(address);

			uint8_t bytes[sizeof(double)];
			ReadProcessMemory(current_proc, reinterpret_cast<void*>(address), &bytes, sizeof(double), &useless);

			return *reinterpret_cast<double*>(bytes);
		}

		void freeBytes(uint8_t* bytes)
		{
			delete[] bytes;
		}

		void placeJmp(uint32_t from, uint32_t to)
		{
			uint32_t hook_size = 0;
			while (hook_size < 5)
			{
				hook_size += EyeStep::read(from + hook_size).len;
			}

			DWORD old_protect = setPageProtect(from, PAGE_EXECUTE_READWRITE);

			writeByte(from, 0xE9);
			writeInt(from + 1, (to - from) - 5);

			for (int i = 5; i < hook_size; i++)
			{
				writeByte(from + i, 0x90);
			}

			setPageProtect(from, old_protect);
		}

		void placeCall(uint32_t from, uint32_t to)
		{
			uint32_t hook_size = 0;
			while (hook_size < 5)
			{
				hook_size += EyeStep::read(from + hook_size).len;
			}

			DWORD old_protect = setPageProtect(from, PAGE_EXECUTE_READWRITE);

			writeByte(from, 0xE8);
			writeInt(from + 1, (to - from) - 5);

			for (int i = 5; i < hook_size; i++)
			{
				writeByte(from + i, 0x90);
			}

			setPageProtect(from, old_protect);
		}

		void placeTrampoline(uint32_t from, uint32_t to, size_t length)
		{
			placeJmp(from, to);
			placeJmp(to + length, from + 5);
		}

		uint32_t rebase(uint32_t address)
		{
			return reinterpret_cast<uint32_t>(base_module) + address;
		}

		uint32_t aslr(uint32_t address)
		{
			return (reinterpret_cast<uint32_t>(base_module) + address) - 0x400000;
		}

		uint32_t raslr(uint32_t address)
		{
			return (address - reinterpret_cast<uint32_t>(base_module)) + 0x400000;
		}

		bool isRel(uint32_t address)
		{
			return (getRel(address) % 16 == 0);
		}

		bool isCall(uint32_t address)
		{
			return (
				isRel(address)
				&& getRel(address) > reinterpret_cast<uint32_t>(base_module)
				&& getRel(address) < reinterpret_cast<uint32_t>(base_module) + base_module_size
				);
		}

		bool isPrologue(uint32_t address)
		{
			return (
				// Ensure that it's aligned (helps to filter it immensely)
				(address % 16 == 0)
				&&
				// Check for 3 different prologues, each with different registers
				((readByte(address) == 0x55 && readShort(address + 1) == 0xEC8B)
					|| (readByte(address) == 0x53 && readShort(address + 1) == 0xDC8B)
					|| (readByte(address) == 0x56 && readShort(address + 1) == 0xF48B))
				);
		}

		bool isEpilogue(uint32_t address)
		{
			// 1. Check for a pop ebp + retn/ret 
			// 2. Check for a leave + retn/ret
			return (
				(readShort(address - 1) == 0xC35D)
				||
				(readShort(address - 1) == 0xC25D
					&& readShort(address + 1) >= 0
					&& readShort(address + 1) % 4 == 0
				)
			) || (
				(readShort(address - 1) == 0xC3C9)
				||
				(readShort(address - 1) == 0xC2C9
					&& readShort(address + 1) >= 0
					&& readShort(address + 1) % 4 == 0
				)
			);
		}

		// determines whether the address is
		// part of the program's .text/code segment
		// by simply checking for 16 null bytes
		bool isValidCode(uint32_t address)
		{
			return !(readQword(address) == NULL && readQword(address + 8) == NULL);
		}

		uint32_t getRel(uint32_t address)
		{
			return address + 5 + static_cast<signed int>(readInt(address + 1));
		}

		uint32_t nextPrologue(uint32_t address)
		{
			uint32_t at = address;

			if (isPrologue(at))
			{
				at += 16;
			}
			else
			{
				at += (at % 16);
			}

			while (!(isPrologue(at) && isValidCode(at)))
			{
				at += 16;
			}

			return at;
		}

		uint32_t prevPrologue(uint32_t address)
		{
			uint32_t at = address;

			if (isPrologue(at))
			{
				at -= 16;
			}
			else
			{
				at -= (at % 16);
			}

			while (!(isPrologue(at) && isValidCode(at)))
			{
				at -= 16;
			}

			return at;
		}

		uint32_t getPrologue(uint32_t address)
		{
			return (isPrologue(address)) ? address : prevPrologue(address);
		}

		uint32_t getEpilogue(uint32_t address)
		{
			uint32_t next_func = util::nextPrologue(address);
			uint32_t at = next_func;

			// Get the return of this function
			while (!util::isEpilogue(at))
			{
				at--;
			}

			if (at < address)
			{
				at = next_func;

				if (util::readByte(at - 1) == 0xCC)
				{
					return at - 1;
				}
			}

			return at;
		}

		uint16_t getRetn(uint32_t address)
		{
			uint32_t epilogue = getEpilogue(address);

			if (util::readByte(epilogue) == 0xC2)
			{
				return readShort(epilogue + 1);
			}

			return 0;
		}

		uint32_t nextCall(uint32_t address, bool location, bool prologue)
		{
			uint32_t at = address;

			if (readByte(at) == 0xE8 || readByte(at) == 0xE9)
			{
				at++;
			}

			while (1)
			{
				if ((
					readByte(at) == 0xE8
					|| readByte(at) == 0xE9
					)
					&&
					isCall(at)
					) {
					bool has_prologue = true;

					// check if we need to get the prologue
					if (prologue && !isPrologue(getRel(at)))
					{
						has_prologue = false;
					}

					if (has_prologue)
					{
						if (location)
						{
							return at;
						}
						else
						{
							return getRel(at);
						}
					}
				}
				at++;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		uint32_t prevCall(uint32_t address, bool location, bool prologue)
		{
			uint32_t at = address;

			if (readByte(at) == 0xE8 || readByte(at) == 0xE9)
			{
				at--;
			}

			while (1)
			{
				if ((
					readByte(at) == 0xE8
					|| readByte(at) == 0xE9
					)
					&&
					isCall(at)
					) {
					bool has_prologue = true;

					// check if we need to get the prologue
					if (prologue && !isPrologue(getRel(at)))
					{
						has_prologue = false;
					}

					if (has_prologue)
					{
						if (location)
						{
							return at;
						}
						else
						{
							return getRel(at);
						}
					}
				}
				at--;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		uint32_t nextRef(uint32_t start, uint32_t func_search, bool prologue)
		{
			uint32_t at = start;

			while (1)
			{
				if ((
					readByte(at) == 0xE8
					|| readByte(at) == 0xE9
					)
					&&
					getRel(at) == func_search
					) {
					break;
				}

				at++;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		uint32_t prevRef(uint32_t start, uint32_t func_search, bool prologue)
		{
			uint32_t at = start;

			while (1)
			{
				if ((
					readByte(at) == 0xE8
					|| readByte(at) == 0xE9
					)
					&&
					getRel(at) == func_search
					) {
					break;
				}

				at--;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		uint32_t nextPointer(uint32_t start, uint32_t ptr_search, bool prologue)
		{
			uint32_t at = start + sizeof(uint32_t);

			while (1)
			{
				if (readInt(at) == ptr_search)
				{
					break;
				}
				at++;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		uint32_t prevPointer(uint32_t start, uint32_t ptr_search, bool prologue)
		{
			uint32_t at = start;

			while (1)
			{
				if (readInt(at) == ptr_search)
				{
					break;
				}
				at--;
			}

			return (prologue) ? getPrologue(at) : at;
		}

		std::vector<uint32_t> getCalls(uint32_t address)
		{
			auto calls = std::vector<uint32_t>();

			uint32_t at = address;
			uint32_t func_end = nextPrologue(at);

			while (at < func_end)
			{
				calls.push_back(nextCall(at));
				at = nextCall(at, true) + 5;
			}

			return calls;
		}

		std::vector<uint32_t> getPointers(uint32_t address)
		{
			auto pointers = std::vector<uint32_t>();

			uint32_t at = address;
			uint32_t func_end = nextPrologue(at);

			while (at < func_end)
			{
				auto i = EyeStep::read(at);

				if (i.source().flags & OP_DISP32 && i.source().disp32 % 4 == 0)
				{
					pointers.push_back(i.source().disp32);
				}
				else if (i.destination().flags & OP_DISP32 && i.destination().disp32 % 4 == 0)
				{
					pointers.push_back(i.destination().disp32);
				}

				at += i.len;
			}

			return pointers;
		}

		uint8_t getConvention(uint32_t func, size_t n_expected_args)
		{
			uint8_t convention = c_cdecl;

			if (n_expected_args == 0)
			{
				return convention;
			}

			uint32_t epilogue = func + 16;
			while (!isPrologue(epilogue) && isValidCode(epilogue))
			{
				epilogue += 16;
			}

			uint32_t args = 0;
			uint32_t func_start = func;
			uint32_t func_end = epilogue;

			while (!isEpilogue(epilogue))
			{
				epilogue--;
			}

			if (readByte(epilogue) == 0xC2)
			{
				convention = c_stdcall;
			}
			else
			{
				convention = c_cdecl;
			}

			// search for the highest ebp offset, which will 
			// indicate the number of args that were pushed
			// on the stack, rather than placed in ECX/EDX
			uint32_t at = func_start;
			while (at < func_end)
			{
				auto i = EyeStep::read(at);

				if (i.flags & OP_SRC_DEST || i.flags & OP_SINGLE)
				{
					auto src = i.source();
					auto dest = i.destination();

					if (src.flags & OP_R32)
					{
						if (dest.flags & OP_R32)
						{
							if (dest.flags & OP_IMM8 && dest.reg[0] == R32_EBP && dest.imm8 != 4 && dest.imm8 < 0x7F)
							{
								// printf("arg offset: %02X\n", dest.imm8);

								if (dest.imm8 > args)
								{
									args = dest.imm8;
								}
							}
						}
						else if (src.flags & OP_IMM8 && src.reg[0] == R32_EBP && src.imm8 != 4 && src.imm8 < 0x7F)
						{
							// printf("arg offset: %02X\n", src.imm8);

							if (src.imm8 > args)
							{
								args = src.imm8;
							}
						}
					}
				}

				at += i.len;
			}

			// no pushed args were used, but we know there
			// is a 1 or 2 EBP arg difference, so it is either
			// a fastcall or a thiscall
			if (args == 0)
			{
				switch (n_expected_args)
				{
				case 1:
					return c_thiscall;
					break;
				case 2:
					return c_fastcall;
					break;
				}
			}

			args -= 8;
			args = (args / 4) + 1;

			if (args == n_expected_args - 1)
			{
				convention = c_thiscall;
			}
			else if (args == n_expected_args - 2)
			{
				convention = c_fastcall;
			}

			return convention;
		}


		uint8_t getConvention(uint32_t func)
		{
			function_info info;
			info.analyze(func);
			return info.convention;
		}

		// When used in a DLL:
		// If the convention is a cdecl, it will not create a new routine.
		// otherwise, it will create a routine to call the function as if it were a cdecl.
		// This means every typedef in your DLL can be a __cdecl.
		// No need for updating the conventions, if you use this on the function addresses.
		//
		// When used in an EXE: 
		// This function will allocate a new portion of memory
		// which invokes the function. However, this newly allocated
		// function can be used as a __cdecl.
		// Same as above, but it will allocate memory for __cdecl  functions also.
		// 
		uint32_t createRoutine(uint32_t function, uint8_t n_args)
		{
			uint8_t convention = getConvention(function, n_args);

			if (!external_mode && convention == c_cdecl)
			{
				return function;
			}

			uint8_t convert_stdcall = FALSE; // personal switch :)
			uint32_t func = function;
			uint32_t size = 0;
			uint8_t data[128];

			auto new_func = reinterpret_cast<uint32_t>(VirtualAlloc(nullptr, 128, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
			if (new_func == NULL)
			{
				throw std::exception("Error while allocating memory");
				return func;
			}

			data[size++] = 0x55; // push ebp

			data[size++] = 0x8B; // mov ebp,esp
			data[size++] = 0xEC;

			if (convention == c_cdecl)
			{
				for (int i = (n_args * 4) + 8; i > 8; i -= 4)
				{
					data[size++] = 0xFF; // push [ebp+??]
					data[size++] = 0x75;
					data[size++] = i - 4;
				}
				data[size++] = 0xE8; // call func
				*reinterpret_cast<uint32_t*>(data + size) = func - (new_func + size + 4);
				size += 4;
				data[size++] = 0x83; // add esp, (n_args * 4)
				data[size++] = 0xC4;
				data[size++] = n_args * 4;
			}
			else if (convention == c_stdcall)
			{
				for (int i = (n_args * 4) + 8; i > 8; i -= 4)
				{
					data[size++] = 0xFF; // push [ebp+??]
					data[size++] = 0x75;
					data[size++] = i - 4;
				}

				data[size++] = 0xE8; // call func
				*reinterpret_cast<uint32_t*>(data + size) = func - (new_func + size + 4);
				size += 4;
			}
			else if (convention == c_thiscall)
			{
				data[size++] = 0x51; // push ecx

				for (int i = n_args; i > 1; i--)
				{
					data[size++] = 0xFF; // push [ebp+??]
					data[size++] = 0x75;
					data[size++] = (i + 1) * 4;
				}

				data[size++] = 0x8B; // mov ecx,[ebp+08]
				data[size++] = 0x4D;
				data[size++] = 0x08;

				data[size++] = 0xE8; // call func
				*reinterpret_cast<uint32_t*>(data + size) = func - (new_func + size + 4);
				size += 4;

				data[size++] = 0x59; // pop ecx
			}
			else if (convention == c_fastcall)
			{
				data[size++] = 0x51; // push ecx
				data[size++] = 0x52; // push edx

				for (int i = n_args; i > 2; i--)
				{
					data[size++] = 0xFF; // push [ebp+??]
					data[size++] = 0x75;
					data[size++] = (i + 1) * 4;
				}

				data[size++] = 0x8B; // mov ecx,[ebp+08]
				data[size++] = 0x4D;
				data[size++] = 0x08;

				data[size++] = 0x8B; // mov edx,[ebp+0C]
				data[size++] = 0x55;
				data[size++] = 0x0C;

				data[size++] = 0xE8; // call func
				*reinterpret_cast<uint32_t*>(data + size) = func - (new_func + size + 4);
				size += 4;

				data[size++] = 0x59; // pop ecx
				data[size++] = 0x5A; // pop edx
			}

			if (!convert_stdcall)
			{
				data[size++] = 0x5D; // pop ebp
				data[size++] = 0xC3; // retn
			}
			else {
				data[size++] = 0xC2; // ret xx
				data[size++] = n_args * 4;
				data[size++] = 0x00;
			}

			memcpy_s(reinterpret_cast<void*>(new_func), size, &data, size);

			savedRoutines.push_back(new_func);

			return new_func;
		}


		// not finished (uniqueness coming soon)
		std::string generate_sig(uint32_t func, size_t instructions)
		{
			std::string signature = "";

			uint32_t at = func;
			for (int j = 0; j < instructions; j++)
			{
				auto i = EyeStep::read(at);

				for (int c = 0; c < i.len; c++)
				{
					if (c == 0)
					{
						signature += to_str(i.bytes[c]);
					}
					else {
						signature += "??";
					}
				}

				at += i.len;
			}

			return signature;
		}

		std::string getAnalysis(uint32_t func)
		{
			function_info info;
			info.analyze(func);
			std::string analysis = "";
			analysis += info.psuedocode;
			return analysis;
		}

		void disableFunction(uint32_t func)
		{
			DWORD old_protect = setPageProtect(func, PAGE_EXECUTE_READWRITE);
			if (isPrologue(func))
			{
				uint16_t ret = getRetn(func);
				if (ret != 0)
				{
					writeByte(func + 3, 0xC2);
					writeShort(func + 4, ret);
				}
				else
				{
					writeByte(func + 3, 0xC3);
				}
			}
			else {
				writeByte(func, 0xC3);
			}
			setPageProtect(func, old_protect);
		}

		std::vector<uint32_t> debug_r32(uint32_t address, uint8_t r32, uint32_t offset, size_t count)
		{
			auto results = std::vector<uint32_t>();

			uint32_t new_func = NULL;
			uint32_t vars = NULL;
			uint32_t signal = NULL;
			uint8_t* old_bytes;
			size_t hook_size = 0;

			// Figure out the instructions that may be overwritten
			while (hook_size < 5)
			{
				hook_size += EyeStep::read(address + hook_size).len;
			}

			old_bytes = readBytes(address, hook_size);

			// Allocate memory internally or remotely
			if (external_mode)
				new_func = reinterpret_cast<uint32_t>(VirtualAllocEx(current_proc, nullptr, 256, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			else
				new_func = reinterpret_cast<uint32_t>(VirtualAlloc(nullptr, 256, MEM_COMMIT, PAGE_EXECUTE_READWRITE));


			// Places we can store values -- "variables"
			vars = new_func + 128;
			signal = new_func + 124;

			uint8_t data[128];
			uint8_t* at = data;

			//
			// Begin writing ASM to the function
			// 

			for (int i = 0; i < hook_size; i++)
			{
				// Place the original bytes first (any that were overwritten)
				*at++ = old_bytes[i];
			}

			*at++ = 0x60; // pushad
			*at++ = 0x50; // push eax

			for (size_t i = 0; i < count; i++)
			{
				*at++ = 0x8B; // mov

				if (offset + (count * 4) < 0x80)
				{
					// Byte-sized offset
					*at++ = 0x40 + r32; //  eax,[r32 + ??]
					*at++ = offset + (i * 4);
				}
				else {
					// DWORD-sized offset
					*at++ = 0x80 + r32; // eax,[r32 + ????????]
					*reinterpret_cast<uint32_t*>(at) = offset + (i * 4);
					at += sizeof(uint32_t);
				}

				*at++ = 0xA3; // mov [vars + x], eax
				*reinterpret_cast<uint32_t*>(at) = vars + (i * 4);
				at += sizeof(uint32_t);
			}

			// update the signal location, meaning
			// the debug is finished
			*at++ = 0xC7; // mov [signal],00000001
			*at++ = 0x05;
			*reinterpret_cast<uint32_t*>(at) = signal;
			at += sizeof(uint32_t);
			*reinterpret_cast<uint32_t*>(at) = 0x00000001;
			at += sizeof(uint32_t);

			*at++ = 0x58; // pop eax
			*at++ = 0x61; // popad

			//
			// Function is finished, let's trampoline to it and back
			// 

			writeBytes(new_func, data, at - data);
			placeTrampoline(address, new_func, at - data);

			//
			// Wait for the hook to be executed, and 
			// our signal value to be set to 1 (anything non-null)
			//

			while (readInt(signal) == NULL)
			{
				Sleep(10);
			}

			//
			// Dump the register offsets/values into our table
			//
			for (int i = 0; i < count; i++)
			{
				results.push_back(readInt(vars + i * 4));
			}

			// Restore protection
			DWORD old_protect = setPageProtect(address, PAGE_EXECUTE_READWRITE);
			writeBytes(address, old_bytes, hook_size);
			setPageProtect(address, old_protect);

			delete[] old_bytes;

			// Clean up in either mode
			if (external_mode)
				VirtualFreeEx(current_proc, reinterpret_cast<void*>(new_func), NULL, MEM_RELEASE);
			else
				VirtualFree(reinterpret_cast<void*>(new_func), NULL, MEM_RELEASE);

			return results;
		}

	}



	function_info::function_info()
	{
		start_address = 0;
		convention = c_auto;
		args = std::vector<function_arg>();
		return_bits = 0;
		stack_cleanup = 0;
		function_size = 0;
		psuedocode[0] = '\0';
	}


	void function_info::analyze(uint32_t func)
	{
		uint32_t func_end = util::getEpilogue(func);

		if (util::readByte(func_end) == 0xC3)
		{
			stack_cleanup = 0;
			func_end += 1;
		}
		else if (util::readByte(func_end) == 0xC2)
		{
			stack_cleanup = util::readShort(func_end + 1);
			func_end += 3;
		}

		// this is the absolute function size
		start_address = func;
		function_size = func_end - func;

		// Identify compiler-generated "strlen" function in memory...
		// we can identify when the compiler generates this
		// simply by checking for a byte signature.
		// This is used to identify const char* args.
		auto inlined_strlen = std::vector<uint32_t>();

		for (int i = 0; i < function_size; i++)
		{
			uint8_t* bytes_strlen = util::readBytes(start_address + i, 8);

			if (bytes_strlen[0] == 0x8A // mov al,[???]
				&& bytes_strlen[2] >= 0x40 && bytes_strlen[2] < 0x48 // inc ???
				&& bytes_strlen[3] == 0x84 && bytes_strlen[4] == 0xC0 // test al,al
				&& bytes_strlen[5] == 0x75 // jnz
				) {
				inlined_strlen.push_back(start_address + i);
				i += 8;
			}

			delete[] bytes_strlen;
		}

		uint8_t ecx_set = FALSE;
		uint8_t edx_set = FALSE;
		uint32_t at = func;

		auto return_value = EyeStep::operand();
		auto ebp_args = std::vector<uint32_t>();


		while (at < func_end)
		{
			auto i = EyeStep::read(at);

			//printf("%s\n", i.data);

			auto src = i.source();
			auto dest = i.destination();

			std::string opcode = "";
			opcode += i.data;

			// not set yet?
			if (convention == c_auto)
			{
				// set the calling convention to the
				// function's return
				if (opcode.find("retn") != std::string::npos)
				{
					stack_cleanup = 0;
					convention = c_cdecl;
				}
				else if (opcode.find("ret ") != std::string::npos)
				{
					stack_cleanup = util::readShort(i.address + 1);
					convention = c_stdcall;
				}
			}

			// does the source operand use a register?
			if (src.reg.size())
			{
				// mov [ebp+08], ???
				// mov [ebp+0C], ???
				// . . .
				if (src.flags & OP_R32 && src.reg[0] == R32_EBP && src.imm8 >= 8 && src.imm8 < 0x40)
				{
					uint8_t found = FALSE;

					for (uint32_t arg : ebp_args)
						if (src.imm8 == arg)
							found = TRUE;

					if (!found)
					{
						// Append args from EBP
						ebp_args.push_back(src.imm8);
						args.push_back({ src.imm8, 32, false, at });
					}
				}

				// Figure out what the very last thing is
				// that gets placed into EAX ( the return value )
				// mov eax, ???
				// or eax, ???
				if (src.reg[0] == R32_EAX)
				{
					if (opcode.find("mov ") != std::string::npos
					 || opcode.find("or ") != std::string::npos
						) {
						return_value = dest;
					}
				} else if (src.reg[0] == R32_ECX)
				{
					ecx_set = TRUE;
				}
				else if (src.reg[0] == R32_EDX)
				{
					convention = c_auto; // let it be determined by the function return
					edx_set = TRUE;
					break;
				}

				// does the destination operand use a register?
				if (dest.reg.size())
				{
					// mov ???, [ebp+08]
					// mov ???, [ebp+0C]
					// . . .
					if (dest.flags & OP_R32 && dest.reg[0] == R32_EBP && dest.imm8 >= 8 && dest.imm8 < 0x40)
					{
						uint8_t found = FALSE;

						for (uint32_t arg : ebp_args)
							if (dest.imm8 == arg)
								found = TRUE;

						if (!found)
						{
							// Append args from EBP
							ebp_args.push_back(dest.imm8);
							args.push_back({ dest.imm8, 32, false, at });
						}
					}

					// instruction does not use ecx or edx in both operands.
					if ((src.reg[0] == R32_EDX && dest.reg[0] == R32_EDX)
					 || (src.reg[0] == R32_ECX && dest.reg[0] == R32_ECX)
					){
						// an instruction was used with `ecx,ecx`
						// or `edx,edx`.
						// We may have to do something about this here...
						/*
						if (opcode.find("test ") != std::string::npos
							&& src.reg[0] == R32_EDX
							&& dest.reg[0] == R32_EDX
							&& !edx_set
							) {
							convention = c_fastcall;
						}
						else if (opcode.find("test ") != std::string::npos
							&& src.reg[0] == R32_ECX
							&& dest.reg[0] == R32_ECX
							&& !ecx_set
							) {
							convention = c_thiscall;
						}
						*/
					}
					else 
					{
						// EDX was used in the destination operand, before
						// it was allocated. It must be a fastcall.
						if (dest.reg[0] == R32_EDX && !edx_set)
						{
							convention = c_fastcall;
							break;
						}
						// ECX was used in the destination operand, before
						// it was allocated. It must be a thiscall.
						else if (dest.reg[0] == R32_ECX && !ecx_set)
						{
							if (convention != c_fastcall)
							{
								convention = c_thiscall;
							}
						}
					}
				}
				else {
					// SINGLE OPERAND INSTRUCTION
					// Check if it pops ECX or pops EDX
					if (opcode.find("pop ") != std::string::npos)
					{
						if (src.reg[0] == R32_ECX)
						{
							ecx_set = FALSE;
						}
						else if (src.reg[0] == R32_EDX)
						{
							edx_set = FALSE;
						}
					}
					// Check if it pushes ECX or pushes EDX
					else if (opcode.find("push ") != std::string::npos)
					{
						if (src.reg[0] == R32_ECX)
						{
							ecx_set = TRUE; // ECX HAS been pushed, meaning it cannot be a thiscall
						}
						else if (src.reg[0] == R32_EDX)
						{
							edx_set = TRUE; // EDX HAS been pushed meaning it cannot be a fastcall
						}
					}
				}
			}

			at += i.len;
		}

		// append the args from ECX/EDX to the args
		// identified from offsets of EBP.
		if (convention == c_thiscall)
			args.push_back({ 0, 32, false, 0 });
		else if (convention == c_fastcall)
		{
			args.push_back({ 0, 32, false, 0 });
			args.push_back({ 0, 32, false, 0 });
		}
		else if (convention == c_auto)
		{
			// set the default calling convention if it could not be identified...
			if (!stack_cleanup)
				convention = c_cdecl;
			else
				convention = c_stdcall;
		}

		// adjust args...check for args that were used
		// with a compiler-generated `strlen()` and identify
		// them as a const char* arg.
		if (inlined_strlen.size())
		{
			for (uint32_t r : inlined_strlen)
			{
				for (int i = args.size() - 1; i >= 0; i--)
				{
					if (args[i].location < r)
					{
						args[i].isCharPointer = true;
						break;
					}
				}
			}
		}

		// Start writing to the psuedocode
		// (we can start with the return value)
		if (return_value.flags & OP_DISP8 || return_value.flags & OP_R8)
		{
			strcat(psuedocode, "bool "); // chances are it's a bool value
			return_bits = sizeof(uint8_t);
		}
		else if (return_value.flags & OP_DISP16 || return_value.flags & OP_R16)
		{
			strcat(psuedocode, "short ");
			return_bits = sizeof(uint16_t);
		}
		else if (return_value.flags & OP_DISP32 || return_value.flags & OP_R32)
		{
			strcat(psuedocode, "int ");
			return_bits = sizeof(uint32_t);
		}
		else {
			// To-do... Analyse when ESP is altered
			// with push and pop,
			// add esp and sub esp.
			// The left over amount (if it's 4 bytes)
			// will tell us whether the function returns an
			// int or not.
			strcat(psuedocode, "int ");
			return_bits = sizeof(uint32_t);
		}

		char c_addr[16];
		sprintf(c_addr, "%08X", start_address);

		strcat(psuedocode, convs[convention]);
		strcat(psuedocode, " ");
		strcat(psuedocode, c_addr);
		strcat(psuedocode, "(");

		for (int i = 0; i < args.size(); i++)
		{
			char c[4];
			sprintf(c, "%i", i + 1);

			if (args[i].isCharPointer)
			{
				strcat(psuedocode, "const char*");
			}
			else if (args[i].bits == 8)
			{
				strcat(psuedocode, "byte");
			}
			else if (args[i].bits == 16)
			{
				strcat(psuedocode, "short");
			}
			else if (args[i].bits == 32)
			{
				strcat(psuedocode, "int");
			}

			strcat(psuedocode, " a");
			strcat(psuedocode, c);

			if (i < args.size() - 1)
			{
				strcat(psuedocode, ", ");
			}
		}

		strcat(psuedocode, ")");

		//system("PAUSE");
	}


	namespace io
	{
		std::string readFile(const char* filename)
		{
			std::string output = "";
			std::ifstream stream;

			stream.open(filename);
			stream.seekg(0, std::ios::end);

			size_t length = stream.tellg();
			stream.seekg(0, std::ios::beg);

			char* buffer = new char[length];
			stream.read(buffer, length);
			output += buffer;

			stream.close();
			delete[] buffer;

			return output;
		}

		void writeFile(const char* filename, void* data, size_t size)
		{
			char* char_array = reinterpret_cast<char*>(data);
			std::ofstream outfile(filename, std::ofstream::trunc); // truncate
			if (outfile.is_open())
			{
				for (size_t i = 0; i < size; i++)
				{
					outfile << char_array[i];
				}
				outfile.close();
			}
		}

		void appendFile(const char* filename, void* data, size_t size)
		{
			char* char_array = reinterpret_cast<char*>(data);
			std::ofstream outfile(filename, std::ios_base::app); // append instead of overwrite
			if (outfile.is_open())
			{
				for (size_t i = 0; i < size; i++)
				{
					outfile << char_array[i];
				}
				outfile.close();
			}
		}

		inline bool checkFileExists(const std::string& name) 
		{
			std::ifstream f(name.c_str());
			return f.good();
		}


		std::string readRawHttp(const char* sUrl)
		{
			// Ask JAY#7902 to work on this when he's
			// not busy on proto conversion!!
			return "not done yet :-(";
		}
	}


	namespace scanner
	{
		bool compare_bytes(const uint8_t* location, const uint8_t* aob, const char* mask)
		{
			for (; *mask; ++aob, ++mask, ++location)
			{
				if (*mask == '.' && *location != *aob)
				{
					return 0;
				}
			}
			return 1;
		}

		// aob			- array of byte string to scan
		// code			- true if restricting the scan to the '.text' section
		// align		- defaults to 1. set to 4 if going by 4 bytes (extremely fast string scans)
		// endresult	- result (#) to stop the scan at
		// checks		- additional checks at the offsets from the location of the aob, during the scan; 
		//				  these can narrow the results, make the scan faster, and increase accuracy
		// 
		scan_results scan(const char* aob, bool code, int align, int endresult, std::vector<scan_check>checks)
		{
			auto results = scan_results();

			MEMORY_BASIC_INFORMATION mbi = { 0 };

			size_t scan_size = 0;
			DWORD bytes_read;
			uint8_t* bytes = nullptr;
			uint32_t region_base = 0;
			uint32_t protection = 0;
			uint32_t start = 0;
			uint32_t end = 0;

			uint8_t* pattern = new uint8_t[128];
			ZeroMemory(pattern, 128);

			char* mask = new char[128];
			ZeroMemory(mask, 128);

			// reinterprets the AOB string as a string mask
			for (int i = 0, j = 0; i < lstrlenA(aob); i++)
			{
				if (aob[i] == 0x20)
				{
					continue;
				}

				char x[2];
				x[0] = aob[i];
				x[1] = aob[1 + i++];

				if (x[0] == '?' && x[1] == '?')
				{
					pattern[j] = 0;
					mask[j++] = '?';
				}
				else
				{
					// convert 2 chars to byte
					int id = 0;
					int n = 0;

				convert:        if (x[id] > 0x60) n = x[id] - 0x57; // n = A-F (10-16)
				else if (x[id] > 0x40) n = x[id] - 0x37; // n = a-f (10-16)
				else if (x[id] >= 0x30) n = x[id] - 0x30; // number chars

				if (id != 0)
					pattern[j] += n;
				else
				{
					id++, pattern[j] += (n * 16);
					goto convert;
				}

				mask[j++] = '.';
				}
			}

			if (!code)
			{
				SYSTEM_INFO siSysInfo;
				GetSystemInfo(&siSysInfo);

				// Restrict the scan to virtual memory
				start = reinterpret_cast<uint32_t>(base_module) + base_module_size;
				end = reinterpret_cast<uint32_t>(siSysInfo.lpMaximumApplicationAddress);
			}
			else
			{
				start = reinterpret_cast<uint32_t>(base_module);
				end = reinterpret_cast<uint32_t>(base_module) + base_module_size;
			}

			while (start < end)
			{
				if (!external_mode)
				{
					VirtualQuery(reinterpret_cast<void*>(start), &mbi, sizeof(mbi));
				}
				else {
					VirtualQueryEx(current_proc, reinterpret_cast<void*>(start), &mbi, sizeof(mbi));
				}

				if (mbi.BaseAddress)
				{
					// Make sure the memory is committed, matches our protection, and isn't PAGE_GUARD.
					if ((mbi.State & MEM_COMMIT) && !(mbi.Protect & PAGE_NOACCESS) && !(mbi.Protect & PAGE_NOCACHE) && !(mbi.Protect & PAGE_GUARD))
					{
						bytes = util::readBytes(start, mbi.RegionSize);

						// Scan all the memory in the region.
						for (size_t i = 0; i < mbi.RegionSize; i += align)
						{
							if (compare_bytes(bytes + i, pattern, mask))
							{
								uint32_t result = start + i;

								if (checks.size() == 0)
									results.push_back(result);
								else
								{
									// Go through a series of extra checks,
									// make sure all are passed before it's a valid result
									size_t checks_pass = 0;

									for (scan_check check : checks)
									{
										switch (check.type)
										{
										case byte_equal:
											if (*reinterpret_cast<uint8_t*>(bytes + i + check.offset) == reinterpret_cast<uint8_t>(check.value)) checks_pass++;
											break;
										case word_equal:
											if (*reinterpret_cast<uint16_t*>(bytes + i + check.offset) == reinterpret_cast<uint16_t>(check.value)) checks_pass++;
											break;
										case int_equal:
											if (*reinterpret_cast<uint32_t*>(bytes + i + check.offset) == reinterpret_cast<uint32_t>(check.value)) checks_pass++;
											break;
										case byte_notequal:
											if (*reinterpret_cast<uint8_t*>(bytes + i + check.offset) != reinterpret_cast<uint8_t>(check.value)) checks_pass++;
											break;
										case word_notequal:
											if (*reinterpret_cast<uint16_t*>(bytes + i + check.offset) != reinterpret_cast<uint16_t>(check.value)) checks_pass++;
											break;
										case int_notequal:
											if (*reinterpret_cast<uint32_t*>(bytes + i + check.offset) != reinterpret_cast<uint32_t>(check.value)) checks_pass++;
											break;
										}
									}

									if (checks_pass == checks.size())
									{
										results.push_back(result);
									}
								}
								if (endresult > 0 && results.size() >= endresult)
								{
									break;
								}
							}
						}

						delete[] bytes;
					}

					// Move onto the next region of memory.
					start += mbi.RegionSize;
				}
			}

			delete[] mask;
			delete[] pattern;

			return results;
		}

		// converts a string like "Test" to an AOB string "54 65 73 74"
		std::string aobstring(const char* str)
		{
			std::string aob = "";

			for (int i = 0; i < lstrlenA(str); i++)
			{
				auto b_char = static_cast<uint8_t>(str[i]);
				aob += to_str(b_char);

				if (i < lstrlenA(str) - 1)
				{
					aob += 0x20;
				}
			}

			return aob;
		}

		// converts a result to an AOB string
		// for example 0x110CBED0 --> "D0 BE 0C 11"
		std::string ptrstring(uint32_t ptr)
		{
			std::string aob = "";

			uint8_t bytes[4];
			memcpy(&bytes, &ptr, 4);

			aob += to_str(bytes[0]);
			aob += to_str(bytes[1]);
			aob += to_str(bytes[2]);
			aob += to_str(bytes[3]);

			return aob;
		}

		scan_results scan_xrefs(const char* str, int nresult)
		{
			scan_results result_list = scan(aobstring(str).c_str(), true, 4, nresult);

			if (result_list.size() > 0)
			{
				return scan(ptrstring(result_list.back()).c_str());
			}
			else {
				throw std::exception("No results found for string");
				return scan_results();
			}
		}

		scan_results scan_xrefs(uint32_t func)
		{
			auto results = scan_results();

			MEMORY_BASIC_INFORMATION mbi = { 0 };

			uint32_t start = reinterpret_cast<uint32_t>(base_module);
			uint32_t end = reinterpret_cast<uint32_t>(base_module) + base_module_size;

			while (start < end)
			{
				if (!external_mode)
				{
					VirtualQuery(reinterpret_cast<void*>(start), &mbi, sizeof(mbi));
				}
				else {
					VirtualQueryEx(current_proc, reinterpret_cast<void*>(start), &mbi, sizeof(mbi));
				}

				if (mbi.Protect == PAGE_EXECUTE_READ)
				{
					uint8_t* bytes = util::readBytes(start, mbi.RegionSize);

					for (size_t at = 0; at < mbi.RegionSize; at++)
					{
						if (bytes[at] == 0xE8 || bytes[at] == 0xE9)
						{
							if (util::getRel(start + at) == func)
							{
								results.push_back(start + at);
							}
						}
					}

					delete[] bytes;
				}

				start += mbi.RegionSize;
			}

			return results;
		}
	}
}
