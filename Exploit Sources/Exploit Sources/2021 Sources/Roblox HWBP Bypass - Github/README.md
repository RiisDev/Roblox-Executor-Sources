# Roblox-HWBP-Bypass
## Credits
* gogo1000

## What is this?
This is a bypass for Roblox's hardware breakpoint detections.

## How does this work?
Roblox check the debug registers in multiple ways to detect hwbreakpoints. First of all they use their WOW64 transition clone to call NtGetContextThread to protect it against simple API hooks. So what i do for this is basically find the transition clone and redirect their call to original function which i hook. They also have a specific thread that they set debug registers to something and then call GetThreadContext to check if they are still set to that, they basically do that to check if GetThreadContext is hooked. So what i do is hook SetThreadContext and save the flag Roblox write and return it for that specific thread when Roblox require it. They also force exception which check debug registers inside VEH then write a flag for results, so not letting their VEH run will cause kick. So i just register a VEH on top of all and clean debug registers for the specific thread the exception is created at.

## What i can use this for?
Well you can use it to hook functions as well as debug the game. If you use Cheat Engine and select VEH debugger in settings instead Windows debugger you can place breakpoints everywhere without getting kicked.
