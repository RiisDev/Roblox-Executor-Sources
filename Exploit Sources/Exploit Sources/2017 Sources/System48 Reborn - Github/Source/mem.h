#pragma once
#include <Windows.h>

void hook(void* func, void* newFunc, int len);
void writebytes(void* address, byte* bytes, int len);
void writebyte(void* address, byte byte);