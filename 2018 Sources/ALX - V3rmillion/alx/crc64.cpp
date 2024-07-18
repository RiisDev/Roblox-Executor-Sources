// 64-bit CRC implementation - andrewl
//
// history:
// Aug 02, 2009 - test inputs added, some explanations corrected
// Jul 08, 2009 - created

#include "crc64.h"

// poly is: x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 + x^40 + x^39 + 
//          x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 + x^24 + x^23 + x^22 + x^21 + 
//          x^19 + x^17 + x^13 + x^12 + x^10 + x^9  + x^7  + x^4  + x^1  + 1
//
// represented here with lsb = highest degree term
//
// 1100100101101100010101111001010111010111100001110000111101000010_
// ||  |  | || ||   | | ||||  | | ||| | ||||    |||    |||| |    | |
// ||  |  | || ||   | | ||||  | | ||| | ||||    |||    |||| |    | +- x^64 (implied)
// ||  |  | || ||   | | ||||  | | ||| | ||||    |||    |||| |    |
// ||  |  | || ||   | | ||||  | | ||| | ||||    |||    |||| |    +--- x^62
// ||  |  | || ||   | | ||||  | | ||| | ||||    |||    |||| +-------- x^57
// .......................................................................
// ||
// |+---------------------------------------------------------------- x^1
// +----------------------------------------------------------------- x^0 (1)
UINT64 poly = 0xC96C5795D7870F42;

// input is dividend: as 0000000000000000000000000000000000000000000000000000000000000000<8-bit byte>
// where the lsb of the 8-bit byte is the coefficient of the highest degree term (x^71) of the dividend
// so division is really for input byte * x^64

// you may wonder how 72 bits will fit in 64-bit data type... well as the shift-right occurs, 0's are supplied
// on the left (most significant) side ... when the 8 shifts are done, the right side (where the input
// byte was placed) is discarded

// when done, table[XX] (where XX is a byte) is equal to the CRC of 00 00 00 00 00 00 00 00 XX
//
UINT64 table[256];

VOID generate_table()
{
    for(INT i=0; i<256; ++i)
    {
    	UINT64 crc = i;

    	for(UINT j=0; j<8; ++j)
    	{
            // is current coefficient set?
    		if(crc & 1)
            {
                // yes, then assume it gets zero'd (by implied x^64 coefficient of dividend)
                crc >>= 1;
    
                // and add rest of the divisor
    			crc ^= poly;
            }
    		else
    		{
    			// no? then move to next coefficient
    			crc >>= 1;
            }
    	}
    
        table[i] = crc;
    }
}

// will give an example CRC calculation for input array {0xDE, 0xAD}
//
// each byte represents a group of 8 coefficients for 8 dividend terms
//
// the actual polynomial dividend is:
//
// = DE       AD       00 00 00 00 00 00 00 00 (hex)
// = 11011110 10101101 0000000000000000000...0 (binary)
//   || ||||  | | || |
//   || ||||  | | || +------------------------ x^71
//   || ||||  | | |+-------------------------- x^69
//   || ||||  | | +--------------------------- x^68
//   || ||||  | +----------------------------- x^66
//   || ||||  +------------------------------- x^64
//   || ||||  
//   || |||+---------------------------------- x^78
//   || ||+----------------------------------- x^77
//   || |+------------------------------------ x^76
//   || +------------------------------------- x^75
//   |+--------------------------------------- x^73
//   +---------------------------------------- x^72
//

// the basic idea behind how the table lookup results can be used with one
// another is that:
//
// Mod(A * x^n, P(x)) = Mod(x^n * Mod(A, P(X)), P(X))
//
// in other words, an input data shifted towards the higher degree terms
// changes the pre-computed crc of the input data by shifting it also
// the same amount towards higher degree terms (mod the polynomial)

// here is an example:
//
// 1) input:
//
//    00 00 00 00 00 00 00 00 AD DE
//          
// 2) index crc table for byte DE (really for dividend 00 00 00 00 00 00 00 00 DE)
//
//    we get A8B4AFBDC5A6ACA4
//
// 3) apply that to the input stream:
//
//    00 00 00 00 00 00 00 00 AD DE 
//       A8 B4 AF BD C5 A6 AC A4
//    -----------------------------
//    00 A8 B4 AF BD C5 A6 AC 09
//
// 4) index crc table for byte 09 (really for dividend 00 00 00 00 00 00 00 00 09)
// 
//    we get 448FCBB7FCB9E309
//
// 5) apply that to the input stream
//
//    00 A8 B4 AF BD C5 A6 AC 09
//    44 8F CB B7 FC B9 E3 09
//    --------------------------
//    44 27 7F 18 41 7C 45 A5
//
//
UINT64 calculate_crc(PBYTE stream, UINT n)
{

    UINT64 crc = 0;

    for(INT i=0; i<n; ++i)
    {
        BYTE index = stream[i] ^ crc;
        UINT64 lookup = table[index];

        crc >>= 8;
        crc ^= lookup;
    }

    return crc;
}
/*
VOID main(INT ac, PCHAR *av)
{
    generate_table();

    UINT64 result;

    printf("taking CRC64 of \"\x80\" (should be 0xC96C5795D7870F42)\n");
    result = calculate_crc((PBYTE)"\x80", 1);
    printf("result0: %016I64X\n", result);

    printf("taking CRC64 of \"\\xDE\\xAD\\xBE\\xEF\" (should be FC232C18806871AF)\n");
    result = calculate_crc((PBYTE)"\xDE\xAD\xBE\xEF", 4);
    printf("result: %016I64X\n", result);

    printf("taking CRC64 of \"99eb96dd94c88e975b585d2f28785e36\" (should be DB7AC38F63413C4E)\n");
    result = calculate_crc((PBYTE)"\x99\xEB\x96\xDD\x94\xC8\x8E\x97\x5B\x58\x5D\x2F\x28\x78\x5E\x36", 16);
    printf("result: %016I64X\n", result);

    printf("taking CRC64 of \"\\DE\\xAD\" (should be 44277F18417C45A5\n");
    result = calculate_crc((PBYTE)"\xDE\xAD", 2);
    printf("result: %016I64X\n", result);

    printf("ctrL+c to quit!\n");
    while(1);
}
*/


