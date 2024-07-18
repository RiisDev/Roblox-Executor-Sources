#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

#include "OpcodeEncryption.h"
#include "DenseHash.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lobject.h"
#include "lstring.h"
#include "lfunc.h"
#include "ldo.h"
#include "lopcodes.h"
#include "lz4.h"
#include "lz4hc.h"
#include "xxhash.h"
}

/* ah yes alas roblox's very own code, this is the serializer which is NOT on the client */

#define LUAVM_SERIALIZER

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

#ifdef LUAVM_SERIALIZER
class LuaSerializer
{
public:
	static std::string serialize(lua_State* L, const std::string& source, RbxOpEncoder encode, unsigned int ckey)
	{
		LoadS ls = { source.c_str(), source.size() };

		std::ostringstream ss;
		int error = lua_load(L, getS, &ls, "=");

		writeByte(ss, error);
		if (error != 0)
		{
			const char* err = lua_tostring(L, -1);
			size_t length = err ? strlen(err) : 0;
			writeInt(ss, length);
			ss.write(err, length);
			lua_pop(L, 1);

			return std::string("error");

		}
		else
		{
			const LClosure* cl = static_cast<const LClosure*>(lua_topointer(L, -1));
			writeRoot(ss, cl->p, encode, ckey);
			lua_pop(L, 1);
		}

		std::string ssdata = ss.str();

		// compress
		int dataSize = ssdata.size();
		int maxSize = LZ4_compressBound(dataSize);
		std::vector<char> compressed(maxSize);
		int compressedSize = LZ4_compress_default(ssdata.c_str(), &compressed[0], dataSize, maxSize);

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
	static void writeRoot(std::ostringstream& ss, Proto* p, RbxOpEncoder encode, unsigned int ckey)
	{
		DenseHashMap<const TString*, unsigned int> strings;
		unsigned int streamStart = ss.tellp();

		// marker for string table offset - will be patched later
		writeInt(ss, 0);

		// write prototype and fill string table
		writeProto(ss, strings, p, encode, ckey);

		// write string table
		unsigned int stringTableOffset = ss.tellp();

		std::vector<const TString*> stringTable(strings.size());

		for (DenseHashMap<const TString*, unsigned int>::const_iterator it = strings.begin(); it != strings.end(); ++it)
			stringTable[it->second - 1] = it->first;
		writeInt(ss, strings.size());
		for (size_t i = 0; i < stringTable.size(); ++i) {
			writeInt(ss, stringTable[i]->tsv.len);
		}
		for (size_t i = 0; i < stringTable.size(); ++i){
			ss.write(getstr(stringTable[i]), stringTable[i]->tsv.len);
		}
		// patch string table offset
		ss.seekp(streamStart);
		writeInt(ss, stringTableOffset - streamStart);
	}

	static void writeByte(std::ostringstream& ss, unsigned char value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeInt(std::ostringstream& ss, int value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeDouble(std::ostringstream& ss, double value)
	{
		ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	static void writeString(std::ostringstream& ss, DenseHashMap<const TString*, unsigned int>& strings, const TString* value)
	{
		if (value)
		{
			unsigned int& index = strings[value];

			if (index == 0)
				index = strings.size();

			writeInt(ss, index);
		}
		else
		{
			writeInt(ss, 0);
		}
	}

	static void writeProto(std::ostringstream& ss, DenseHashMap<const TString*, unsigned int>& strings, Proto* p, RbxOpEncoder encode, unsigned int ckey)
	{
		writeInt(ss, p->sizep);
		writeInt(ss, p->sizek);
		writeInt(ss, p->sizecode);
		writeInt(ss, p->sizelocvars);
		writeInt(ss, p->sizelineinfo);
		writeInt(ss, p->sizeupvalues);

		writeByte(ss, p->maxstacksize);
		writeByte(ss, p->is_vararg);
		writeByte(ss, p->nups);
		writeByte(ss, p->numparams);

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
			writeInt(ss, p->lineinfo[i] - lastLine);

			lastLine = p->lineinfo[i];
		}

		for (int i = 0; i < p->sizelocvars; ++i)
		{
			const struct LocVar* l = &p->locvars[i];

			writeInt(ss, l->startpc);
			writeInt(ss, l->endpc);
			writeString(ss, strings, l->varname);
		}

		for (int i = 0; i < p->sizeupvalues; ++i)
			writeString(ss, strings, p->upvalues[i]);

		for (int i = 0; i < p->sizecode; ++i)
		{
			writeInt(ss, encode.Encrypt(p->code[i], i, ckey));
		}

		for (int i = 0; i < p->sizep; ++i)
			writeProto(ss, strings, p->p[i], encode, ckey);
	}

	struct LoadS
	{
		const char *s;
		size_t size;
	};

	static const char* getS(lua_State *L, void *ud, size_t *size)
	{
		LoadS *ls = (LoadS *)ud;
		(void)L;
		if (ls->size == 0) return NULL;
		*size = ls->size;
		ls->size = 0;
		return ls->s;
	}
};
#endif