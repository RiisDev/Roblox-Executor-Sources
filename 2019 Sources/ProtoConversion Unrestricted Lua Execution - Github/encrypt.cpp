#include "encrypt.h"

long long multiplicativeInverse(long long a, long long n)
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

	return (t < 0) ? t + n : t;
}

uint32_t rbxDaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd)
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