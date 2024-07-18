#ifndef XXHASH32_H
#define XXHASH32_H

#include <string>
#include <vector>
#include <windows.h>
typedef unsigned char BYTE;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned long uint32;

class xxHash32
{
private:
	struct XXH_State
	{
		ulong total_len;
		uint seed;
		uint v1;
		uint v2;
		uint v3;
		uint v4;
		uint memsize;
		BYTE *memory;
	}; // struct XXH_State

public:
	xxHash32();
	~xxHash32();
	void Init(const uint seed = 0);
	bool Update(const void *buf, const size_t size);
	uint Digest();
	uint CalculateHash32(const void *buf, const size_t len, const uint seed = 0);
	uint CalculateHash32(const std::string buf, const uint seed = 0)
	{
		std::vector<BYTE> bytes(buf.begin(), buf.end());
		bytes.push_back('\0');
		BYTE *c = &bytes[0];

		return CalculateHash32((void*)c, buf.size(), seed);
	} // end func CalculateHash

protected:
	XXH_State _state;

private:
	static const uint PRIME32_1 = 2654435761U;
	static const uint PRIME32_2 = 2246822519U;
	static const uint PRIME32_3 = 3266489917U;
	static const uint PRIME32_4 = 668265263U;
	static const uint PRIME32_5 = 374761393U;

	uint RotateLeft32(const uint, const int);

}; // end class xxHash32


#endif // XXHASH32_H
