#pragma once
#include "Encryptions.h"
#include "RBX.h"

DWORD encode_key;

#define proto_obfuscate(p, v) *(int*)(int)(p) = (int)(v) - (int)(p)
#define closure_obfuscate(p, v) *(int*)(int)(p) = (int)(p) - (int)(v)

int inverse(__int64 a, __int64 m) {
	__int64 m0 = m, t, q;
	__int64 x0 = 0, x1 = 1;
	while (a > 1)
	{
		q = a / m;
		t = m;
		m = a % m, a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}

	if (x1 < 0)
		x1 += m0;
	return x1;
}

class RbxOpEncoder {
public:
	Instruction convert_instructions(Instruction inst, DWORD i, DWORD ckey);
};

Instruction RbxOpEncoder::convert_instructions(Instruction inst, DWORD i, DWORD ckey) {
	int r_inst;

	OpCode op = GET_OPCODE(inst);

	R_SET_OPCODE(r_inst, get_roblox_opcode[op]);

	switch (getOpMode(op)) {
	case iABC:
		R_SETARG_A(r_inst, GETARG_A(inst));
		R_SETARG_B(r_inst, GETARG_B(inst));
		R_SETARG_C(r_inst, GETARG_C(inst));
		break;
	case iABx:
		R_SETARG_A(r_inst, GETARG_A(inst));
		R_SETARG_Bx(r_inst, GETARG_Bx(inst));
		break;
	case iAsBx:
		R_SETARG_A(r_inst, GETARG_A(inst));
		R_SETARG_sBx(r_inst, GETARG_sBx(inst));
		break;

	default:
		break;
	}

	switch (op) {
	case OP_JMP:
		r_inst = ((r_inst >> 26 << 26) | jump_encryption(r_inst, i));
		break;
	case OP_CALL:
		r_inst = ((r_inst >> 26 << 26) | call_enc(r_inst, i));
		break;
	case OP_TAILCALL:
	case OP_RETURN:
		r_inst = ((r_inst >> 26 << 26) | dax_encode_op(r_inst, i, 1470882913, i - 1577854801, 641680189) & 0x3FFFFFF);
		break;
	case OP_SETUPVAL:
		r_inst = ((r_inst >> 26 << 26) | setupval_encryption(r_inst, i));
		break;
	case OP_CLOSURE:
		r_inst = ((r_inst >> 26 << 26) | closure_enc(r_inst, i));
		break;
	case OP_MOVE:
		R_SETARG_C(r_inst, 1);
		break;

	default:
		break;
	}

	r_inst *= ckey;

	return r_inst;
}


double r_xor_number(double* number);

#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

#include <vector>
#include <cassert>
#include <utility>
#include <functional>

#define LUAVM_ENCODEINSN(insn, key) ((insn) * key)



namespace detail
{
	template <typename Key, typename Item, typename Hash, typename Eq>
	class DenseHashTable
	{
	public:
		typedef typename std::vector<Item>::const_iterator const_iterator;

		DenseHashTable(size_t capacity, const Hash& hash, const Eq& eq)
			: filled(0)
			, hash(hash)
			, eq(eq)
		{
			if (capacity)
			{
				items.reserve(capacity);
				rehash(capacity);
			}
		}

		const_iterator begin() const
		{
			return items.begin();
		}

		const_iterator end() const
		{
			return items.end();
		}

		bool empty() const
		{
			return items.empty();
		}

		size_t size() const
		{
			return items.size();
		}

		size_t bucket_count() const
		{
			return buckets.size();
		}

		float load_factor() const
		{
			return buckets.empty() ? 0 : float(filled) / float(buckets.size());
		}

		void clear()
		{
			items.clear();
			buckets.clear();
			filled = 0;
		}

	protected:
		std::vector<Item> items;
		std::vector<int32_t> buckets;
		size_t filled;

		Hash hash;
		Eq eq;

		void rehash()
		{
			if (filled >= buckets.size() * 3 / 4)
			{
				rehash(items.size() * 2);
			}
		}

		void rehash(size_t capacity)
		{
			size_t newbuckets = 16;
			while (newbuckets < capacity) newbuckets *= 2;

			size_t hashmod = newbuckets - 1;

			std::vector<int32_t>(newbuckets, -1).swap(buckets);

			for (size_t i = 0; i < items.size(); ++i)
			{
				size_t bucket = hash(getKey(items[i])) & hashmod;

				for (size_t probe = 0; probe <= hashmod; ++probe)
				{
					if (buckets[bucket] < 0)
					{
						buckets[bucket] = i;
						break;
					}

					bucket = (bucket + probe + 1) & hashmod;
				}
			}

			filled = items.size();
		}

		int find_bucket(const Key & key) const
		{
			if (buckets.empty())
				return -1;

			size_t hashmod = buckets.size() - 1;
			size_t bucket = hash(key) & hashmod;

			for (size_t probe = 0; probe <= hashmod; ++probe)
			{
				int32_t probe_index = buckets[bucket];

				if (probe_index == -1)
					return -1;

				if (probe_index >= 0 && eq(getKey(items[probe_index]), key))
					return bucket;

				bucket = (bucket + probe + 1) & hashmod;
			}

			assert(false);
			return -1;
		}

		std::pair<Item*, bool> insert_item(const Key & key)
		{
			assert(!buckets.empty());

			size_t hashmod = buckets.size() - 1;
			size_t bucket = hash(key) & hashmod;

			for (size_t probe = 0; probe <= hashmod; ++probe)
			{
				int32_t probe_index = buckets[bucket];

				if (probe_index < 0)
				{
					buckets[bucket] = items.size();
					filled += probe_index == -1;

					items.push_back(Item());
					getKey(items.back()) = key;

					return std::make_pair(&items.back(), true);
				}

				if (eq(getKey(items[probe_index]), key))
					return std::make_pair(&items[probe_index], false);

				bucket = (bucket + probe + 1) & hashmod;
			}

			assert(false);
			return std::make_pair(static_cast<Item*>(0), false);
		}

		void erase_bucket(int bucket)
		{
			assert(bucket >= 0);

			int32_t probe_index = buckets[bucket];
			assert(probe_index >= 0);

			int probe_bucket = find_bucket(getKey(items.back()));
			assert(probe_bucket >= 0);

			items[probe_index] = items.back();
			buckets[probe_bucket] = probe_index;

			items.pop_back();

			buckets[bucket] = -2;
		}

	private:
		static const Key & getKey(const Key & item) { return item; }
		static Key& getKey(Key & item) { return item; }
		template <typename Value> static const Key& getKey(const std::pair<Key, Value> & item) { return item.first; }
		template <typename Value> static Key& getKey(std::pair<Key, Value> & item) { return item.first; }
	};
}

template <typename Key, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key>>
class DenseHashSet : public detail::DenseHashTable<Key, Key, Hash, Eq>
{
public:
	explicit DenseHashSet(size_t capacity = 0, const Hash& hash = Hash(), const Eq& eq = Eq())
		: detail::DenseHashTable<Key, Key, Hash, Eq>(capacity, hash, eq)
	{
	}

	bool contains(const Key& key) const
	{
		return this->find_bucket(key) >= 0;
	}

	bool insert(const Key& key)
	{
		this->rehash();

		return this->insert_item(key).second;
	}

	void erase(const Key& key)
	{
		int bucket = this->find_bucket(key);

		if (bucket >= 0)
			this->erase_bucket(bucket);
	}
};

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key>>
class DenseHashMap : public detail::DenseHashTable<Key, std::pair<Key, Value>, Hash, Eq>
{
public:
	explicit DenseHashMap(size_t capacity = 0, const Hash& hash = Hash(), const Eq& eq = Eq())
		: detail::DenseHashTable<Key, std::pair<Key, Value>, Hash, Eq>(capacity, hash, eq)
	{
	}

	const Value* find(const Key& key) const
	{
		int bucket = this->find_bucket(key);

		return bucket < 0 ? NULL : &this->items[this->buckets[bucket]].second;
	}

	Value& operator[](const Key& key)
	{
		this->rehash();

		return this->insert_item(key).first->second;
	}

	void erase(const Key& key)
	{
		int bucket = this->find_bucket(key);

		if (bucket >= 0)
			this->erase_bucket(bucket);
	}
};

extern "C"
{
#include "LZ4\lz4.h"
#include "LZ4\lz4hc.h"
#include "xxhash/xxhash.h"
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


void lua_pushlclosure(uintptr_t rL, DWORD closure)
{
	TValue* top = *(TValue * *)(rL + 8);
	top->value.gc = (GCObject*)closure;
	top->tt = RLUA_TFUNCTION;
	*(uint32_t*)(rL + 8) += sizeof(TValue);
}
class LuaSerializer
{
public:
	template<typename T>
	static std::string serialize(lua_State* L, const std::string& source, T encode, unsigned int ckey)
	{
		LoadS ls = { source.c_str(), source.size() };

		std::ostringstream ss;
		// write basic data
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
		return ss.str();
	}

private:
	template<typename T>
	static void writeRoot(std::ostringstream & ss, Proto * p, T encode, unsigned int ckey)
	{
		DenseHashMap<const TString*, unsigned int> strings(NULL);
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

		for (size_t i = 0; i < stringTable.size(); ++i)
			writeInt(ss, stringTable[i]->tsv.len);
		for (size_t i = 0; i < stringTable.size(); ++i)
			ss.write(getstr(stringTable[i]), stringTable[i]->tsv.len);
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

			writeInt(ss, index);
		}
		else
		{
			writeInt(ss, 0);
		}
	}

	template<typename T>
	static void writeProto(std::ostringstream& ss, DenseHashMap<const TString*, unsigned int>& strings, Proto* p, T encode, unsigned int ckey)
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
			const auto o = &p->k[i];

			switch (ttype(o))
			{
			case LUA_TNIL:
				writeByte(ss, Constant_Nil);
				break;

			case LUA_TBOOLEAN:
				writeByte(ss, bvalue(o) ? Constant_True : Constant_False);
				break;

			case LUA_TNUMBER: {
				writeByte(ss, Constant_Number);
				writeDouble(ss, r_xor_number(&o->value.n));
				break;
			}
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
			p->lineinfo[i] = p->lineinfo[i];
			writeInt(ss, p->lineinfo[i] - lastLine);

			lastLine = p->lineinfo[i];
		}

		for (int i = 0; i < p->sizelocvars; ++i)
		{
			struct LocVar* l = &p->locvars[i];
			writeInt(ss, l->startpc);
			writeInt(ss, l->endpc);
			writeString(ss, strings, l->varname);
		}

		for (int i = 0; i < p->sizeupvalues; ++i)
		{
			writeString(ss, strings, p->upvalues[i]);
		}
		for (int i = 0; i < p->sizecode; ++i)
		{
			writeInt(ss, encode.convert_instructions(p->code[i], i, ckey));
		}

		for (int i = 0; i < p->sizep; ++i)
		{
			writeProto(ss, strings, p->p[i], encode, ckey);
		}
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

class LuaDeserializer
{
public:
	static int deserialize(uintptr_t rL, const std::string& code, const char* chunkname)
	{
		std::string buffer = code;

		std::vector<char> data(buffer.begin(), buffer.end());

		VectorStream ss(data);

		// decode error
		if (int error = readByte(ss))
		{
			unsigned int length = readInt(ss);

			return 1;
		}

		// deserialize (finally!)

		auto source = rluaS_newlstr(rL, chunkname, strlen(chunkname));
		auto p = readRoot(ss, rL, source);
		auto cl = RBX::luaF_newLclosure(rL, 0, *reinterpret_cast<uintptr_t*>(rL + 64));
		closure_obfuscate(cl + 20, p);
		lua_pushlclosure(rL, cl);

		
	}

	static int deserializeFailure(lua_State * L, const char* chunkname)
	{
		return deserializeError(L, "", chunkname);
	}

	static int deserializeError(lua_State * L, const std::string & error, const char* chunkname, int code = 1)
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
			size_t newsize = min(size, data.size() - offset);

			memcpy(buffer, &data[0] + offset, newsize);
			offset += newsize;
		}

		size_t tellg() const
		{
			return offset;
		}

		void seekg(size_t newoffset)
		{
			offset = newoffset;
		}

	private:
		std::vector<char>& data;
		size_t offset;
	};

	static RBX::Proto* readRoot(VectorStream& ss, uintptr_t rL, TString* source)
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

			strings[i] = rluaS_newlstr(rL, &stringBuffer[0], length);
		}

		ss.seekg(streamStart + 4);

		return readProto(ss, strings, rL, source);
	}

	static unsigned char readByte(VectorStream & ss)
	{
		unsigned char value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static int readInt(VectorStream & ss)
	{
		int value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static double readDouble(VectorStream & ss)
	{
		double value = 0;
		ss.read(reinterpret_cast<char*>(&value), sizeof(value));
		return value;
	}

	static TString* readString(VectorStream & ss, const std::vector<TString*> & strings)
	{
		unsigned int index = readInt(ss);
		return index ? strings[index - 1] : NULL;
	}

	static RBX::Proto* readProto(VectorStream & ss, const std::vector<TString*> & strings, uintptr_t rL, TString * source)
	{
		RBX::Proto* p = RBX::luaF_newproto(rL);

		p->sizep = readInt(ss);
		p->sizek = readInt(ss);
		p->sizecode = readInt(ss);
		p->sizelocvars = readInt(ss);
		p->sizelineinfo = readInt(ss);
		p->sizeupvalues = readInt(ss);

		proto_obfuscate(&p->source, source);
		auto pPool =
			reinterpret_cast<RBX::Proto * *>(rluaM_realloc_(rL, 0, 0, p->sizep * sizeof(Proto*), 3));
		proto_obfuscate(&p->p, pPool);
		auto kPool =
			reinterpret_cast<TValue*>(rluaM_realloc_(rL, 0, 0, p->sizek * sizeof(TValue), 3));
		proto_obfuscate(&p->k, kPool);
		auto cPool =
			reinterpret_cast<Instruction*>(rluaM_realloc_(rL, 0, 0, p->sizecode * sizeof(Instruction), 3));
		proto_obfuscate(&p->code, cPool);
		auto lvPool =
			reinterpret_cast<LocVar*>(rluaM_realloc_(rL, 0, 0, p->sizelocvars * sizeof(LocVar), 3));
		proto_obfuscate(&p->locvars, lvPool);

		auto liPool =
			reinterpret_cast<int*>(rluaM_realloc_(rL, 0, 0, p->sizelineinfo * sizeof(int), 3));
		proto_obfuscate(&p->lineinfo, liPool);

		auto uPool =
			reinterpret_cast<TString * *>(rluaM_realloc_(rL, 0, 0, p->sizeupvalues * sizeof(TString*), 3));
		proto_obfuscate(&p->upvalues, uPool);
		p->maxstacksize = readByte(ss);
		p->is_vararg = readByte(ss);
		p->nups = readByte(ss);
		p->numparams = readByte(ss);
		for (int i = 0; i < p->sizek; ++i)
		{
			auto o = &kPool[i];

			switch (readByte(ss))
			{
			case Constant_Nil:

				o->tt = RLUA_TNIL;
				break;
			case Constant_False:

				o->tt = RLUA_TBOOLEAN;
				o->value.b = false;
				break;

			case Constant_True:
				
				o->tt = RLUA_TBOOLEAN;
				o->value.b = true;
				break;

			case Constant_Number:
			
				o->tt = RLUA_TNUMBER;
				o->value.n = readDouble(ss);
				break;

			case Constant_String:
				
				o->tt = RLUA_TSTRING;
				o->value.gc = (GCObject*)readString(ss, strings);
				break;

			default:
				break;
			}
		}

		int lastLine = 0;

		for (int i = 0; i < p->sizelineinfo; ++i)
		{
		
			liPool[i] = readInt(ss) + lastLine;

			lastLine = liPool[i];
		}

		for (int i = 0; i < p->sizelocvars; ++i)
		{
			auto l = &lvPool[i];
			
			l->startpc = readInt(ss);
			l->endpc = readInt(ss);
			l->varname = readString(ss, strings);
		}

		for (int i = 0; i < p->sizeupvalues; ++i) {
	
			uPool[i] = readString(ss, strings);
		}

		for (int i = 0; i < p->sizecode; ++i)
		{
			
			cPool[i] = readInt(ss) * 1;
		}
		for (int i = 0; i < p->sizep; ++i)
		{
		
			pPool[i] = readProto(ss, strings, rL, source);
		}
		// p->hash = calculateHash(p, luaKeyPair.first);

		return p;
	}
};




void set_encode_key(int rL, DWORD* key) { // string "local", decompile for decode key
	int a1 = rL;
	int _ckey = (a1 + 44 + *(DWORD*)(a1 + 12)) ^ *(DWORD*)(a1 + 44 + *(DWORD*)(a1 + 12));
	*key = inverse(_ckey, 4294967296);
}




double r_xor_number(double* number) {
	int rxor = Format(0x20456A0);
	PVOID old_number = number;

	double new_number;

	__asm {
		mov edx, old_number
		mov eax, rxor
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd new_number, xmm1
	}

	return new_number;
}



void compile_script(int rL, lua_State * L, PCHAR source) {
	/* compilation and then running of bytecode */

	int thread = rL;
	int buffer[] = { 0, 0 };;

	try {
		set_encode_key(thread, &encode_key);
	//	SandboxThread(thread, 6, (int)buffer); //context 7 went to 6


		if (luaL_loadstring(L, source))
		{
			printf("Lua Error: %s\n", lua_tostring(L, -1));

			lua_pop(L, 1);
			return;
		}
		else
		{
			RbxOpEncoder converter;
			std::string bytecode = LuaSerializer::serialize(L, std::string(source), converter, encode_key);
			if (bytecode.size() > 0) {
				LuaDeserializer::deserialize(thread, bytecode, source); 
			}
			Spawn(thread);
		
		}
	}

	catch (...) {
	}
}

void execute_script(int rL, lua_State * L, const std::string & source) {
	compile_script(rL, L, (PCHAR)source.c_str());
}
