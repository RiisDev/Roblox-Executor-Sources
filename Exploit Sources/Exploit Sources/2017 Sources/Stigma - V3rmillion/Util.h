#pragma once

#include <windows.h>
#include <stdio.h>

int CallFunc(int func_addr, int nargs, int *arg_buffer, int *rtcheck);
void printERR();
void printOK();
void SetColor(int forgc);
void xor_encrypt(char *key, char *string, int n);