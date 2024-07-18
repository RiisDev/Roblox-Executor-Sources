// 64-bit CRC implementation - andrewl
//
// history:
// Aug 02, 2009 - test inputs added, some explanations corrected
// Jul 08, 2009 - created

#include <windows.h>
#include <stdio.h>

extern UINT64 poly;// = 0xC96C5795D7870F42;

extern UINT64 table[256];

VOID generate_table();

UINT64 calculate_crc(PBYTE stream, UINT n);


