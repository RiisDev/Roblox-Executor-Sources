#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "Retcheck.h"

namespace SignatureFind
{
	namespace Addresses
	{
		uintptr_t RobloxBase(uintptr_t address)
		{
			return (address - 0x400000 + reinterpret_cast<DWORD>(GetModuleHandle(0)));
		}
	}
}

using spawnFn = void(__cdecl*)(uintptr_t rL);
spawnFn r_spawn = (spawnFn)SignatureFind::Addresses::RobloxBase(0x728d20);
using sandboxFn = void(__cdecl*)(uintptr_t, int[], int[]);
sandboxFn Sandbox = (sandboxFn)SignatureFind::Addresses::RobloxBase(0x7289a0);

namespace zx3
{
	class RbxLua
	{
	public:
		static DWORD luaF_newproto(uintptr_t rL)
		{
			return ((DWORD(__cdecl*)(uintptr_t rL))SignatureFind::Addresses::RobloxBase(0x7C2900))(rL);
		}
		static const char* luaS_newlstr(uintptr_t rL, const char* str, size_t Size)
		{
			using luaS_newlstrFn = const char* (__stdcall*)(uintptr_t rL, const char* str, size_t length);
			luaS_newlstrFn r_newlstr = reinterpret_cast<luaS_newlstrFn>(SignatureFind::Addresses::RobloxBase(0x7c4610));
			return r_newlstr(rL, str, Size);
		}

		static int lua_newthread(uintptr_t rL)
		{
			return ((int(__cdecl*)(uintptr_t))unprotect(SignatureFind::Addresses::RobloxBase(0x7c9590)))(rL);
		}

        DWORD* __cdecl luaF_newLClosure(int a1, int a2, int a3, int a4)
        {
            int v4; // ebx
            int v5; // edi
            int v6; // esi
            DWORD* v7; // eax
            int v8; // edx
            int v9; // ecx
            char v10; // al
            int v11; // eax

            v4 = a2;
            v5 = a1 + 24 + *(DWORD*)(a1 + 24);
            v6 = 24 * (a2 + 1);
            v7 = (DWORD*)(*(int(__cdecl**)(int, DWORD, DWORD, DWORD, int, signed int))(v5 + 16))(
                a1,
                *(DWORD*)(v5 + 20),
                0,
                0,
                v6,
                3);
            v8 = (int)v7;
         
            *(DWORD*)(v5 + 88) += v6;
            v9 = a1 + 24 + *(DWORD*)(a1 + 24);
            *v7 = *(DWORD*)(v9 + 56);
            v10 = *(BYTE*)(v9 + 24);
            *(DWORD*)(v9 + 56) = v8;
            *(BYTE*)(v8 + 5) = v10 & 3;
            *(DWORD*)(v8 + 16) = a3;
            *(BYTE*)(v8 + 4) = 7;
            *(BYTE*)(v8 + 6) = 0;
            *(BYTE*)(v8 + 7) = a2;
            *(BYTE*)(v8 + 8) = *(BYTE*)(a4 + 83);
            *(DWORD*)(v8 + 20) = v8 + a4 + 20;
            if (a2 > 0)
            {
                v11 = v8 + 40;
                do
                {
                    *(DWORD*)(v11 - 8) = 0;
                    v11 += 24;
                    *(DWORD*)(v11 - 24) = 0;
                    --v4;
                } while (v4);
            }
            return (DWORD*)v8;
        }
	};
}

namespace zx3
{
    namespace Obfuscation
    {
        void ObfuscateProto(uintptr_t a1, uintptr_t m)
        {
            *reinterpret_cast<uintptr_t*>(a1) = m ^ a1;
       }
        namespace GlobalState
        {
            void DeObfuscateGL(uintptr_t a1)
            {
                reinterpret_cast<uintptr_t*>(a1) - *reinterpret_cast<uintptr_t*>(a1);
            }
        }
    }
}