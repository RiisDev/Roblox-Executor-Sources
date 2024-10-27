#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <rbx/environment/utilities/instance/instance.hpp>

namespace rbx::environment
{
	inline sol::userdata hui;

	class c_env
	{
		/*
			<table> getgenv( <void> )
			Returns the environment that will be applied to each script ran by Hydrogen.
		*/
		static std::int32_t getgenv( lua_State* L );
		
		/*

			<table> getrenv(<void>) 
			Returns the global environment for the LocalScript state.
		*/
		static std::int32_t getrenv( lua_State* L );

		/*
			<table> getreg(<void>)
			Returns the Lua registry.
		*/
		static std::int32_t getreg( lua_State* L );

		/*
			<table> getgc( <bool ? > include_tables )
			Returns all functions and userdata values within the GC.Passing true will also return tables.
		*/
		static sol::object getgc( sol::optional< bool > include_tables, sol::optional< bool > include_threads, sol::this_state L );

		/*
		    <table<Instance>> getinstances(<void>)
			Returns a list of all instances within the game.
		*/
		static std::int32_t getinstances( lua_State* L );

		/*
			<table<Instance>> getnilinstances(<void>)
			Returns a list of all instances parented to nil within the game.
		*/
		static std::int32_t getnilinstances( lua_State* L );

		/*
			<table<union<LocalScript, ModuleScript>>> getscripts(<void>)
			Returns a list of all scripts within the game.
		*/
		static std::int32_t getscripts( lua_State* L );

		/*
		    <table<ModuleScript>> getloadedmodules(<bool ? > dont_filter)
			Returns all ModuleScripts loaded in the game.
		*/
		static sol::object getloadedmodules( sol::optional< bool > dont_filter, sol::this_state L );

		/*
			<table<ModuleScript, LocalScript>> getrunningscripts()
			Returns all Scripts loaded in the game.
		*/
		static sol::object getrunningscripts( sol::this_state L );

		/*
			<table<Connection>> getconnections(<ScriptSignal> obj)  
			Gets a list of connections to the specified signal.
		*/
		static sol::object getconnections( script_signal_t obj, sol::this_state L );

		/*
			<void> firesignal(<ScriptSignal> Signal, <variant> Args...)
			Fires all the connections connected to the signal Signal with Args.
		*/
		static sol::object firesignal( script_signal_t obj, sol::variadic_args va, sol::this_state L );

		/*
			<void> defersignal(<ScriptSignal> Signal, <variant> Args...)
			Defers all the connections connected to the signal Signal with Args.
		*/
		static sol::object defersignal( script_signal_t obj, sol::variadic_args va, sol::this_state L );

		/*
			<void> fireclickdetector(<ClickDetector> Detector, <number> Distance)
			Fires the designated ClickDetector with provided Distance. If Distance isn't provided, it will default to 0.
		*/
		static void fireclickdetector( click_detector_t obj, sol::optional< double > duration, sol::this_state L );

		/*
			<Number> getproximitypromptduration( <ProximityPrompt> Prompt )
			Fires the designated ProximityPrompt.
		*/
		static float getproximitypromptduration( proximity_prompt_t prompt, sol::this_state L );

		/*
			<void> setproximitypromptduration( <ProximityPrompt> Prompt, <Number> Duration )
			Fires the designated ProximityPrompt.
		*/
		static void setproximitypromptduration( proximity_prompt_t prompt, double duration, sol::this_state L );

		/*
			<void> fireproximityprompt( <ProximityPrompt> Prompt )
			Fires the designated ProximityPrompt.
		*/
		static void fireproximityprompt( proximity_prompt_t prompt, sol::this_state L );

		/*
		    <void> firetouchinterest(<Instance> Part, <BasePart> ToTouch, <uint> Toggle)
			Fakes a .Touched event to ToTouch with Part. The Toggle argument must be either 0 or 1 (for fire/un-fire).
		*/
		static void firetouchinterest( base_part_t part, base_part_t to_touch, std::uint32_t toggle, sol::this_state L );

		/*
			<bool> isnetworkowner(<BasePart> Part)
			Returns true if the Part is owned by the player.
		*/
		static bool isnetworkowner( base_part_t part, sol::this_state L );

		/*
			<bool> setscriptable(<Instance> obj, <string> property, <bool> state)
			Sets whether the property is accessible or not from lua.
		*/
		static bool setscriptable( instance_t obj, std::string prop, bool state, sol::this_state L );

		/*
		 	<void> isscriptable(<Instance> obj, <string> property)
			Returns true if the property is accessible from lua.
		*/
		static bool isscriptable( instance_t obj, std::string prop, sol::this_state L );

		/*
			<variant> gethiddenproperty(<Instance> Object, <string> Property)
			Returns the hidden property Property from Object. Errors if the property does not exist.
		*/
		static std::pair< sol::object, bool > gethiddenproperty( instance_t obj, std::string_view prop, sol::this_state L );

		/*
			<bool> sethiddenproperty(<Instance> Object, <string> Property, <variant> Value)  
			Sets the hidden property Property with Value from Object. Errors if the property does not exist.
		*/
		static bool sethiddenproperty( instance_t obj, std::string_view prop, sol::object value, sol::this_state L );

		/*
			<void> setsimulationradius(<uint> SimulationRadius, <uint?> MaxSimulationRadius)  
			Sets the player's SimulationRadius. If MaxSimulationRadius is specified, it will set that as well.
		*/
		static void setsimulationradius( double value, sol::optional<double> max_value, sol::this_state L );

		/*
			<string> getcustomasset(<string> path)
			Returns a rbxasset:// content id for the asset located at path, allowing you to use unmoderated assets. Internally, files are copied to the game's content directory.
		*/
		static std::string getcustomasset( std::string path, sol::this_state L );

		/*
			<std::string, std::string> identifyexecutor(<void>)
			Returns the name and version of the current executor.
		*/
		static std::tuple< std::string, std::string > identifyexecutor( sol::this_state L );

		/*
			<Folder gethui(<void>)
			Returns a hidden GUI container. Should be used as an alternative to CoreGui and PlayerGui.
		*/
		static std::int32_t gethui( lua_State* L );

		/*
			<uint> getthreadidentity(<void>)  
			Returns the current thread identity.
		*/
		static std::uint32_t getthreadidentity( sol::this_state L );

		/*
			<uint> setthreadidentity(<uint> m)
			Sets the current thread identity.
		*/
		static void setthreadidentity( std::uint32_t n, sol::this_state L );

		/*
			<void> require(<variant> arg)
			Rquires a modulescript.
		*/
		static sol::object require( sol::object arg, sol::this_state L );

		static std::int32_t httpget( lua_State* L );

		static std::int32_t httpgetasync( lua_State* L );

		static std::int32_t request( lua_State* L );

		static void __fastcall on_function_call( std::uint8_t* function_slot_wrapper, void* edx, void* args );
		static void __fastcall set_generic_callback_helper( void* callback_descriptor, void* edx, void* object, void* function );

		static inline decltype( &on_function_call ) m_on_function_call;
		static inline decltype( &set_generic_callback_helper ) m_set_generic_callback_helper;
	public:
		static std::int32_t on_index_bridge( lua_State* L );
		static std::int32_t on_namecall_bridge( lua_State* L );
		static std::int32_t on_newindex_bridge( lua_State* L );

		static inline decltype( &on_index_bridge ) m_on_index_bridge;
		static inline decltype( &on_namecall_bridge ) m_on_namecall_bridge;
		static inline decltype( &on_newindex_bridge ) m_on_newindex_bridge;

		void init( lua_State* L ) const;
	};
}