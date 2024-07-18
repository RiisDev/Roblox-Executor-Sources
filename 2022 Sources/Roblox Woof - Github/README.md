# Credits
* [gogo1000](https://github.com/gogo9211)
* [0x90](https://github.com/AmJayden)
* [iivillian](https://github.com/iivillian)

# How to use
For this to work, either one of the compiled dlls must be in the same directory as RobloxPlayerBeta.exe

Do not change the name as its a necessary part of the bypass, your directory should look like this.

![](https://cdn.discordapp.com/attachments/855337473986265109/954074710788956240/unknown.png)

# How it works
This bypass sets a TLS flag before its called in order to stop roblox from getting your HWID (making it think it already has it)
It's as simple as that, however you need to be injected before Roblox can set it, which is why we made an "auto launch"

# Auto-launch
To have our code run before Roblox is fully started, we hijack a DirectX dll and "proxy" it.

This works because the precedence of LoadLibrary always checks the working directory before any system ones.

But this causes issues, LoadLibrary will return a handle to our module rather than the real one, and if it tries to call any exports (it will), we're screwed, so how do we fix that?

In order to fix the exports, we load the real dll, unlink the proxy one, and replace the export directory of our proxy with the export directory of the real module.
This allows us to give the game the real module's exports, while keeping a fake DLL, so it all works out.

# Reversal
![](https://cdn.discordapp.com/attachments/855337473986265109/954064772968620052/unknown.png)

This is the function that TLS uses to set the static variable to the HWID buffer.
If you look at the top, you can see it checks if a value is already set before running any of the below code, using that to our advantage is how we implemented our bypass.

As for what it queries, here's some screenshots of the data taken on my laptop.

![](https://cdn.discordapp.com/attachments/855337473986265109/954057957107171428/notepad_rLgBJ9mUEO.png)

![](https://cdn.discordapp.com/attachments/855337473986265109/954057692807303238/unknown.png)

![](https://cdn.discordapp.com/attachments/855337473986265109/954057624448548864/unknown.png)


Here's them querying SMBIOS:

![](https://cdn.discordapp.com/attachments/855337473986265109/954066392930471976/unknown.png)


Them querying processor info:

![](https://cdn.discordapp.com/attachments/855337473986265109/954069045773209660/unknown.png)

![](https://cdn.discordapp.com/attachments/855337473986265109/954069168536322088/unknown.png)

![](https://cdn.discordapp.com/attachments/855337473986265109/954069304888942612/unknown.png)

![](https://cdn.discordapp.com/attachments/855337473986265109/954069456349446174/unknown.png)

Here are [logged syscalls from before they kick you](https://pastebin.com/raw/UyKsQjCJ) (after you're flagged)

[Anticheat module's imports](https://pastebin.com/raw/X2PLGaF7)

[Main process' imports](https://pastebin.com/raw/Xasu3wKU)
