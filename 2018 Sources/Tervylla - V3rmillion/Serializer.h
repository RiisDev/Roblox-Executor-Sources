#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "DenseHash.h"
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include "RbxOpEncoder.h"
#include "DenseHash.h"
extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lobject.h"
#include "Lua/lstring.h"
#include "Lua/lfunc.h"
#include "Lua/ldo.h"
#include "Lua/ldebug.h"
#include "Lua/lopcodes.h"
#include "LZ4\lz4.h"
}

#include "xxTeaHash\xxhash.h"

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

#define LUAVM_SERIALIZER

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
		for (size_t i = 0; i < stringTable.size(); ++i) {
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
			writeInt(ss, encode.encode(p->code[i], i, ckey));
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

#ifdef LUAVM_DESERIALIZER
class LuaDeserializer
{
public:
	static int deserialize(lua_State* L, const std::string& code, const char* chunkname, unsigned int modkey)
	{
		// This happens if one of the client-side lua compile/execute paths is hit - deserves a special 'empty error' case for obscurity
		if (code.empty())
			return deserializeError(L, "", chunkname);

		if (code.size() <= 8)
			return deserializeFailure(L, chunkname);

		std::string buffer = code;

		// recover the hash key
		char hashbytes[4];
		memcpy(hashbytes, &buffer[0], 4);

		for (size_t i = 0; i < 4; ++i)
		{
			hashbytes[i] ^= kBytecodeMagic[i];
			hashbytes[i] -= i * kBytecodeHashMultiplier;
		}

		// un-xor the buffer
		for (size_t i = 0; i < buffer.size(); ++i)
			buffer[i] ^= hashbytes[i % 4] + i * kBytecodeHashMultiplier;

		// re-hash the buffer
		unsigned int hash;
		memcpy(&hash, hashbytes, 4);

		unsigned int rehash = XXH32(&buffer[0], buffer.size(), kBytecodeHashSeed);

		if (rehash != hash)
			return deserializeFailure(L, chunkname);

		// uncompress the buffer
		int dataSize;
		memcpy(&dataSize, &buffer[4], 4);

		std::vector<char> data(dataSize);

		if (LZ4_decompress_safe(&buffer[8], &data[0], buffer.size() - 8, dataSize) != dataSize)
			return deserializeFailure(L, chunkname);

		VectorStream ss(data);

		// decode error
		if (int error = readByte(ss))
		{
			unsigned int length = readInt(ss);

			if (length == data.size() - 5)
				return deserializeError(L, std::string(&data[5], length), chunkname, error);
			else
				return deserializeFailure(L, chunkname);
		}

		// deserialize (finally!)
		TString* source = luaS_new(L, chunkname);
		Proto* p = readRoot(ss, L, source, modkey);

		Closure* cl = luaF_newLclosure(L, p->nups, hvalue(gt(L)));

		cl->l.p = p;
		for (int i = 0; i < p->nups; i++)
			cl->l.upvals[i] = luaF_newupval(L);

		setclvalue(L, L->top, cl);
		incr_top(L);

		RBXASSERT_VERY_FAST(luaG_checkcode(p, L->l_G->ckey));

		return 0;
	}

	static int deserializeFailure(lua_State* L, const char* chunkname)
	{
		RBX::DataModel::sendStats |= HATE_INVALID_BYTECODE;

		return deserializeError(L, "", chunkname);
	}

	static int deserializeError(lua_State* L, const std::string& error, const char* chunkname, int code = 1)
	{
		char chunkid[80];
		luaO_chunkid(chunkid, chunkname, sizeof(chunkid));

		std::string text = chunkid + error;

		lua_pushlstring(L, text.c_str(), text.size());

		return code;
	}

private:
	class VectorStream
	{
	public:
		VectorStream(std::vector<char>& data)
			: data(data), offset(0)
		{
		}

		void read(char* buffer, size_t size)
		{
			size_t newsize = std::min(size, data.size() - offset);

			memcpy(buffer, &data[0] + offset, newsize);
			offset += newsize;
		}

		size_t tellg() const
		{
			return offset;
		}

		void seekg(size_t newoffset)
		{
			RBXASSERT_VERY_FAST(newoffset <= data.size());

			offset = newoffset;
		}

	private:
		std::vector<char>& data;
		size_t offset;
	};

	static Proto* readRoot(VectorStream& ss, lua_State* L, TString* source, unsigned int modkey)
	{
		unsigned int streamStart = ss.tellg();

		unsigned int stringTableOffset = readInt(ss);

		ss.seekg(streamStart + stringTableOffset);

		unsigned int stringCount = readInt(ss);

		std::vector<unsigned int> stringLengths(stringCount);

		for (unsigned int i = 0; i < stringCount; ++i)
			stringLengths[i] = readInt(ss);

		std::vector<TString*> strings(stringCount);

		std::vector<char> stringBuffer;

		for (unsigned int i = 0; i < stringCount; ++i)
		{
			unsigned int length = stringLengths[i];

			stringBuffer.resize(length);

			ss.read(&stringBuffer[0], length);

			strings[i] = luaS_newlstr(L, &stringBuffer[0], length);
		}

		ss.seekg(streamStart + 4);
		return readProto(ss, strings, L, source, modkey);
	}

	static unsigned char readByte(VectorStream& ss)
	{
		unsigned char value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static int readInt(VectorStream& ss)
	{
		int value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static double readDouble(VectorStream& ss)
	{
		double value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static TString* readString(VectorStream& ss, const std::vector<TString*>& strings)
	{
		unsigned int index = readInt(ss);

		RBXASSERT_VERY_FAST(index <= strings.size());

		return index ? strings[index - 1] : NULL;
	}

	static Proto* readProto(VectorStream& ss, const std::vector<TString*>& strings, lua_State* L, TString* source, unsigned int modkey)
	{
		Proto* p = luaF_newproto(L);

		p->source = source;

		p->sizep = readInt(ss);
		p->p = luaM_newvector(L, p->sizep, Proto*);

		p->sizek = readInt(ss);
		p->k = luaM_newvector(L, p->sizek, TValue);

		p->sizecode = readInt(ss);
		p->code = luaM_newvector(L, p->sizecode, InstructionV);

		p->sizelocvars = readInt(ss);
		p->locvars = luaM_newvector(L, p->sizelocvars, LocVar);

		p->sizelineinfo = readInt(ss);
		p->lineinfo = luaM_newvector(L, p->sizelineinfo, int);

		p->sizeupvalues = readInt(ss);
		p->upvalues = luaM_newvector(L, p->sizeupvalues, TString*);

		p->maxstacksize = readByte(ss);
		p->is_vararg = readByte(ss);
		p->nups = readByte(ss);
		p->numparams = readByte(ss);

		for (int i = 0; i < p->sizek; ++i)
		{
			TValue* o = &p->k[i];

			switch (readByte(ss))
			{
			case Constant_Nil:
				setnilvalue(o);
				break;

			case Constant_False:
				setbvalue(o, false);
				break;

			case Constant_True:
				setbvalue(o, true);
				break;

			case Constant_Number:
				setnvalue(o, readDouble(ss));
				break;

			case Constant_String:
				setsvalue2n(L, o, readString(ss, strings));
				break;

			default:
				lua_assert(0);
				break;
			}
		}

		int lastLine = 0;

		for (int i = 0; i < p->sizelineinfo; ++i)
		{
			p->lineinfo[i] = readInt(ss) + lastLine;

			lastLine = p->lineinfo[i];
		}

		for (int i = 0; i < p->sizelocvars; ++i)
		{
			struct LocVar* l = &p->locvars[i];

			l->startpc = readInt(ss);
			l->endpc = readInt(ss);
			l->varname = readString(ss, strings);
		}

		for (int i = 0; i < p->sizeupvalues; ++i)
			p->upvalues[i] = readString(ss, strings);

		for (int i = 0; i < p->sizecode; ++i)
			p->code[i].v = readInt(ss) * modkey;

		for (int i = 0; i < p->sizep; ++i)
			p->p[i] = readProto(ss, strings, L, source, modkey);
		return p;
	}
};
#endif
