/*
 * MIT License
 *
 * Copyright (c) 2017 Rokas Kupstys
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <Windows.h>
#include <WinInet.h>
#include <iostream>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define HOOKER_ERROR            FALSE
#define HOOKER_SUCCESS          TRUE
#define HOOKER_MEM_R            (1 << 0)
#define HOOKER_MEM_W            (1 << 1)
#define HOOKER_MEM_X            (1 << 2)
#define HOOKER_MEM_RW           (HOOKER_MEM_R|HOOKER_MEM_W)
#define HOOKER_MEM_RX           (HOOKER_MEM_R|HOOKER_MEM_X)
#define HOOKER_MEM_RWX          (HOOKER_MEM_R|HOOKER_MEM_W|HOOKER_MEM_X)
/// Memory protection flags are platform specific (not a combination of above flags) and should not be converted.
#define HOOKER_MEM_PLATFORM     (1 << 31)

/// Write a call instruction (5 bytes on x86/64).
#define HOOKER_HOOK_CALL        (1 << 0)
/// Write a jump instruction (5 bytes on x86/64).
#define HOOKER_HOOK_JMP         (1 << 1)
/// Use fat jump (16 for call and 14 bytes for jmp on x64). Has no effect on x86.
#define HOOKER_HOOK_FAT         (1 << 2)

#if __cplusplus
extern "C" {
#endif

/// Call any address with arbitrary amount of arguments. Returns value of specified type.
/// \param returnType of return value.
/// \param address of a call.
#define hooker_call(returnType, address, ...) (returnType(*)(...))(address)(__VA_ARGS__);
/// Call any address with arbitrary amount of arguments. Does not return anything.
/// \param address of a call.
#define hooker_callv(address, ...)            (void(*)(...))(address)(__VA_ARGS__);

/// Change protection of memory range.
/// \param p memory address.
/// \param size of memory at address p.
/// \param protection a combination of HOOKER_MEM_* flags.
/// \param original_protection on supported platforms will be set to current memory protection mode. May be null. If not null - always initialize to a best-guess current protection flags value, because on some platforms (like linux) this variable will not be set.
bool hooker_mem_protect(void* p, size_t size, size_t protection, size_t* original_protection);
/// Get mnemonic size of current platform.
size_t hooker_get_mnemonic_size(void* address, size_t min_size);

/// Hotpatch a call.
void* hooker_hotpatch(void* location, void* new_proc);
/// Unhotpatch a call.
bool hooker_unhotpatch(void* location);

/// Writes a jump or call hook from `address` to `new_proc`.
/// \param address a pointer where hook should be written
/// \param new_proc a pointer where hook should point to.
/// \param flags any of HOOKER_HOOK_* flags. They may not be combined.
/// \param nops of bytes to nop after hook instruction. Specify -1 to autocalculate.
/// \returns null on failure or non-null on success.
bool hooker_write_instruction(void* address, void* new_proc, size_t flags, size_t nops);

/// Writes a 5 byte jump with E9 opcode. Difference between pointers is limited to 32bit.
/// \param address a pointer where hook should be written
/// \param new_proc a pointer where hook should point to.
/// \returns null on failure or non-null on success.
bool hooker_write_jmp(void* address, void* new_proc);
/// Writes a 5 byte call with E8 opcode. Difference between pointers is limited to 32bit.
/// \param address a pointer where hook should be written
/// \param new_proc a pointer where hook should point to.
/// \returns null on failure or non-null on success.
bool hooker_write_call(void* address, void* new_proc);

/// Redirect call to custom proc.
/// \param address a start of original call. Warning: It should not contain any relatively-addressed instructions like calls or jumps.
/// \param new_proc a proc that will be called instead of original one.
/// \returns pointer, calling which will invoke original proc. It is user's responsibility to call original code when necessary.
void* hooker_hook(void* address, void* new_proc, size_t flags);

/// Unhook a hook created by hooker_hook(.., .., HOOKER_HOOK_REDIRECT, ..).
/// \param address where hook was written to.
/// \param original result of hooker_hook() call.
void hooker_unhook(void* address, void* original);

/// Return address in object's vmt which is pointing to specified method.
/// \param object is a pointer to a c++ object.
/// \param method is a pointer to a c++ object method.
size_t* hooker_get_vmt_address(void* object, void* method);

/// Find a first occurrence of memory pattern.
/// \param start a pointer to beginning of memory range.
/// \param size a size of memory range. If size is 0 then entire memory space will be searched. If pattern does not exist this will likely result in a crash. Negative size will search backwards.
/// \param pattern a array of bytes to search for.
/// \param pattern_len a length of pattern array.
/// \param wildcard byte in the pattern array. It must be of same size as indicated by `pattern_len`.
const void* hooker_find_pattern(const void* start, int size, const uint8_t* pattern, size_t pattern_len, uint8_t wildcard);

/// Find a first occurrence of memory pattern.
/// \param start a pointer to beginning of memory range.
/// \param size a size of memory range. If size is 0 then entire memory space will be searched. If pattern does not exist this will likely result in a crash. Negative size will search backwards.
/// \param pattern a array of bytes to search for.
/// \param pattern_len a length of pattern array.
/// \param wildcard array where values may be one of: 0? = 1, ?0 = 2, ?? = 3.
const void* hooker_find_pattern_ex(const void* start, int size, const uint8_t* pattern, size_t pattern_len, const uint8_t* wildcard);

/// Find a first occurrence of specified data.
/// \param start a pointer to beginning of memory range.
/// \param size a size of memory range. If size is 0 then entire memory space will be searched. If pattern does not exist this will likely result in a crash. Negative size will search backwards.
/// \param data to search.
const void* hooker_find(const void* start, int size, const uint8_t* pattern, int pattern_len);

/// Find instruction "lea REG, DATA"
/// \param start a pointer to beginning of memory range.
/// \param size a size of memory range. If size is 0 then entire memory space will be searched. If pattern does not exist this will likely result in a crash. Negative size will search backwards.
/// \param data pointer that is loaded by lea instruction.
/// \param data_len length of data.
/// \returns a pointer to lea instruction or 0.
const void* hooker_find_lea_data_64(const void* start, int size, const void* data, int data_len);

/// Fill memory with nops (0x90 opcode).
/// \param start of the memory address.
/// \param size of the memory that will be filled.
/// \returns HOOKER_SUCCESS or HOOKER_ERROR.
bool hooker_nop(void* start, size_t size);

/// Write bytes to specified memory address.
/// \param start of the memory address.
/// \param data to be written.
/// \param size of data.
bool hooker_write(void* start, void* data, size_t size);

/// Searches for symbol in specified library. On Windows LoadLibrary() will be called if its not loaded yet, otherwise GetModuleHandle() will be used.
/// On linux dlopen(RTLD_NODELETE) and dlclose() will always be called.
/// \param lib_name string with dynamic library name.
/// \param sym_name string with exported symbol name.
/// \returns pointer to resolved dynamic symbol.
void* hooker_dlsym(const char* lib_name, const char* sym_name);

#if _WIN32
/// Replaces entry in import address table of specified module.
/// \param mod_name string with name of module whose import table is to be modified.
/// \param imp_mod_name string with name of module which module specified in `mod_name` imports.
/// \param imp_proc_name string with name of symbol imported from module specified in `imp_mod_name`.
/// \param new_proc a pointer that should replace old entry in import address table.
/// \returns original value that was in import address table or null pointer on failure.
void* hooker_hook_iat(const char* mod_name, const char* imp_mod_name, const char* imp_proc_name, void* new_proc);
#endif

// Define following macro in a single translation unit in order to use library without building it.
#ifdef HOOKER_IMPLEMENTATION
#    include "hooker.c"
#endif  // HOOKER_IMPLEMENTATION

#if __cplusplus
};
#endif
