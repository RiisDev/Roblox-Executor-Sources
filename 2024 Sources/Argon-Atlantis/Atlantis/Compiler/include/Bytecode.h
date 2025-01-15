// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

// clang-format off

// This header contains the bytecode definition for Luau interpreter
// Creating the bytecode is outside the scope of this file and is handled by bytecode builder (BytecodeBuilder.h) and bytecode compiler (Compiler.h)
// Note that ALL enums declared in this file are order-sensitive since the values are baked into bytecode that needs to be processed by legacy clients.

// # Bytecode definitions
// Bytecode instructions are using "word code" - each instruction is one or many 32-bit words.
// The first word in the instruction is always the instruction header, and *must* contain the opcode (enum below) in the least significant byte.
//
// Instruction word can be encoded using one of the following encodings:
//     ABC - least-significant byte for the opcode, followed by three bytes, A, B and C; each byte declares a register index, small index into some other table or an unsigned integral value
//     AD - least-significant byte for the opcode, followed by A byte, followed by D half-word (16-bit integer). D is a signed integer that commonly specifies constant table index or jump offset
//     E - least-significant byte for the opcode, followed by E (24-bit integer). E is a signed integer that commonly specifies a jump offset
//
// Instruction word is sometimes followed by one extra word, indicated as AUX - this is just a 32-bit word and is decoded according to the specification for each opcode.
// For each opcode the encoding is *static* - that is, based on the opcode you know a-priory how large the instruction is, with the exception of NEWCLOSURE

// # Bytecode indices
// Bytecode instructions commonly refer to integer values that define offsets or indices for various entities. For each type, there's a maximum encodable value.
// Note that in some cases, the compiler will set a lower limit than the maximum encodable value is to prevent fragile code into bumping against the limits whenever we change the compilation details.
// Additionally, in some specific instructions such as ANDK, the limit on the encoded value is smaller; this means that if a value is larger, a different instruction must be selected.
//
// Registers: 0-254. Registers refer to the values on the function's stack frame, including arguments.
// Upvalues: 0-199. Upvalues refer to the values stored in the closure object.
// Constants: 0-2^23-1. Constants are stored in a table allocated with each proto; to allow for future bytecode tweaks the encodable value is limited to 23 bits.
// Closures: 0-2^15-1. Closures are created from child protos via a child index; the limit is for the number of closures immediately referenced in each function.
// Jumps: -2^23..2^23. Jump offsets are specified in word increments, so jumping over an instruction may sometimes require an offset of 2 or more. Note that for jump instructions with AUX, the AUX word is included as part of the jump offset.

// # Bytecode versions
// Bytecode serialized format embeds a version number, that dictates both the serialized form as well as the allowed instructions. As long as the bytecode version falls into supported
// range (indicated by LBC_BYTECODE_MIN / LBC_BYTECODE_MAX) and was produced by Luau compiler, it should load and execute correctly.
//
// Note that Luau runtime doesn't provide indefinite bytecode compatibility: support for older versions gets removed over time. As such, bytecode isn't a durable storage format and it's expected
// that Luau users can recompile bytecode from source on Luau version upgrades if necessary.

// # Bytecode version history
//
// Note: due to limitations of the versioning scheme, some bytecode blobs that carry version 2 are using features from version 3. Starting from version 3, version should be sufficient to indicate bytecode compatibility.
//
// Version 1: Baseline version for the open-source release. Supported until 0.521.
// Version 2: Adds Proto::linedefined. Supported until 0.544.
// Version 3: Adds FORGPREP/JUMPXEQK* and enhances AUX encoding for FORGLOOP. Removes FORGLOOP_NEXT/INEXT and JUMPIFEQK/JUMPIFNOTEQK. Currently supported.
// Version 4: Adds Proto::flags, typeinfo, and floor division opcodes IDIV/IDIVK. Currently supported.
// Version 5: Adds SUBRK/DIVRK and vector constants. Currently supported.
// Version 6: Adds FASTCALL3. Currently supported.

// # Bytecode type information history
// Version 1: (from bytecode version 4) Type information for function signature. Currently supported.
// Version 2: (from bytecode version 4) Type information for arguments, upvalues, locals and some temporaries. Currently supported.

// Bytecode opcode, part of the instruction header
enum LuauOpcode
{
    LOP_NOP = 0,
    LOP_BREAK = 227,
    LOP_LOADNIL = 198,
    LOP_LOADB = 169,
    LOP_LOADN = 140,
    LOP_LOADK = 111,
    LOP_MOVE = 82,
    LOP_GETGLOBAL = 53,
    LOP_SETGLOBAL = 24,
    LOP_GETUPVAL = 251,
    LOP_SETUPVAL = 222,
    LOP_CLOSEUPVALS = 193,
    LOP_GETIMPORT = 164,
    LOP_GETTABLE = 135,
    LOP_SETTABLE = 106,
    LOP_GETTABLEKS = 77,
    LOP_SETTABLEKS = 48,
    LOP_GETTABLEN = 19,
    LOP_SETTABLEN = 246,
    LOP_NEWCLOSURE = 217,
    LOP_NAMECALL = 188,
    LOP_CALL = 159,
    LOP_RETURN = 130,
    LOP_JUMP = 101,
    LOP_JUMPBACK = 72,
    LOP_JUMPIF = 43,
    LOP_JUMPIFNOT = 14,
    LOP_JUMPIFEQ = 241,
    LOP_JUMPIFLE = 212,
    LOP_JUMPIFLT = 183,
    LOP_JUMPIFNOTEQ = 154,
    LOP_JUMPIFNOTLE = 125,
    LOP_JUMPIFNOTLT = 96,
    LOP_ADD = 67,
    LOP_SUB = 38,
    LOP_MUL = 9,
    LOP_DIV = 236,
    LOP_MOD = 207,
    LOP_POW = 178,
    LOP_ADDK = 149,
    LOP_SUBK = 120,
    LOP_MULK = 91,
    LOP_DIVK = 62,
    LOP_MODK = 33,
    LOP_POWK = 4,
    LOP_AND = 231,
    LOP_OR = 202,
    LOP_ANDK = 173,
    LOP_ORK = 144,
    LOP_CONCAT = 115,
    LOP_NOT = 86,
    LOP_MINUS = 57,
    LOP_LENGTH = 28,
    LOP_NEWTABLE = 255,
    LOP_DUPTABLE = 226,
    LOP_SETLIST = 197,
    LOP_FORNPREP = 168,
    LOP_FORNLOOP = 139,
    LOP_FORGLOOP = 110,
    LOP_FORGPREP_INEXT = 81,
    LOP_FASTCALL3 = 52,
    LOP_FORGPREP_NEXT = 23,
    LOP_NATIVECALL = 250,
    LOP_GETVARARGS = 221,
    LOP_DUPCLOSURE = 192,
    LOP_PREPVARARGS = 163,
    LOP_LOADKX = 134,
    LOP_JUMPX = 105,
    LOP_FASTCALL = 76,
    LOP_COVERAGE = 47,
    LOP_CAPTURE = 18,
    LOP_SUBRK = 245,
    LOP_DIVRK = 216,
    LOP_FASTCALL1 = 187,
    LOP_FASTCALL2 = 158,
    LOP_FASTCALL2K = 129,
    LOP_FORGPREP = 100,
    LOP_JUMPXEQKNIL = 71,
    LOP_JUMPXEQKB = 42,
    LOP_JUMPXEQKN = 13,
    LOP_JUMPXEQKS = 240,
    LOP_IDIV = 211,
    LOP_IDIVK = 182,
    LOP__COUNT = 211
};

// Bytecode instruction header: it's always a 32-bit integer, with low byte (first byte in little endian) containing the opcode
// Some instruction types require more data and have more 32-bit integers following the header
#define LUAU_INSN_OP(insn) ((insn) & 0xff)

// ABC encoding: three 8-bit values, containing registers or small numbers
#define LUAU_INSN_A(insn) (((insn) >> 8) & 0xff)
#define LUAU_INSN_B(insn) (((insn) >> 16) & 0xff)
#define LUAU_INSN_C(insn) (((insn) >> 24) & 0xff)

// AD encoding: one 8-bit value, one signed 16-bit value
#define LUAU_INSN_D(insn) (int32_t(insn) >> 16)

// E encoding: one signed 24-bit value
#define LUAU_INSN_E(insn) (int32_t(insn) >> 8)

// Bytecode tags, used internally for bytecode encoded as a string
enum LuauBytecodeTag
{
    // Bytecode version; runtime supports [MIN, MAX], compiler emits TARGET by default but may emit a higher version when flags are enabled
    LBC_VERSION_MIN = 3,
    LBC_VERSION_MAX = 6,
    LBC_VERSION_TARGET = 5,
    // Type encoding version
    LBC_TYPE_VERSION_MIN = 1,
    LBC_TYPE_VERSION_MAX = 3,
    LBC_TYPE_VERSION_TARGET = 3,
    // Types of constant table entries
    LBC_CONSTANT_NIL = 0,
    LBC_CONSTANT_BOOLEAN,
    LBC_CONSTANT_NUMBER,
    LBC_CONSTANT_STRING,
    LBC_CONSTANT_IMPORT,
    LBC_CONSTANT_TABLE,
    LBC_CONSTANT_CLOSURE,
    LBC_CONSTANT_VECTOR,
};

// Type table tags
enum LuauBytecodeType
{
    LBC_TYPE_NIL = 0,
    LBC_TYPE_BOOLEAN,
    LBC_TYPE_NUMBER,
    LBC_TYPE_STRING,
    LBC_TYPE_TABLE,
    LBC_TYPE_FUNCTION,
    LBC_TYPE_THREAD,
    LBC_TYPE_USERDATA,
    LBC_TYPE_VECTOR,
    LBC_TYPE_BUFFER,

    LBC_TYPE_ANY = 15,

    LBC_TYPE_TAGGED_USERDATA_BASE = 64,
    LBC_TYPE_TAGGED_USERDATA_END = 64 + 32,

    LBC_TYPE_OPTIONAL_BIT = 1 << 7,

    LBC_TYPE_INVALID = 256,
};

// Builtin function ids, used in LOP_FASTCALL
enum LuauBuiltinFunction
{
    LBF_NONE = 0,

    // assert()
    LBF_ASSERT,

    // math.
    LBF_MATH_ABS,
    LBF_MATH_ACOS,
    LBF_MATH_ASIN,
    LBF_MATH_ATAN2,
    LBF_MATH_ATAN,
    LBF_MATH_CEIL,
    LBF_MATH_COSH,
    LBF_MATH_COS,
    LBF_MATH_DEG,
    LBF_MATH_EXP,
    LBF_MATH_FLOOR,
    LBF_MATH_FMOD,
    LBF_MATH_FREXP,
    LBF_MATH_LDEXP,
    LBF_MATH_LOG10,
    LBF_MATH_LOG,
    LBF_MATH_MAX,
    LBF_MATH_MIN,
    LBF_MATH_MODF,
    LBF_MATH_POW,
    LBF_MATH_RAD,
    LBF_MATH_SINH,
    LBF_MATH_SIN,
    LBF_MATH_SQRT,
    LBF_MATH_TANH,
    LBF_MATH_TAN,

    // bit32.
    LBF_BIT32_ARSHIFT,
    LBF_BIT32_BAND,
    LBF_BIT32_BNOT,
    LBF_BIT32_BOR,
    LBF_BIT32_BXOR,
    LBF_BIT32_BTEST,
    LBF_BIT32_EXTRACT,
    LBF_BIT32_LROTATE,
    LBF_BIT32_LSHIFT,
    LBF_BIT32_REPLACE,
    LBF_BIT32_RROTATE,
    LBF_BIT32_RSHIFT,

    // type()
    LBF_TYPE,

    // string.
    LBF_STRING_BYTE,
    LBF_STRING_CHAR,
    LBF_STRING_LEN,

    // typeof()
    LBF_TYPEOF,

    // string.
    LBF_STRING_SUB,

    // math.
    LBF_MATH_CLAMP,
    LBF_MATH_SIGN,
    LBF_MATH_ROUND,

    // raw*
    LBF_RAWSET,
    LBF_RAWGET,
    LBF_RAWEQUAL,

    // table.
    LBF_TABLE_INSERT,
    LBF_TABLE_UNPACK,

    // vector ctor
    LBF_VECTOR,

    // bit32.count
    LBF_BIT32_COUNTLZ,
    LBF_BIT32_COUNTRZ,

    // select(_, ...)
    LBF_SELECT_VARARG,

    // rawlen
    LBF_RAWLEN,

    // bit32.extract(_, k, k)
    LBF_BIT32_EXTRACTK,

    // get/setmetatable
    LBF_GETMETATABLE,
    LBF_SETMETATABLE,

    // tonumber/tostring
    LBF_TONUMBER,
    LBF_TOSTRING,

    // bit32.byteswap(n)
    LBF_BIT32_BYTESWAP,

    // buffer.
    LBF_BUFFER_READI8,
    LBF_BUFFER_READU8,
    LBF_BUFFER_WRITEU8,
    LBF_BUFFER_READI16,
    LBF_BUFFER_READU16,
    LBF_BUFFER_WRITEU16,
    LBF_BUFFER_READI32,
    LBF_BUFFER_READU32,
    LBF_BUFFER_WRITEU32,
    LBF_BUFFER_READF32,
    LBF_BUFFER_WRITEF32,
    LBF_BUFFER_READF64,
    LBF_BUFFER_WRITEF64,
};

// Capture type, used in LOP_CAPTURE
enum LuauCaptureType
{
    LCT_VAL = 0,
    LCT_REF,
    LCT_UPVAL,
};

// Proto flag bitmask, stored in Proto::flags
enum LuauProtoFlag
{
    // used to tag main proto for modules with --!native
    LPF_NATIVE_MODULE = 1 << 0,
    // used to tag individual protos as not profitable to compile natively
    LPF_NATIVE_COLD = 1 << 1,
    // used to tag main proto for modules that have at least one function with native attribute
    LPF_NATIVE_FUNCTION = 1 << 2,
};
