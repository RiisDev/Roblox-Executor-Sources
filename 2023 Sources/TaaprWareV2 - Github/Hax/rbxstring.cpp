#include "rbxstring.h"

bool constexpr inline is_typable(unsigned char character) {
	if (character >= 'a' && character <= 'z') {
		return true;
	}
	if (character >= 'A' && character <= 'Z') {
		return true;
	}
	if (character >= '0' && character <= '9') {
		return true;
	}
	if (character == ' ' || character == '_') {
		return true;
	}
	return false;
}
bool constexpr inline uint_is_typable(UINT uint_address) {
	bool typable = true;
	for (UINT8 i = 0; i < 4; i++) {
		UCHAR byte = *reinterpret_cast<UCHAR*>(uint_address + i);
		if (!is_typable(byte)) {
			if (typable && byte == 0x00) {
				return true;
			}
			typable = false;
		}
	}
	/*if (!typable) {
		volatile UINT value_at_ptr = 0;
		printf("Testing...");
		if (*reinterpret_cast<UINT**>(uint_address) != NULL) {
			value_at_ptr = ***reinterpret_cast<UINT***>(uint_address);
			printf("%X is a ptr to a valid ptr pointing to %X\n", uint_address, value_at_ptr);
			return false;
		} else {
			printf("%X is a ptr to an invalid ptr\n", uint_address);
			return true;
		}
	}*/
	return typable;
}

std::string rbx_string_to_string(rbx_string rbx) {
	UINT address = reinterpret_cast<UINT>(&rbx);
	if (uint_is_typable(address)) {
		// string
		return rbx.contents;
	}
	else {
		if (uint_is_typable(reinterpret_cast<UINT>(&((*reinterpret_cast<rbx_string**>(address))->contents)))) {
			// &string
			return std::string((*reinterpret_cast<rbx_string**>(address))->contents);
		}
		else {
			// &&string
			return std::string((**reinterpret_cast<rbx_string***>(address))->contents);
		}
	}
}