# Occult Virtual Machine (OVM)
Research project coordinated by select Rain members aimed at developing a new way to perform script execution on Roblox (as older ways such as proto conversion and bytecode conversion are outdated and difficult to update). The Occult Virtual Machine (OVM) is in heavy development and isn't stable enough to run entire lua scripts (yet). Another exploit similar to OVM exists, named IVM (Intriga Virtual Machine). We're collaborating together so we don't become Jordan v2.

## How the OVM works
The Occult Virtual Machine is a raw, cut-down lua virtual machine that operates on a foreign stack rather than its own to ensure compatibility with functions exposed in another (different) lua environment. Due to the design of the Occult VM, little to no security checks running on the Roblox client need to be circumvented by the exploit, and any functions "protected" by Roblox's security can easily be copied over to our own software with the security parts removed (which is totally awesome since we don't have to refind those functions each time Roblox pushes an update).

## Comparison between Seven/Elysian and OVM/IVM
**Seven/Elysian**:
* Functions are ran on Roblox's lua environment (function conversion needed due to changes between our and their env)
* Many security features such as the memory integrity checker (code change detection) and the return checker needs to be circumvented to avoid detection
* A lot of elements (memory offsets, functions, keys, etc) need to be updated individually each time Roblox pushes an update, which takes a lot of time and work
* Quite easy to patch (changing how the VM works can break both exploits very easily)
* Mostly developed by a single individual with little to no organization. Development rate is slow.

**OVM/IVM**:
* Functions are ran on the Occult virtual machine (no conversion needed)
* Little to no security features need to be circumvented as we almost never touch Roblox's memory
* Less elements need to be updated than conventional exploits as we rarely use Roblox's own code
* Very hard to patch (Roblox will need to resort to physical detection)
* Codeveloped with competent developers and organized. Working as a team is always good.

## To-Do (subject to change)

###Exploit
- [x] Rename "Last Occult" (LO) to "Occult Virtual Machine" (OVM, trying to get a Nyx/Alx feel here. IVM/OVM)
- [x] Find the new OVM logo 
- [ ] Implement the whitelist
- [ ] Make our functions secure (import vmp libs)
- [ ] OPTIONAL: Merge with IVM if everything goes awry

###Virtual Machine
- [x] Develop the VM's frame
- [x] Implement necessary opcodes to execute C functions (confirmed working)
- [x] Implement basic stack management logic between VM jumps
- [x] Modify our garbage collector so it works on foreign states (roblox takes care of it already)
- [x] Set rL->savedpc to a pointer to our PC register when necessary (jumps, return values, etc)
- [x] Implement OP_VARARG (preferrably for tuple functions)
- [ ] Add proper error handling (set rL->errorJmp/rL->errfunc to our error handling functions) to redirect errors thrown by Roblox to user-set error handlers (pcall, xpcall, ypcall, etc)
- [ ] Replace roblox's pcall/xpcall/ypcall with our own (error handling purposes)

###Script/API (add funcs here if you have ideas)
- [x] Implement loadstring
- [x] Unlock instance metatables
- [ ] Implement SaveInstance (if it still works)
- [ ] Add DataModel::GetObjects (instance mts unlocking needed)

###Calling
- [x] C function calling
- [x] C Return values handling
- [ ] Replace the fucking horrible lua_pushcclosure with just newcclosure (why are we copying pushcclosure anyway)
- [ ] When the above is done, get the return values to work

###Tables
- [x] Implement table instructions (SETTABLE, GETTABLE, SETGLOBAL, GETGLOBAL, etc)
- [x] Implement handling of metatables (__index, __newindex) (idea: make the vm detect if it's one of our tables)

###Thread scheduler
NOTE: We might not reimplement the thread scheduler and use Roblox's instead
- [ ] Reimplement the thread scheduler (wait, spawn, delay)
- [ ] Add a lua interface to the thread scheduler (replace the thread's wait/spawn/delay funcs with ours)
- [ ] Set up events (we'll figure how soon)
