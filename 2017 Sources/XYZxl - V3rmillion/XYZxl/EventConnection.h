/*
 * File Name: EventConnection.h
 * Author: FiniteReality
 * Description: Event Connection Object
 */

#pragma once

//#include "MainLua.h"
class lua_State;

namespace Engine
{
	namespace Events
	{
		class Event;

		class EventConnection
		{
			public:
				EventConnection(Event *creator, int reference);
				virtual ~EventConnection();

				static void Setup();

				static EventConnection *CheckConnection(lua_State *L, int idx);

				void registerToLua(lua_State *L);
				void disconnect(lua_State *L);
				int WrapToLua(lua_State *L);
			protected:
				static int lua_disconnect(lua_State *L);
				static int lua_getconnected(lua_State *L);
				static int lua_ToString(lua_State *L);
				static int lua_index(lua_State *L);
				static int lua_newindex(lua_State *L);
			private:
				bool Connected;
				int reference;
				Event *creator;
		};
	}
}