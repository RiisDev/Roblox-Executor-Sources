# kawaii-compiler-UWP

This is a open source roblox lua script `compiler` that can run `luau` scripts with all the lua keywords at a fast achieving rate without the use of any major `code`. The **discord server** is the same for the `UWP` edition.

This is `UWP.` UWP stands for Universal Windows Platform. It is a computing platform created by Microsoft and first introduced in Windows 10. The purpose of this platform is to help develop universal apps that run on Windows 10.

# UWP vs normal roblox?

The `UWP` edition can be found the Microsoft Store. The original **client** now has byfron released about *2* weeks ago. Each update it gets even harder to bypass. We switched to the **Microsoft Store** edition.

This will **NOT** load into the `byfron` client edition of roblox. Your *shitty* injector will return an exception. This `UWP` is 32 bit. You can't just change the target arch **from the normal kawaii compiler** and expect it to work on the `UWP` edition.

# how does it work?

During `execution`, the script is first `compiled` into bytecode, a hex-like code that can be read by the deserializer or VM before being sent to Roblox. Instead of using the Lua C API, Roblox’s error catcher is used to catch any errors and return them in the developer console. If there are no errors, the script is compressed using a compression algorithm and then passed down to VM::Load.

Each exploit uses luac, which includes basic functions like getfield, pushnil, pcall. While luac is not required for basic exploits, it is necessary for functions, custom lua environment registration, and other advanced features.

The embedded Lua C API within the dll can be updated easily if structs, addresses, or offsets are properly updated. This is not required unless you want a more advanced exploit, which this source is not designed for.

*Fun Fact: UWP has limited access to some functions like MessageBox so....the happy world of Roblox!*

# whats new?

I implemented a state of the art `inlined` functions. I have not tested them, but they are legit so it has to work (real)! There is also actual **useful** comments. There is also a teleport handler. **It** is not the best because you can use **ACTUAL** events like Teleport->TeleportInProgress but didn't feel like it.

# how to use?

Our discord server is `discord.gg/UmtdVUUvGw` and it won't expire. We will help you here.

# credits

`scheduler` http://github.com/NezyDev/

`compiler` https://github.com/roblox/luau/

`you` https://skid-axon.com/

