#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <rbx/environment/utilities/instance/instance.hpp>

#include <filesystem>

namespace rbx::environment
{
	class c_debug
	{
		/*
			<table> debug.getconstants(union<function, number> fi)  
			Retrieve the constants in function fi or at level fi.
		*/
		static sol::table getconstants( sol::object fi, sol::this_state L );

		/*
			<variant> debug.getconstant(union<function, number> fi, <number> idx)  
			Set constant idx to tuple value at level or function fi.
		*/
		static sol::object getconstant( sol::object fi, std::int32_t idx, sol::this_state L );

		/*
			<void> debug.setconstant(union<function, number> fi, union<string, int> idx, union<number, bool, nil, string> value)  
			Set constant idx to tuple value at level or function fi.
		*/
		static void setconstant( sol::object fi, std::int32_t idx, sol::object value, sol::this_state L );


		/*
			<table> debug.getupvalues(union<function, number> fi)
			Retrieve the upvalues in function fi or at level fi.
		*/
		static sol::table getupvalues( sol::object fi, sol::this_state L );

		/*
			<variant> debug.getupvalue(union<function, number> fi, <number> idx)
			Returns the upvalue with name idx in function or level fi.
		*/
		static sol::object getupvalue( sol::object fi, std::int32_t idx, sol::this_state L );

		/*
			<void> debug.setupvalue(<function, number> fi, <number> idx, <table> value)  
			Set upvalue idx to value value at level or function fi.
		*/
		static void m_setupvalue( sol::object fi, std::int32_t idx, sol::object value, sol::this_state L );

		/*
			<table> debug.getprotos(union<function, number> fi)
			Returns a table containing the inner functions of function fi. Note these functions will not have upvalues, use debug.getproto with activated true to get a list of instances.
		*/
		static sol::table getprotos( sol::object fi, sol::this_state L );

		/*
			union<function, table<function>> debug.getproto(<function, number> f, <int> index, <bool?> activated)
			Gets the inner function of f at index.
			Note if activated is true, instead it will return a table of functions. These are the instances of that function that exist within the GC.
		*/
		static sol::object getproto( sol::object fi, std::int32_t idx, std::optional< bool > activated, sol::this_state L );

		/*
			<table> debug.getstack(<number> indice)
			Gets the method stack at level indice.
		*/
		static sol::object getstack( std::int32_t indice, std::optional< std::int32_t > index, sol::this_state L );

		/*
			<void> debug.setstack(<number> indice, <number> indice, <table> value)
			Set the stack indice at level indice to value value at level or function fi.
		*/
		static void setstack( std::int32_t indice, std::int32_t index, sol::object value, sol::this_state L );

		/*
			<table> debug.getinfo(union<function, number> fi)  
			Returns a table of info pertaining to the lua function fi.
		*/
		static sol::table getinfo( sol::object fi, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}