#include "encryption.h"

UINT get_encrypted_ptr(encryption enc, UINT address_of_ptr) {
	switch (enc) {
	case ptrenc_none:
		return *reinterpret_cast<UINT*>(*reinterpret_cast<UINT*>(address_of_ptr));
	case ptrenc_add:
		return address_of_ptr + *reinterpret_cast<UINT*>(address_of_ptr);
	case ptrenc_xor:
		return address_of_ptr ^ *reinterpret_cast<UINT*>(address_of_ptr);
	default:
		printf(XorString("[ERROR] Invalid encryption %d when getting\n"), enc);
		return 0;
	}
}
void set_encrypted_ptr(encryption enc, UINT address_of_ptr, UINT value) {
	switch (enc) {
	case ptrenc_none:
		*reinterpret_cast<UINT*>(address_of_ptr) = value;
		break;
	case ptrenc_add:
		*reinterpret_cast<UINT*>(address_of_ptr) = value - address_of_ptr;
		break;
	case ptrenc_xor:
		*reinterpret_cast<UINT*>(address_of_ptr) = value ^ address_of_ptr;
		break;
	default:
		printf(XorString("[ERROR] Invalid encryption %d when setting\n"), enc);
		break;
	}
}