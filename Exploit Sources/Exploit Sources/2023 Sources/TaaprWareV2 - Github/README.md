NEW WORKING VERSION: https://github.com/plusgiant5/TaaprWareV3

# TaaprWare V2

WARNING: This code is very bad and I don't know if it still works anymore. Future exploits by me will be nothing like this. It's not updated, but it's possible to update (even following the recent compression changes).

This is an open source Roblox executor with a custom DLL made in C++, and a UI made in C#.
It features level 8 execution (or whatever level you want), custom functions, metamethod hooking (breaks corescripts), multi Roblox injection, an autoexec folder, and a workspace folder.

It's not very secure though. With the init script, it's easily detectable because of the insecure hook that doesn't use newcclosure, but on the C++ side it's immune to simple detection methods that I though of while writing it. It doesn't trigger any 268/unexpected client behavior kicks, at least not at the time I'm writing this.

Discord (better exploits soon): https://discord.gg/nAEHrW9EF9

# How to build

DLL: Set to Release x86 and turn off precompiled headers

UI: Add reference to the AvalonEdit dll in the Release folder

# HttpGet/GetObjects hook

Because of the way custom functions work, hooking metamethods causes a bunch of errors in the output which is annoying. If you want game:HttpGet and game:GetObjects capabilities and can handle the error spam, uncomment line 1380 in InitScript.lua.

# About

This entire exploit started as testing grounds for me to learn more, and it definitely helped me learn. It helped me get from C++ hello world programs to real Luau function recreation for Roblox and finding offsets, so expect it to be messy.
It heavily relies on the Lua init script to function properly, and it uses RuntimeScriptService::RunScript to execute. This execution method was annoying to work with as there is no lua state involved. I had to hook a terrain function and have the Lua code call that function with arguments that tell the C++ code what to do with the lua state. It was a good learning experience, but I will not be using this execution method again.

With a few edits, you can get execution working with only these addresses and no offets: rbx_getscheduler, rbx_addscript, rbx_runscript, rbx_deserializer_detour and rbx_deserializer_detour2
Or, for the bare minimum (localscript level, no custom bytecode) rbx_getscheduler and rbx_runscript.

The framework of custom functions was also done with very few addresses, needing only rbx_writevoxels, rL->top, and rL->base.

This exploit is good for beginners who want to experiment with custom functions without finding every offset in existence.

If you need help with addresses or offsets, join the Discord and I will help you.
