#include "Util.h"

__declspec(naked) int CallFunc(int func_addr, int nargs, int *arg_buffer, int *retss) {
	__asm {
		push ebp
		mov ebp, esp

		pushad

		mov edx, [nargs]

		mov eax, [arg_buffer]
		xor ecx, ecx

		push real_return

		lupe :
		mov ebx, [eax + ecx * 4]
			push ebx
			inc ecx
			cmp ecx, edx
			jne lupe

			mov eax, [retss]
			mov eax, [eax + edx * 4]
			push eax
			mov eax, [func_addr]
			jmp eax

			real_return :
		mov[ebp - 0x50], eax
			popad
			mov eax, [ebp - 0x50]
			pop ebp
			ret
	}
}

void xor_encrypt(char *key, char *string, int n) {
	int i;
	int keyLength = strlen(key);
	for (i = 0; i < n; i++) {
		string[i] = string[i] ^ key[i%keyLength];
	}
}

void SetColor(int forgc) {
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		wColor = (csbi.wAttributes & 0xF0) + (forgc & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}

void printOK() {
	SetColor(2);
	// printf("Done\n");
	SetColor(7);
}