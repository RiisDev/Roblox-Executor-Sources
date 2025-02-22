/*
 * File Name: Event.cpp
 * Author(s): Pelanyo Kamara
 * Description: Event Object
 */

#include "Events.h"
#include "MainLua.h"
#include "dllmain.h"

static inline void luaL____setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
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

#define LOCKED_METATABLE "This metatable is locked!"

static double GetTime() { 
	return (double)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

namespace Engine
{
	namespace Events
	{
		const char *Event::EVENT_KEY = "EVENTSTUFF";

		Event::Event(std::string eventName)
		{
			EventName = eventName;
			ConnectionRefs = new std::vector<int>();
			waitingObjects = new std::vector<EventWait*>();
		}

		Event::~Event() {}

		void Event::RegisterEvents(lua_State* L)
		{
			lua_pushlightuserdata(L, (void *)Event::EVENT_KEY);
			lua_newtable(L);
			lua_settable(L, LUA_REGISTRYINDEX);

			luaL_newmetatable(L, PLASTIC_EVENT_NAME);

			lua_pushstring(L, "__metatable");
			lua_pushstring(L, LOCKED_METATABLE);
			lua_rawset(L, -3);

			lua_pushstring(L, "__tostring");
			lua_pushcfunction(L, lua__tostring);
			lua_rawset(L, -3);

			lua_pushstring(L, "__index");
			lua_newtable(L);

			luaL_Reg methods[]{
				{ "connect", lua_Connect },
				{ "wait", lua__wait },
				{ NULL, NULL }
			};
			luaL____setfuncs(L, methods, 0);

			lua_rawset(L, -3);

			lua_pop(L, 1);
		}

		Event *Event::CheckEvent(lua_State *L, int idx)
		{
			void *udata = luaL_checkudata(L, 1, PLASTIC_EVENT_NAME);

			Event *connection = *((Event**)udata);

			return connection;
		}

		int Event::lua__newindex(lua_State *L)
		{
			return luaL_error(L, "you cannot assign objects to events");
		}

		int Event::lua__tostring(lua_State *L)
		{
			Event* evt = CheckEvent(L, 1);
			std::string EventName = "Signal " + evt->EventName;
			ConsoleOutput(EventName);
			lua_pushstring(L, EventName.c_str());

			return 1;
		}

		int Event::lua__wait(lua_State *L)
		{
			Event* evt = CheckEvent(L, 1);
			double Time = (double)luaL_optnumber(L, 2, 1.0 / 30.0) * 1000.0;
			EventWait *Waiter = new EventWait();
			Waiter->L = L;
			double T = GetTime();
			Waiter->ETime = T;
			evt->waitingObjects->push_back(Waiter);
			return lua_yield(L, 0);
		}

		int Event::WrapToLua(lua_State *L)
		{
			Event **udata = (Event **)lua_newuserdata(L, sizeof(this));
			*udata = this;

			luaL_getmetatable(L, PLASTIC_EVENT_NAME);
			lua_setmetatable(L, -2);

			return 1;
		}

		void Event::Fire(Event_Args pushArgs, std::vector<const void*> Info)
		{
			//luaMutex.lock();
			//Engine::EngineHandler *Handler = Engine::EngineHandler::GetSingleton();
			lua_State* G = GetGlobalState();//XYZ_State;//Handler->GetLogicSystem()->GetGlobalState();
			if (G == nullptr)
			{
				XYZ_State = luaL_newstate();

				Engine::Events::Event::RegisterEvents(XYZ_State);
				Engine::Events::EventConnection::Setup(XYZ_State);

				luaL_openlibs(XYZ_State);
				SetupLuaState(XYZ_State);
				G = XYZ_State;
			}
			lua_pushlightuserdata(G, (void *)Event::EVENT_KEY);
			lua_rawget(G, LUA_REGISTRYINDEX);
			double currentTime = GetTime();
			if(waitingObjects->size() > 0)
			{
				EventWait *W = waitingObjects->back();
				lua_State *L = W->L;

				lua_State *T = lua_tothread(G, -1);
				lua_pop(G, 1);

				lua_pushnumber(L, (currentTime - W->ETime)/1000.0);
				lua_pushnumber(L, 0);//Handler->GetElapsedTime()/1000.0);

				lua_resume(L, 2);
			}

			if (waitingObjects->size() > 0)
			{
				EventWait *W = waitingObjects->back();
				waitingObjects->pop_back();
				delete W;
			}

			for (unsigned int i = 0; i < ConnectionRefs->size(); i++)
			{
				int ref = ConnectionRefs->at(i); // get the reference
				lua_rawgeti(G, -2, ref); // get our thread info
				if (!lua_isnil(G, -1))
				{
					lua_getfield(G, -1, "thread"); // get the origin thread
					lua_State *origin = lua_tothread(G, -1);
					lua_pop(G, 1); // pop it from the global stack
					lua_getfield(G, -1, "func"); // get the function to invoke
					lua_State *T = lua_newthread(origin); // create a new thread (pushed to origin stack)
					lua_xmove(G, T, 1); // move function to thread (pops from global stack)
					int resp = lua_resume(T, pushArgs(T, Info)); // resume thread
					if (resp != 0 && resp != LUA_YIELD)
					{
						lua_pop(T, 1);
						std::cout << luaL_checkstring(T, -1) << std::endl;
					}
					lua_pop(origin, 1); // pop thread
					//origin = null;
				}
				lua_pop(G, 1); // pop thread info 
			}
			lua_pop(G, 1); // pop event table
			//Handler = null;
			//G = null;
			//luaMutex.unlock();
		}

		int Event::lua_Connect(lua_State *L)
		{
			Event *evt = CheckEvent(L, 1);
			lua_State* G = GetGlobalState();//XYZ_State;//XYZ_State;//EngineHandler::GetSingleton()->GetLogicSystem()->GetGlobalState();
			luaL_argcheck(L, lua_isfunction(L, 2), 2,
				"Lua function expected");

			lua_pushlightuserdata(G, (void *)Event::EVENT_KEY);
			lua_rawget(G, LUA_REGISTRYINDEX);

			lua_newtable(G);

			lua_xmove(L, G, 1); // func
			lua_setfield(G, -2, "func");

			lua_pushthread(L);
			lua_xmove(L, G, 1); // thread
			lua_setfield(G, -2, "thread");

			int threadRef = luaL_ref(G, -2);

			evt->ConnectionRefs->push_back(threadRef);

			EventConnection *conn = new EventConnection(evt, threadRef);
			conn->WrapToLua(L);

			return 1;
		}

#define lua_event_con_name "luaL_Type_EventConnection"

		void EventConnection::Setup(lua_State* L)
		{
			//lua_State *L = XYZ_State;//EngineHandler::GetSingleton()->GetLogicSystem()->GetGlobalState();

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
			luaL____setfuncs(L, methods, 0);
			lua_rawset(L, -3);

			lua_pushstring(L, "__properties");
			lua_newtable(L);
			luaL_Reg props[]{
				{ "connected", lua_getconnected },
				{ NULL, NULL }
			};
			luaL____setfuncs(L, props, 0);
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