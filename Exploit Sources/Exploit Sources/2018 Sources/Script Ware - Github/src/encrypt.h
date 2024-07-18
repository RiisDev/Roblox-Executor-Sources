#pragma once
#include <iostream>
#include <cstdint>

uint32_t dax_encode_op(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd);
int call_enc(int a1, int a2);
int closure_enc(int a1, int a2);

unsigned int setupval_encryption(unsigned int instr, unsigned int pc);
unsigned int jump_encryption(unsigned int instr, unsigned int pc);