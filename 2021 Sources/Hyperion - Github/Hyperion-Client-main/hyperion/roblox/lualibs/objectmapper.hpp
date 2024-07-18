#pragma once
#include "../bypasses/callcheck/callcheck.hpp"

namespace hyperion::lsm 
{
	int mt_index(state ls);
	int mt_newindex(state ls);
	int mt_tostring(state ls);
	int mt_gc(state ls);
	int mt_namecall(state ls);

	struct luaStructMemberInfo {
		const char* name;
		int type;
		bool locked = false;
		TValue val;
	};

	class luaStruct {
	private:
		const char* name;
		std::vector<luaStructMemberInfo> mInfo;

	public:
		luaStruct(const char* name) : name(name) {}

		void create(const char* name, bool locked) {
			luaStructMemberInfo i;
			i.name = name;
			i.type = LUA_TNIL;
			i.locked = locked;
			mInfo.push_back(i);
		}

		void create(const char* name, int type, bool locked) {
			luaStructMemberInfo i;
			i.name = name;
			i.type = type;
			i.locked = locked;
			mInfo.push_back(i);
		}

		const char* const getName() {
			return name;
		}

		int* getType(const char* name) {
			for (int i = 0; i < mInfo.size(); i++) {
				if (std::strcmp(mInfo[i].name, name) == 0) {
					return &mInfo[i].type;
				}
			}
			return nullptr;
		}

		TValue* getValue(const char* name) {
			for (int i = 0; i < mInfo.size(); i++) {
				if (std::strcmp(mInfo[i].name, name) == 0) {
					return &mInfo[i].val;
				}
			}
			return nullptr;
		}

		void setValue(const char* name, state ls, int idx) {
			ls.push_value(idx);
			ls.setfield(LUA_REGISTRYINDEX, name);
			TValue* val = reinterpret_cast<TValue*>(ls.index2addr(idx));
			ls.settop(idx);

			for (int i = 0; i < mInfo.size(); i++) {
				if (std::strcmp(mInfo[i].name, name) == 0) {
					mInfo[i].val.value = val->value;
					mInfo[i].val.tt = val->tt;
					mInfo[i].type = val->tt;
				}
			}
		}

		bool exists(const char* name) {
			for (int i = 0; i < mInfo.size(); i++) {
				if (std::strcmp(mInfo[i].name, name) == 0) {
					return true;
				}
			}

			return false;
		}

		bool locked(const char* name) {
			for (int i = 0; i < mInfo.size(); i++) {
				if (std::strcmp(mInfo[i].name, name) == 0) {
					return mInfo[i].locked;
				}
			}

			return true;
		}

		void push(state ls) {
			hyperion::functions::push_light_userdata(ls.return_state(), this);
			hyperion::functions::create_table(ls.return_state(), 0, 0);

			ls.push_string("__index");
			hyperion::bypasses::callcheck::push_function(ls.return_state(), reinterpret_cast<uintptr_t>(mt_index), 0);
			hyperion::functions::settable(ls.return_state(), -3);

			ls.push_string("__newindex");
			hyperion::bypasses::callcheck::push_function(ls.return_state(), reinterpret_cast<uintptr_t>(mt_newindex), 0);
			hyperion::functions::settable(ls.return_state(), -3);

			ls.push_string("__tostring");
			hyperion::bypasses::callcheck::push_function(ls.return_state(), reinterpret_cast<uintptr_t>(mt_tostring), 0);
			hyperion::functions::settable(ls.return_state(), -3);

			ls.push_string("__gc");
			hyperion::bypasses::callcheck::push_function(ls.return_state(), reinterpret_cast<uintptr_t>(mt_gc), 0);
			hyperion::functions::settable(ls.return_state(), -3);

			ls.push_string("__namecall");
			hyperion::bypasses::callcheck::push_function(ls.return_state(), reinterpret_cast<uintptr_t>(mt_namecall), 0);
			hyperion::functions::settable(ls.return_state(), -3);

			hyperion::functions::setmetatable(ls.return_state(), -2);

			ls.push_value(-1);
			ls.setfield(LUA_REGISTRYINDEX, this->name);
		}
	};

	int mt_index(state ls) {
		luaStruct* t = static_cast<luaStruct*>(ls.touserdata(1));
		const char* k = ls.tostring(2);
		TValue* v = t->getValue(k);

		if (v == nullptr) {
			//luaL_error(ls, "%s is not a valid member of %s.", k, t->getName());
			return 0;
		}

		auto top = *reinterpret_cast<TValue**>(ls.return_state() + 24);

		top->value = v->value;
		top->tt = v->tt;
		*reinterpret_cast<uintptr_t*>(ls.return_state() + 24) += 16;
		
		return 1;
	}


	int mt_namecall(state ls) {
		const char* namecall = reinterpret_cast<const char*>(*reinterpret_cast<uintptr_t*>(ls.return_state() + 104) + 20);
		luaStruct* t = static_cast<luaStruct*>(ls.touserdata(1));
		TValue* v = t->getValue(namecall);

		if (v == nullptr) {
			//luaL_error(ls, "%s is not a valid member of %s.", k, t->getName());
			return 0;
		}

		auto top = *reinterpret_cast<TValue**>(ls.return_state() + 24);

		top->value = v->value;
		top->tt = v->tt;
		*reinterpret_cast<uintptr_t*>(ls.return_state() + 24) += 16;

		ls.push_value(1);

		ls.pcall(1, 1, 0);

		return 1;
	}

	int mt_newindex(state ls) {
		luaStruct* t = static_cast<luaStruct*>(ls.touserdata(1));
		const char* k = ls.tostring(2);
		TValue* v = reinterpret_cast<TValue*>(ls.index2addr(3));

		if (!t->exists(k)) {
			//luaL_error(ls, "%s is not a valid member of %s.", k, t->getName());
			return 0;
		}

		if (t->locked(k)) {
			//luaL_error(ls, "%s is a locked member of %s.", k, t->getName());
			return 0;
		}

		int* ott = t->getType(k);

		if (v->tt != *ott) {
			//luaL_error(ls, "%s expected, got %s.", luaT_typenames[*ott], luaT_typenames[v->tt]);
			return 0;
		}

		t->setValue(k, ls, 3);

		return 0;
	}

	int mt_tostring(state ls) {
		luaStruct* t = static_cast<luaStruct*>(ls.touserdata(1));
		ls.push_string(t->getName());
		return 1;
	}

	int mt_gc(state ls) {
		delete static_cast<luaStruct*>(ls.touserdata(1));
		return 0;
	}
}