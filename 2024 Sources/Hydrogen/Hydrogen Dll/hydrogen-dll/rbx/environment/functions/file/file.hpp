#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <rbx/environment/utilities/instance/instance.hpp>

#include <filesystem>

namespace rbx::environment
{
	class c_file
	{
		static inline std::filesystem::path workspace_path;

		/*
			<string> readfile(<string> path)  
			Reads the contents of the file located at path and returns it. If the file does not exist, it errors.
		*/
		static std::string readfile( std::string path, sol::this_state L );

		/*
			<void> writefile(<string> path, <string> contents)  
			Writes contents to the supplied path.
			Extensions that are not allowed: .exe, .scr, .bat, .com, .csh, .msi, .vb, .vbs, .vbe, .ws, .wsf, .wsh, .ps1, .psy. Attempting to call this function with those extensions will error.
		*/
		static void writefile( std::string path, std::string contents, sol::this_state L );

		/*
			<void> appendfile(<string> path, <string> content)  
			Appends content to the file contents at path. If the file does not exist, it errors.
		*/
		static void appendfile( std::string path, std::string contents, sol::this_state L );

		/*
			<union<function, nil>, <string?>> loadfile(<string> path) 
			Loads in the contents of a file as a chunk and returns it if compilation is successful. Otherwise, if an error has occurred during compilation, nil followed by the error message will be returned.
		*/
		static std::tuple< sol::object, std::optional< std::string > > loadfile( std::string path, sol::this_state L );

		/*
			<string?> runfile(<string> path)
			Attempts to load the file located at path and execute it on a new thread. Returns error if failed to compile.
		*/
		static std::optional< std::string > runfile( std::string path, sol::this_state L );

		/*
			<table> listfiles(<string> folder)
			Returns a table of files in folder.
		*/
		static sol::table listfiles( std::string folder, sol::this_state L );

		/*
			<bool> isfile(<string> path)
			Returns if path is a file or not.
		*/
		static bool isfile( std::string path, sol::this_state L );

		/*
			<bool> isfolder(<string> path)
			Returns if path is a folder or not.
		*/
		static bool isfolder( std::string path, sol::this_state L );

		/*
			<void> makefolder(<string> path)
			Creates a new folder at path.
		*/
		static void makefolder( std::string path, sol::this_state L );

		/*
			<void> delfolder(<string> path)
			Deletes the folder in the supplied path, if no folder exists, it errors.
		*/
		static void delfolder( std::string path, sol::this_state L );

		/*
			<void> delfile(<string> path)
			Deletes the file in the supplied path, if no file exists, it errors.
		*/
		static void delfile( std::string path, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}