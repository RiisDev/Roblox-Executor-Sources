#include "LuauSerializer.h"

/* buffer reading */
double readDouble(const char* dump, int& offset) {
	auto res = *(lua_Number*)(dump + offset);
	offset += 8;
	return res;
}

int readCompressedInt(const char* dump, int& offset) {
	int result = 0;
	int added = 0;
	int current;

	do {
		current = *(char*)(offset + dump);
		result |= (*(char*)((offset)++ + dump) & 0x7F) << added;
		added += 7;
	} while (current < 0);

	return result;
}

int readInt(const char* dump, int& offset) {
	auto res = *(int*)(dump + offset);
	offset += 4;
	return res;
}

char readByte(const char* dump, int& offset) {
	auto res = *(const char*)(dump + offset);
	offset += 1;
	return res;
}

#include <Windows.h>
const char* readString(const char* dump, int& offset, int& strSize) {
	strSize = readCompressedInt(dump, offset);
	auto dest = VirtualAlloc(NULL, strSize + 1, MEM_COMMIT, PAGE_READWRITE);// calloc(strSize + 1, strSize + 1);
	memcpy(dest, dump + offset, strSize);
	offset += strSize;
	return (const char*)dest;
}

/* buffer writing */
void writeByte(std::string& buffer, char obj) {
	buffer += obj;
}

void writeInt(std::string& buffer, int obj) {
	for (int i = 0; i < sizeof(__int32); i++) {
		writeByte(buffer, reinterpret_cast<const char*>(&obj)[i]);
	}
}

static void writeCompressedInt(std::string& buffer, unsigned int value)
{
	do
	{
		int v = value & 0x7F;
		value >>= 7;

		if (value)
			v |= 0x80;

		writeByte(buffer, v);
	} while (value);
}

void writeDouble(std::string& buffer, lua_Number obj) {
	for (int i = 0; i < sizeof(__int64); i++) {
		writeByte(buffer, reinterpret_cast<const char*>(&obj)[i]);
	}
}

void writeString(std::string& buffer, const char* str, int len) {
	writeCompressedInt(buffer, len);
	buffer += std::string(str, len);
}

#define checkarray(field, size)if ((unsigned int)field > size){ break; }

int LuauSerializer::readClosure(lua_State* L, const char* chunkname, const char* source, size_t len) {
	int offset = 1;

	std::vector<Proto*> protoTable;

	if (source[0] == 1) {
		// handle all proto strings
		auto protoSource = luaS_new(L, chunkname);

		int stringSize = readCompressedInt(source, offset);
		std::vector<TString*> stringsTable(stringSize);

		for (int i = 0; i < stringSize; i++) {
			int strSize = 0;

			auto str = readString(source, offset, strSize);
			stringsTable[i] = (luaS_newlstr(L, str, strSize));
		}

		// begin reading proto
		int protoSize = readCompressedInt(source, offset);

		for (;;) {
			auto f = luaF_newproto(L);

			// bytes
			f->source = protoSource;
			f->maxstacksize = readByte(source, offset);
			f->numparams = readByte(source, offset);
			f->nups = readByte(source, offset);
			f->is_vararg = readByte(source, offset);

			// proto code
			f->sizecode = readCompressedInt(source, offset);

			checkarray(f->sizecode, 0x3FFFFFFF);
			f->code = luaM_newvector(L, f->sizecode, Instruction);

			if (f->sizecode > 0) {
				for (int i = 0; i < f->sizecode; i++) {
					f->code[i] = readInt(source, offset);
				}
			}

			// proto constants
			f->sizek = readCompressedInt(source, offset);

			checkarray(f->sizek, 0xFFFFFFF);
			f->k = luaM_newvector(L, f->sizek, TValue);

			if (f->sizek > 0) {
				for (int i = 0; i < f->sizek; i++) {
					switch (readByte(source, offset)) {
					case Constant_Nil: {
						setnilvalue(&f->k[i]);
						break;
					}
					case Constant_Bool: {
						setbvalue(&f->k[i], readByte(source, offset) != 0);
						break;
					}
					case Constant_Number: {
						setnvalue(&f->k[i], readDouble(source, offset));
						break;
					}
					case Constant_String: {
						setsvalue2n(
							L, &f->k[i], stringsTable[readCompressedInt(source, offset) - 1]
						);
						break;
					}
					case Constant_Method: {
						readInt(source, offset);
						setnilvalue(&f->k[i]);
						break;
					}
					case Constant_Global: {
						int globalSize = readCompressedInt(source, offset);
						if (globalSize) {
							for (int _i = 0; _i < globalSize; _i++) {
								readCompressedInt(source, offset); // this would be an automatically resolved global
							}
							setnilvalue(&f->k[i]);
						}
						break;
					}
					}
				}
			}

			// proto children
			f->sizep = readCompressedInt(source, offset);

			checkarray(f->sizep, 0x3FFFFFFF);
			f->p = luaM_newvector(L, f->sizep, Proto*);

			if (f->sizep > 0) {
				for (int i = 0; i < f->sizep; i++) {
					f->p[i] = protoTable.at(readCompressedInt(source, offset));
				}
			}

			if (int sourceOffset = readCompressedInt(source, offset))
				f->source = stringsTable[sourceOffset - 1];

			// proto lines
			if (readByte(source, offset)) { // lineinfo included?
				auto shift = readByte(source, offset);

				auto first = (f->sizecode + 3) & 0xFFFFFFFC;
				auto second = ((f->sizecode - 1) >> shift) + 1;

				f->sizelineinfo = f->sizecode;
				f->lineinfo = luaM_newvector(L, f->sizecode, int);

				char lastline = 0;
				if (!(f->sizecode == 0) && !(f->sizecode < 0)) {
					for (int i = 0; i < f->sizecode; i++) {
						auto line = readByte(source, offset);
						lastline += line;
					}
				}

				int lastline2 = 0;
				if (second > 0) {
					bool c;
					do {
						c = (second-- == 1);
					} while (!c);
				}
			}

			// locvars and upvalues, if included
			if (readByte(source, offset)) {
				f->sizelocvars = readCompressedInt(source, offset);

				checkarray(f->sizelocvars, 0xFFFFFFF);
				f->locvars = luaM_newvector(L, f->sizelocvars, LocVar);

				if (f->sizelocvars > 0) {
					for (int i = 0; i < f->sizelocvars; i++) {
						if (int locvarName = readCompressedInt(source, offset))
							f->locvars[i].varname = stringsTable[locvarName - 1];

						f->locvars[i].startpc = readCompressedInt(source, offset);
						f->locvars[i].endpc = readCompressedInt(source, offset);

						readCompressedInt(source, offset);
						readInt(source, offset);
					}
				}

				f->sizeupvalues = readCompressedInt(source, offset);

				checkarray(f->sizeupvalues, 0x3FFFFFFF);
				f->upvalues = luaM_newvector(L, f->sizeupvalues, TString*);

				if (f->sizeupvalues > 0) {
					for (int i = 0; i < f->sizeupvalues; i++) {
						if (int upvalueString = readCompressedInt(source, offset))
							f->upvalues[i] = stringsTable[upvalueString - 1];
					}
				}
			}

			protoTable.push_back(f);

			if (protoTable.size() == protoSize) {
				goto finish;
			}
		}

		luaM_toobig(L);
	}
	else {
		char chunkid[80];

		luaO_chunkid(chunkid, chunkname, 80);
		lua_pushstring(L, chunkid);

		if (source[0]) {
			lua_pushstring(L, ": bytecode version mismatch");
		}
		else {
			lua_pushlstring(L, source + 1, len - 1);
		}

		lua_concat(L, 2);

		return 1;
	}

finish:
	auto p = protoTable[readCompressedInt(source, offset)];
	Closure* cl = luaF_newLclosure(L, p->nups, hvalue(gt(L)));

	cl->l.p = p;
	for (int i = 0; i < p->nups; i++)
		cl->l.upvals[i] = luaF_newupval(L);

	setclvalue(L, L->top, cl);
	incr_top(L);

	return 0;
}

struct ProtoEx {
	Proto* p;
	std::vector<int> proto_array;
	std::vector<int> strings_array;
};

int mapTables(Proto* p, std::vector<ProtoEx*>& protoTable, std::vector<std::string>& stringTable) {
	auto px = new ProtoEx{};
	px->p = p;

	for (int i = 0; i < p->sizep; i++) {
		int curr_offset = mapTables(p->p[i], protoTable, stringTable);
		px->proto_array.push_back(curr_offset);
	}

	for (int i = 0; i < p->sizek; i++) {
		auto o = &p->k[i];

		if (o->tt == LUA_TSTRING) {
			stringTable.push_back(
				std::string(getstr(tsvalue(o)), tsvalue(o)->len)
			);
			px->strings_array.push_back(stringTable.size());
		}
	}

	int map_offset = protoTable.size();
	protoTable.push_back(px);

	return map_offset;
}

std::string LuauSerializer::writeClosure(lua_State* L) {
	std::string buffer;

	if (lua_type(L, -1) == LUA_TFUNCTION) {
		auto cl = static_cast<const LClosure*>(lua_topointer(L, -1));
		Proto* f = cl->p;

		std::vector<ProtoEx*> protoTable;
		std::vector<std::string> stringTable;

		writeByte(buffer, 1);

		int topProto = mapTables(f, protoTable, stringTable);
		writeCompressedInt(buffer, stringTable.size());

		for (int i = 0; i < stringTable.size(); i++) {
			auto str = stringTable[i];
			writeString(buffer, str.c_str(), str.size());
		}

		int protoSize = protoTable.size();
		writeCompressedInt(buffer, protoSize);

		for (int i = 0; i < protoSize; i++) {
			auto px = protoTable[i];
			auto p = px->p;

			writeByte(buffer, p->maxstacksize);
			writeByte(buffer, p->numparams);
			writeByte(buffer, p->nups);
			writeByte(buffer, p->is_vararg);

			int sizecode = p->sizecode;
			writeCompressedInt(buffer, sizecode);

			if (sizecode) {
				for (int i = 0; i < sizecode; i++) {
					writeInt(buffer, p->code[i]);
				}
			}

			int string_curr = 0;
			writeCompressedInt(buffer, p->sizek);

			if (p->sizek) {
				for (int i = 0; i < p->sizek; i++) {
					auto o = &p->k[i];
					switch (o->tt) {
					case LUA_TNIL: {
						writeByte(buffer, Constant_Nil);
						break;
					}
					case LUA_TBOOLEAN: {
						writeByte(buffer, Constant_Bool);
						writeByte(buffer, bvalue(o));
						break;
					}
					case LUA_TNUMBER: {
						writeByte(buffer, Constant_Number);
						writeDouble(buffer, nvalue(o));
						break;
					}
					case LUA_TSTRING: {
						writeByte(buffer, Constant_String);
						writeCompressedInt(buffer, px->strings_array[string_curr++]);
						break;
					}
					}
				}
			}

			int proto_curr = 0;
			writeCompressedInt(buffer, p->sizep);

			if (p->sizep) {
				for (int i = 0; i < p->sizep; i++) {
					writeCompressedInt(buffer, px->proto_array[proto_curr++]);
				}
			}

			writeCompressedInt(buffer, false);

			if (p->sizelineinfo) {
				writeByte(buffer, true); // use lineinfo
				writeByte(buffer, NULL); // no compression

				int lastline = 0;
				for (int i = 0; i < p->sizelineinfo; i++) {
					writeByte(buffer, 0);
				}

				for (int i = 0; i < p->sizelineinfo; i++) {
					writeInt(buffer, p->lineinfo[i]);
				}
			}

			writeByte(buffer, true); // use locvars and upvals
			writeCompressedInt(buffer, p->sizelocvars);

			if (p->sizelocvars > 0) {
				for (int i = 0; i < f->sizelocvars; i++) {
					writeCompressedInt(buffer, 1); // string offset
					writeCompressedInt(buffer, f->locvars[i].endpc);
					writeCompressedInt(buffer, f->locvars[i].startpc);
				}
			}

			writeCompressedInt(buffer, p->sizeupvalues);

			if (f->sizeupvalues > 0) {
				for (int i = 0; i < f->sizeupvalues; i++) {
					writeCompressedInt(buffer, 1); // string offset;
				}
			}
		}

		writeCompressedInt(buffer, topProto);
		lua_pop(L, 1);
	}
	else if (lua_type(L, -1) == LUA_TSTRING) {
		const char* err = lua_tostring(L, -1);

		writeByte(buffer, NULL);
		writeString(buffer, err, strlen(err));

		lua_pop(L, 1);
	}

	return buffer;
}
