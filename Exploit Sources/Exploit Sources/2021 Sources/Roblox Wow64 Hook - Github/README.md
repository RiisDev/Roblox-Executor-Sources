# Roblox-Wow64-Hook
## Credits
* gogo1000
* 0x90

## What is this?
This is a bypass for Roblox's WOW64 transition checks.
It also include example hook.

## How does this work?
Roblox does integrity checks of the gate to check if its hooked and also clone it when the game starts for important syscalls.
What we do here is hook the function which integrity check the gate and also redirect their cloned gate to the original gate.

I included example NtQueryVirtualMemory hook with it, which basically allows you to change page protections inside .text without getting kicked
