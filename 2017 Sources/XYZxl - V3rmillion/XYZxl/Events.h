/*
 * File Name: Event.h
 * Author(s): Pelanyo Kamara
 * Description: Event Object
 */

#pragma once

#define PLASTIC_EVENT_NAME "Event"

#include <vector>
#include <string>
//#include "MainLua.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "luaconf.h"
#include "lualib.h"
}

//class lua_State;

namespace Engine
{
	namespace Events
	{
		struct EventWait
		{
			lua_State *L;
			double ETime;
		};

		typedef int (Event_Args)(lua_State* L, std::vector<const void*> Info);

		class Event
		{
			public:
				Event(std::string Name);
				virtual ~Event();

				static void RegisterEvents(lua_State* L);

				void Fire(Event_Args Args, std::vector<const void*>);

				static Event *CheckEvent(lua_State *L, int idx);
				int WrapToLua(lua_State *L);

				std::string EventName;
			protected:
				static int lua_Connect(lua_State *L);

				static int lua__newindex(lua_State *L);
				static int lua__tostring(lua_State *L);
				static int lua__wait(lua_State *L);
			private:
				std::vector<int> *ConnectionRefs;
				static const char *EVENT_KEY;

				std::vector<EventWait*> *waitingObjects;
		};

		class EventConnection
		{
			public:
				EventConnection(Event *creator, int reference);
				virtual ~EventConnection();

				static void Setup(lua_State* L);

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