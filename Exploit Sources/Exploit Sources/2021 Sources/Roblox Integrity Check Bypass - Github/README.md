# Roblox-Integrity-Check-Bypass
## Credits
* gogo1000
* iivillian
* 0x90

## What is this?
This is a bypass for the game Roblox's integrity check (dubbed memcheck).
Allowing you to write memory in the .text and .vmp0 segments.

## How does this work?
This bypass clones the .text and .vmp0 segments, collects all silent checkers, then redirects all existing ones to a selective one.


Then hooks the core hasher, with our detour being a recreation of the hasher, which loads our spoofed address (hashed address translated to our clone address) and replaces all references of the real hash to ours.
