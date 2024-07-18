/*
 * File Name: EventConnection.h
 * Author: FiniteReality
 * Description: Event Connection Object
 */

#include "EventConnection.h"
#include "Event.h"

#define LOCKED_METATABLE "This metatable is locked!"

static inline void luaL__setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
	luaL_checkstack(L, nup, "too many upvalues");
	for (; l->name != NULL; l++) {  /* fill the table with given functions */
		int i;
		for (i = 0; i < nup; i++)  /* copy upvalues to the top */
			lua_pushvalue(L, -nup);
		lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
		lua_setfield(L, -(nup + 2), l->name);
	}
	lua_pop(L, nup);  /* remove upvalues */
}

namespace Engine
{
	namespace Events
	{
#define lua_event_con_name "luaL_Type_EventConnection"

		void EventConnection::Setup()
		{
			lua_State *L = XYZ_State;//EngineHandler::GetSingleton()->GetLogicSystem()->GetGlobalState();

			luaL_newmetatable(L, lua_event_con_name);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, LOCKED_METATABLE);
			lua_rawset(L, -3);

			lua_pushstring(L, "__methods");
			lua_newtable(L);
			luaL_Reg methods[]{
				{ "disconnect", lua_disconnect },
				{ NULL, NULL }
			};
			luaL__setfuncs(L, methods, 0);
			lua_rawset(L, -3);

			lua_pushstring(L, "__properties");
			lua_newtable(L);
			luaL_Reg props[]{
				{ "connected", lua_getconnected },
				{ NULL, NULL }
			};
			luaL__setfuncs(L, props, 0);
			lua_rawset(L, -3);

			lua_pushstring(L, "__tostring");
			lua_pushcfunction(L, lua_ToString);
			lua_rawset(L, -3);

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, lua_index);
			lua_rawset(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, lua_newindex);
			lua_rawset(L, -3);

			lua_pop(L, 1);
		}

		EventConnection::EventConnection(Event *creator, int reference)
		{
			this->creator = creator;
			this->reference = reference;
		}

		EventConnection::~EventConnection()
		{
		}

		void EventConnection::disconnect(lua_State *L)
		{
			if (Connected)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, reference);
				//creator->DisconnectEvent(this);
				Connected = false;
			}

		}

		int EventConnection::lua_disconnect(lua_State *L)
		{
			EventConnection *evtcon = CheckConnection(L, 1);
			evtcon->disconnect(L);

			return 0;
		}

		int EventConnection::lua_getconnected(lua_State *L)
		{
			EventConnection *PEC = CheckConnection(L, 1);
			if (PEC != nullptr)
			{
				lua_pushboolean(L, PEC->Connected);

				return 1;
			}
			return 0;
		}

		int EventConnection::lua_ToString(lua_State *L)
		{
			lua_pushstring(L, "Connection");

			return 1;
		}

		int EventConnection::lua_index(lua_State *L)
		{
			EventConnection* Connection = CheckConnection(L, 1);

			if (Connection != NULL)
			{
				const char* Name = luaL_checkstring(L, 2);

				lua_getmetatable(L, 1);
				lua_getfield(L, -1, "__properties");
				lua_getfield(L, -1, Name);

				if (lua_iscfunction(L, -1))
				{
					lua_remove(L, -2);
					lua_remove(L, -2);

					lua_pushvalue(L, 1);
					lua_call(L, 1, 1);
					return 1;
				}
				else
				{
					lua_pop(L, 2);

					lua_getfield(L, -1, "__methods");
					lua_getfield(L, -1, Name);
					if (lua_iscfunction(L, -1))
					{
						lua_remove(L, -2);
						lua_remove(L, -3);

						return 1;
					}
					else
					{
						lua_pop(L, 3);
						return luaL_error(L, "Attempt to index '%s' (a nil value)", Name);
					}
				}
			}
			return 0;
		}

		int EventConnection::lua_newindex(lua_State* L)
		{
			return luaL_error(L, "you cannot assign objects to events");
		}

		int EventConnection::WrapToLua(lua_State *L)
		{
			EventConnection** evt = (EventConnection**)lua_newuserdata(L, sizeof(*this));
			*evt = this;

			luaL_getmetatable(L, lua_event_con_name);
			lua_setmetatable(L, -2);

			return 1;
		}

		EventConnection *EventConnection::CheckConnection(lua_State *L, int reference)
		{
			return *(EventConnection**)luaL_checkudata(L, reference, lua_event_con_name);
		}
	}
}