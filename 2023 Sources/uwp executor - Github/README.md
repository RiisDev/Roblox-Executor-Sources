# UWP Executor

A simple script executor for the microsoft store version of Roblox.
(Not meant for actual use)

## Installation

The project and its libraries were all intended (and compiled) with the v143 platform toolset; you will need to install the latest version of Visual Studio 2022 to compile the project.

## Usage

To inject the DLL you can use [Extreme Injector](https://github.com/master131/ExtremeInjector) (or any other injection tool) with a manual map injection method. Keep in mind that the offsets change weekly and you'll have to update them yourself in [Roblox.hpp](UWP_Executor/Roblox.hpp), you can use [UWP Roblox Dumper](https://github.com/Spoorloos/uwp_roblox_dumper) or find them yourself.

## Credits

Credits to [EpixScripts/rbxcompile](https://github.com/EpixScripts/rbxcompile) for the compression method.

