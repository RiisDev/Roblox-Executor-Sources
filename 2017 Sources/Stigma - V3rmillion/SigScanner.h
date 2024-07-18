#include <windows.h>
#include <stdio.h>

class SigScanner {
public:
	int ScanNoAlign(char *toFind, int bcount, int reachsz = 1024, int start = 0, int endd = 0x50000000, int align = 1);
	int ScanNoAlignW(char* toFind, int bcount, int reachsz = 1024, int start = 0, int endd = 0x50000000, int align = 1);
};