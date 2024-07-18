# kawaii-compiler [READ WARNING AT BOTTOM]

This is an open source project aimed at `roblox` to achieve `luau` script execution without the need of updating *major* any compiler, OP codes, or offsets.

We will help you here if you need anything: https://discord.gg/UmtdVUUvGw

# how does it work?

In the `execution`, it will first compile up the `script` into a hex-like code called `bytecode` which the deserializer or VM can read before it is actually sent to **roblox**. Since we are not using the `Lua C API`, we would use **robloxs** error catcher which catches any **errors** and returns them in the developer console. If there is **no error**, it will compress the script using a `compression algorithm`. After that, it will pass down to ``VM::Load`` and pass on from there.

# who made the compiler?

**Roblox** had actually made the compiler which can be found at https://github.com/Roblox/luau and it updates pretty much every week!

# how to update it?

We only decided to use less then 10 `addresses` and only 2 `offsets` which can be easily found. In order to find the `lua_state_top` or `lua_state_base` type in your engine `"Argument 3"` and click the first reference. It should be a `gettop` type function.

The addresses do not really have a specific explantion but you can always find some addys at https://github.com/speedstarkiwi/addy-tutorial/blob/main/addys.c

# what scripts can this run?

It won't run every single `script` out there, but it does have the same method as roblox to run scripts, but it can run `Infinite Yield, Dex Explorer, Obfuscated Scripts` and more. It has the possibility to run `Owl Hub` and `Dark Hub` if modded properly.

# does it use lua C?

Remember that each exploit uses `luac`, which are basic functions like `getfield, pushnil, pcall`. They don't require `luac` for basic exploits, but using it for functions, custom lua environment registeration, or others is required.

The `Lua C` api that is embedded with the `dll` can easily be updated if structs, addresses, or offsets are updated properly. It is not required, unless you want an actual good exploit which this source is not made to do.

# how to embed lua C?

They either can be rewritten in standard by using `__inline` and the lua structs, or you can simply just use the `VM` to call them.

# credits

`scheduler` http://github.com/NezyDev/

`compiler` https://github.com/roblox/luau/

`you` https://skid.com/

It would be really cool if you credited me if you used this source <3! (u prob wont skid)

# important!

make sure you compile in release x86 otherwise you will get `pch` errors!
![image](https://user-images.githubusercontent.com/121459772/230804253-d1680155-0e14-4aa9-a591-226ff1d69bb2.png)

if you get this `zstd.h` not found error, that looks like this!
![image](https://user-images.githubusercontent.com/121459772/230804269-e79a394d-2b2f-4ae6-9a90-db46c14b80bd.png)
paste this include code on top of the `dllmain.cpp`
```
#include "zstd/include/zstd.h"
```

# important(BYFRON!)

So `byfron` was released. This encrypts everything. **Kawaii compiler** does not work as of right now. You will need a **BYFRON** bypass.

# edits

4/20 - switched to `getstate` instead of lua state.
