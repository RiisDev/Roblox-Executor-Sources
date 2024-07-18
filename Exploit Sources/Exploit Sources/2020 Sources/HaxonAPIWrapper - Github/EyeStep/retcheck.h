#pragma once

namespace retcheck {
	union func_data {
		int address;
		int size;
	};

	std::vector<func_data> functions;

	int patch(int func_start) {
		int func_end = func_start + 3;
		while (!(*(BYTE*)func_end == 0x55 && *(USHORT*)(func_end + 1) == 0xEC8B)) {
			func_end++;
		}
		int func_size = func_end - func_start;
		int retcheck_at = func_start;

		BYTE check[8];
		while (retcheck_at < func_end) {
			memcpy(&check, reinterpret_cast<void*>(retcheck_at), 8);
			// find retcheck signature
			if (check[0] == 0x72 && check[2] == 0xA1 && check[7] == 0x8B) {
				retcheck_at = retcheck_at - func_start;
				break;
			}
			retcheck_at++;
		}

		if (retcheck_at == 0) {
			return func_start;
		}
		else {
			// if there is retcheck . . .
			func_size = func_end - func_start;
			int func = reinterpret_cast<int>(VirtualAlloc(nullptr, func_size, 0x1000 | 0x2000, 0x40));
			int jmpstart = func + retcheck_at + 2;
			int newjmpdist = 0;

			BYTE* data = new BYTE[func_size];
			memcpy(data, reinterpret_cast<void*>(func_start), func_size);

			int i = 0;
			while (i < func_size) {
				// Fix relative calls
				if (data[i] == 0xE8) {
					int oldrel = *(int*)(func_start + i + 1);
					int relfunc = (func_start + i + oldrel) + 5;

					if (relfunc % 0x10 == 0) {
						int newrel = relfunc - (func + i + 5);
						*(int*)(data + i + 1) = newrel;
						i += 4;
					}
				}
				i++;
			}

			// jump to the epilogue
			data[retcheck_at] = 0xEB;

			// write modified bytes to our new function
			memcpy(reinterpret_cast<void*>(func), data, func_size);
			delete[] data;

			// store information about this de-retcheck'd function
			func_data rdata;
			rdata.address = func;
			rdata.size = func_size;
			functions.push_back(rdata);

			return func;
		}
	}

	// clean up for all retcheck functions
	void flush() {
		for (int i = 0; i < functions.size(); i++) {
			VirtualFree(reinterpret_cast<void*>(functions.at(i).address), functions.at(i).size, MEM_RELEASE);
		}
	}
}
