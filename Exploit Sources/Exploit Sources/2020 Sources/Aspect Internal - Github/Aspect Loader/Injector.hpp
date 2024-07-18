#pragma once
///-------------------------------------------------------------------------------------------------
/// Author:             ReactiioN
/// Created:            11.05.2017
///
/// Last modified by:   ReactiioN
/// Last modified on:   11.05.2017
///-------------------------------------------------------------------------------------------------
/// Copyright(c) ReactiioN <https://reactiion.pw>. All rights reserved.
///-------------------------------------------------------------------------------------------------
#pragma once
/// [url]https://msdn.microsoft.com/de-de/library/windows/desktop/ff818516(v=vs.85).aspx[/url]
#include <Windows.h>
/// [url]https://msdn.microsoft.com/de-de/library/windows/desktop/ms686701(v=vs.85).aspx[/url]
#include <TlHelp32.h>
/// [url]http://en.cppreference.com/w/cpp/header/string[/url]
#include <string>
/// [url]http://en.cppreference.com/w/cpp/header/unordered_map[/url]
#include <unordered_map>

///-------------------------------------------------------------------------------------------------
/// <summary>   A win32/64 .dll injector. </summary>
///
/// <remarks>   ReactiioN, 11.05.2017. </remarks>
///-------------------------------------------------------------------------------------------------
class Injector
{
    /// <summary>
    /// Information describing the proc.
    /// </summary>
    using ProcInfo = std::unordered_map<std::string, PROCESSENTRY32>;

    /// <summary>
    /// Information describing the module.
    /// </summary>
    using ModuleInfo = std::unordered_map<std::string, MODULEENTRY32>;

public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Default constructor. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///-------------------------------------------------------------------------------------------------
    Injector() = default;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Injects the given file. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="filename"> Filename of the file. </param>
    ///
    /// <returns>   True if it succeeds, false if it fails. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool                    inject(const std::string& filename);

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Injects. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="pid">      The PID. </param>
    /// <param name="filename"> Filename of the file. </param>
    ///
    /// <returns>   True if it succeeds, false if it fails. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool                    inject(const uint32_t pid, const std::string& filename);

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Determines if we can parse process modules. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <returns>   True if it succeeds, false if it fails. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool                     parse_process_modules();

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Searches for the first target process. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="process_name"> Name of the process. </param>
    ///
    /// <returns>   True if it succeeds, false if it fails. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool                    find_target_process(const std::string& process_name);

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Gets loaded modules. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <returns>   The loaded modules. </returns>
    ///-------------------------------------------------------------------------------------------------
    inline const ModuleInfo& get_loaded_modules() const;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Parse loaded modules. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <returns>   A ModuleInfo. </returns>
    ///-------------------------------------------------------------------------------------------------
    ModuleInfo               parse_loaded_modules() const;

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Ejects the given module name. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="module_name">  Name of the module. </param>
    ///-------------------------------------------------------------------------------------------------
    void                     eject(const std::string& module_name);

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Parse running proccesses. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <returns>   A ProcInfo. </returns>
    ///-------------------------------------------------------------------------------------------------
    static ProcInfo          parse_running_proccesses();

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Parse loaded modules. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="pid">  The PID. </param>
    ///
    /// <returns>   A ModuleInfo. </returns>
    ///-------------------------------------------------------------------------------------------------
    static ModuleInfo        parse_loaded_modules(const uint32_t pid);

    ///-------------------------------------------------------------------------------------------------
    /// <summary>   Gets directory file path. </summary>
    ///
    /// <remarks>   ReactiioN, 11.05.2017. </remarks>
    ///
    /// <param name="file"> The file. </param>
    ///
    /// <returns>   The directory file path. </returns>
    ///-------------------------------------------------------------------------------------------------
    static std::string       get_directory_file_path(const std::string& file);

private:
    /// <summary>
    /// Identifier for the process.
    /// </summary>
    uint32_t   m_processId = 0;
    /// <summary>
    /// The function of LoadLibrarA.
    /// </summary>
    uintptr_t  m_loadLibrary = 0;
    /// <summary>
    /// The loaded modules.
    /// </summary>
    ModuleInfo m_loadedModules;
};

inline const Injector::ModuleInfo& Injector::get_loaded_modules() const
{
    return m_loadedModules;
}

///-------------------------------------------------------------------------------------------------
/// End of injector.hpp
///-------------------------------------------------------------------------------------------------
