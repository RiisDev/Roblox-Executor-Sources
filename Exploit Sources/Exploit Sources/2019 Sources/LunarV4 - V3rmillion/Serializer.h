#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include "DenseHash.h"
#include "RbxOpEncoder.hpp"

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lobject.h"
#include "Lua/lstring.h"
#include "Lua/lfunc.h"
#include "Lua/ldo.h"
#include "Lua/lopcodes.h"
#include "Lua/lstring.h"
#include "LZ4\lz4.h"
#include "LZ4\lz4hc.h"
#include "XXHash\xxhash.h"
}

static const char kBytecodeMagic[] = "RSB1";
static const unsigned int kBytecodeHashSeed = 42;
static const unsigned int kBytecodeHashMultiplier = 41;

enum BytecodeConstantType
{
	Constant_Nil,
	Constant_False,
	Constant_True,
	Constant_Number,
	Constant_String,
};

Proto* ok;

class LuaSerializer
{
public:
	static uintptr_t returnHash(lua_State* L, unsigned int modKey) {
		return generateHash(ok, modKey);
	}

	static uintptr_t rerturnups()
	{
		return ok->nups;
	}

	static uintptr_t pickone()
	{
		return ok->numparams;
	}

	static Proto* okhand(Proto* p) {

		ok = p;
		return p;
	}



	static std::string serialize(lua_State* L, const std::string& source, RbxOpEncoder encode, unsigned int ckey, unsigned int modKey)
	{
		LoadS ls = { source.c_str(), source.size() };

		std::ostringstream ss;
		int error = lua_load(L, getS, &ls, "=");

		writeByte(ss, error ? 0 : 0xF0);
		if (error != 0)
		{
			const char* err = lua_tostring(L, -1);
			size_t length = err ? strlen(err) : 0;
			writeCompressedInt(ss, length);
			ss.write(err, length);
		}
		else
		{
			const LClosure* cl = static_cast<const LClosure*>(lua_topointer(L, -1));
			okhand(cl->p);
			writeRoot(ss, cl->p, encode, ckey, modKey);
		}

		lua_pop(L, 1);

		std::string ssdata = ss.str();

		// compress
		int dataSize = ssdata.size();
		int maxSize = LZ4_compressBound(dataSize);
		std::vector<char> compressed(maxSize);
		int compressedSize = LZ4_compress(ssdata.c_str(), &compressed[0], dataSize);

		// prepend magic
		std::string result = kBytecodeMagic;
		result.append(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
		result.append(&compressed[0], compressedSize);

		// compute xxhash
		unsigned int hash = XXH32(&result[0], result.size(), kBytecodeHashSeed);
		// xor data with the hash
		unsigned char hashbytes[4];
		memcpy(hashbytes, &hash, sizeof(hash));
		for (size_t i = 0; i < result.size(); ++i)
			result[i] ^= hashbytes[i % 4] + i * kBytecodeHashMultiplier;
		return result;
	}

private:
	static void writeRoot(std::ostringstream & ss, Proto * p, RbxOpEncoder encode, unsigned int ckey, unsigned int modKey)
	{
		DenseHashMap<const TString*, unsigned int> strings;
		unsigned int streamStart = static_cast<unsigned int>(ss.tellp());

		// marker for string table offset - will be patched later
		writeInt(ss, 0);

		// write prototype and fill string table
		writeProto(ss, strings, p, encode, ckey, modKey);

		// write string table
		unsigned int stringTableOffset = static_cast<unsigned int>(ss.tellp());

		std::vector<const TString*> stringTable(strings.size());

		for (DenseHashMap<const TString*, unsigned int>::const_iterator it = strings.begin(); it != strings.end(); ++it)
			stringTable[it->second - 1] = it->first;
		writeCompressedInt(ss, strings.size());
		for (size_t i = 0; i < stringTable.size(); ++i)
		{
			writeCompressedInt(ss, stringTable[i]->tsv.len);
		}
		for (size_t i = 0; i < stringTable.size(); ++i)
		{
			ss.write(getstr(stringTable[i]), stringTable[i]->tsv.len);
		}
		// patch string table offset
		ss.seekp(streamStart);
		writeInt(ss, stringTableOffset - streamStart);
	}

	static void writeByte(std::ostringstream & ss, unsigned char value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeInt(std::ostringstream & ss, int value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeCompressedInt(std::ostringstream & ss, unsigned int value)
	{
		do
		{
			int v = value & 0x7F;
			value >>= 7;

			if (value)
				v |= 0x80;

			writeByte(ss, v);
		} while (value);
	}

	static void writeDouble(std::ostringstream & ss, double value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeString(std::ostringstream & ss, DenseHashMap<const TString*, unsigned int> & strings, const TString * value)
	{
		if (value)
		{
			unsigned int& index = strings[value];

			if (index == 0)
				index = strings.size();

			writeCompressedInt(ss, index);
		}
		else
		{
			writeCompressedInt(ss, 0);
		}
	}

	static void writeProto(std::ostringstream & ss, DenseHashMap<const TString*, unsigned int> & strings, Proto * p, RbxOpEncoder encode, unsigned int ckey, unsigned int modKey)
	{
		writeCompressedInt(ss, p->sizep);
		writeCompressedInt(ss, p->sizek);
		writeCompressedInt(ss, p->sizecode);
		writeCompressedInt(ss, p->sizelocvars);
		writeCompressedInt(ss, p->sizelineinfo);
		writeCompressedInt(ss, p->sizeupvalues);

		writeByte(ss, p->maxstacksize);
		writeByte(ss, p->is_vararg);
		writeByte(ss, p->nups);
		writeByte(ss, p->numparams);

		/* This is a rather hacky solution; convert instructions here so they are ready for the proto hashing algorithm */
		for (int i = 0; i < p->sizecode; i++)
			p->code[i] = encode.encodeInstruction(p->code[i], i, ckey);

		/* hash */
		writeInt(ss, generateHash(p, modKey));

		okhand(p);

		

		for (int i = 0; i < p->sizek; ++i)
		{
			const TValue* o = &p->k[i];

			switch (ttype(o))
			{
			case LUA_TNIL:
				writeByte(ss, Constant_Nil);
				break;

			case LUA_TBOOLEAN:
				writeByte(ss, bvalue(o) ? Constant_True : Constant_False);
				break;

			case LUA_TNUMBER:
				writeByte(ss, Constant_Number);
				writeDouble(ss, nvalue(o));
				break;

			case LUA_TSTRING:
				writeByte(ss, Constant_String);
				writeString(ss, strings, rawtsvalue(o));
				break;

			default:
				lua_assert(0);
				break;
			}
		}

		int lastLine = 0;

		for (int i = 0; i < p->sizelineinfo; ++i)
		{
			// the lines weren't previously encoded.
			p->lineinfo[i] = p->lineinfo[i] ^ (i << 8);
			writeCompressedInt(ss, p->lineinfo[i] - lastLine);

			lastLine = p->lineinfo[i];
		}

		for (int i = 0; i < p->sizelocvars; ++i)
		{
			const struct LocVar* l = &p->locvars[i];

			writeCompressedInt(ss, l->startpc);
			writeCompressedInt(ss, l->endpc);
			writeString(ss, strings, l->varname);
		}

		for (int i = 0; i < p->sizeupvalues; ++i)
			writeString(ss, strings, p->upvalues[i]);

		for (int i = 0; i < p->sizecode; ++i)
			writeInt(ss, p->code[i]);

		for (int i = 0; i < p->sizep; ++i)
			writeProto(ss, strings, p->p[i], encode, ckey, modKey);
	}

	struct LoadS
	{
		const char* s;
		size_t size;
	};

	static const char* getS(lua_State * L, void* ud, size_t * size)
	{
		LoadS* ls = (LoadS*)ud;
		(void)L;
		if (ls->size == 0) return NULL;
		*size = ls->size;
		ls->size = 0;
		return ls->s;
	}
};