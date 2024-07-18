#include "xxHash32.h"

xxHash32::xxHash32()
{
	// Empty
} // end constructor

xxHash32::~xxHash32()
{
	if (_state.memory)
		delete[] _state.memory;
} // Destructor

void xxHash32::Init(const uint seed)
{
	_state.seed = seed;
	_state.v1 = seed + PRIME32_1 + PRIME32_2;
	_state.v2 = seed + PRIME32_2;
	_state.v3 = seed + 0;
	_state.v4 = seed - PRIME32_1;
	_state.total_len = 0;
	_state.memsize = 0;
	_state.memory = new BYTE[16];
} // end func Init

bool xxHash32::Update(const void *input, const size_t len)
{
	const BYTE *ptrBuffer, *ptrTemp, *ptrEnd;

	ptrBuffer = (BYTE *)input;
	_state.total_len += (uint)len;
	if (_state.memsize + len < 16) //
	{
		ptrTemp = _state.memory + _state.memsize;
		memcpy((void *)ptrTemp, ptrBuffer, len);
		_state.memsize += len;

		return true;
	} // end if

	ptrEnd = ptrBuffer + len;

	if (_state.memsize > 0) // 
	{
		ptrTemp = _state.memory + _state.memsize;
		memcpy((void *)ptrTemp, ptrBuffer, 16 - _state.memsize);

		_state.v1 = PRIME32_1 * RotateLeft32(_state.v1 + PRIME32_2 * *((uint32 *)(_state.memory)), 13);
		_state.v2 = PRIME32_1 * RotateLeft32(_state.v2 + PRIME32_2 * *((uint32 *)(_state.memory + 4)), 13);
		_state.v3 = PRIME32_1 * RotateLeft32(_state.v3 + PRIME32_2 * *((uint32 *)(_state.memory + 8)), 13);
		_state.v4 = PRIME32_1 * RotateLeft32(_state.v4 + PRIME32_2 * *((uint32 *)(_state.memory + 12)), 13);

		ptrBuffer = ptrBuffer + (16 - _state.memsize);
		_state.memsize = 0;
	} // end if

	if (ptrBuffer <= ptrEnd - 16)
	{
		uint v1 = _state.v1;
		uint v2 = _state.v2;
		uint v3 = _state.v3;
		uint v4 = _state.v4;

		const BYTE *ptrLimit = ptrEnd - 16;
		do
		{
			v1 = PRIME32_1 * RotateLeft32(v1 + PRIME32_2 * *((uint32 *)(ptrBuffer)), 13);
			v2 = PRIME32_1 * RotateLeft32(v2 + PRIME32_2 * *((uint32 *)(ptrBuffer + 4)), 13);
			v3 = PRIME32_1 * RotateLeft32(v3 + PRIME32_2 * *((uint32 *)(ptrBuffer + 8)), 13);
			v4 = PRIME32_1 * RotateLeft32(v4 + PRIME32_2 * *((uint32 *)(ptrBuffer + 12)), 13);
			ptrBuffer += 16;
		} while (ptrBuffer <= ptrLimit);

		_state.v1 = v1;
		_state.v2 = v2;
		_state.v3 = v3;
		_state.v4 = v4;
	} // end if

	if (ptrBuffer < ptrEnd)
	{
		ptrTemp = _state.memory;
		memcpy((void *)ptrTemp, ptrBuffer, ptrEnd - ptrBuffer);
		_state.memsize = ptrEnd - ptrBuffer;
	} // end if

	return true;
} // end func Update

uint xxHash32::Digest()
{
	uint h32;
	BYTE *ptrBuffer, *ptrEnd;

	if (_state.total_len >= 16)
	{
		h32 = RotateLeft32(_state.v1, 1) + RotateLeft32(_state.v2, 7) + RotateLeft32(_state.v3, 12) + RotateLeft32(_state.v4, 18);
	} // end if
	else
	{
		h32 = _state.seed + PRIME32_5;
	} // end else

	h32 += (uint32)_state.total_len;

	ptrBuffer = _state.memory;
	ptrEnd = ptrBuffer + _state.memsize;
	while (ptrBuffer + 4 <= ptrEnd)
	{
		h32 += (*((uint32 *)(ptrBuffer)) * PRIME32_3);
		h32 = RotateLeft32(h32, 17) * PRIME32_4;
		ptrBuffer += 4;
	} // end while

	while (ptrBuffer < ptrEnd)
	{
		h32 += (*((BYTE *)(ptrBuffer)) * PRIME32_5);
		h32 = RotateLeft32(h32, 11) * PRIME32_1;
		ptrBuffer++;
	} // end while

	h32 ^= h32 >> 15;
	h32 *= PRIME32_2;
	h32 ^= h32 >> 13;
	h32 *= PRIME32_3;
	h32 ^= h32 >> 16;

	return h32;
} // end func Digest

uint xxHash32::CalculateHash32(const void *input, const size_t len, const uint seed)
{
	uint h32;
	const BYTE *ptrBuffer, *ptrLimit, *ptrEnd;

	ptrBuffer = (BYTE *)input;
	ptrEnd = ptrBuffer + len;

	if (len >= 16)
	{
		ptrLimit = ptrEnd - 16;
		uint32 v1 = seed + PRIME32_1 + PRIME32_2;
		uint v2 = seed + PRIME32_2;
		uint v3 = seed;
		uint v4 = seed - PRIME32_1;

		do
		{
			v1 = PRIME32_1 * RotateLeft32(v1 + PRIME32_2 * *((uint32 *)(ptrBuffer)), 13);
			v2 = PRIME32_1 * RotateLeft32(v2 + PRIME32_2 * *((uint32 *)(ptrBuffer + 4)), 13);
			v3 = PRIME32_1 * RotateLeft32(v3 + PRIME32_2 * *((uint32 *)(ptrBuffer + 8)), 13);
			v4 = PRIME32_1 * RotateLeft32(v4 + PRIME32_2 * *((uint32 *)(ptrBuffer + 12)), 13);
			ptrBuffer += 16;
		} while (ptrBuffer <= ptrLimit);

		h32 = RotateLeft32(v1, 1) + RotateLeft32(v2, 7) + RotateLeft32(v3, 12) + RotateLeft32(v4, 18);
	} // end if
	else
	{
		h32 = seed + PRIME32_5;
	} // end else

	h32 += (uint)len;

	while (ptrBuffer + 4 <= ptrEnd)
	{
		h32 += (*((uint32 *)(ptrBuffer)) * PRIME32_3);
		h32 = RotateLeft32(h32, 17) * PRIME32_4;
		ptrBuffer += 4;
	} //end while

	while (ptrBuffer < ptrEnd)
	{
		h32 += (*((BYTE *)(ptrBuffer)) * PRIME32_5);
		h32 = RotateLeft32(h32, 11) * PRIME32_1;
		++ptrBuffer;
	} // end while

	h32 ^= (h32 >> 15);
	h32 *= PRIME32_2;
	h32 ^= (h32 >> 13);
	h32 *= PRIME32_3;
	h32 ^= (h32 >> 16);

	return h32;
} // end func CalculateHash

uint xxHash32::RotateLeft32(const uint value, const int count)
{
	return (value << count) | (value >> (32 - count));
} // end func RotateLeft

