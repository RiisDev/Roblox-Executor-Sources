# [URS] Roblox Remote Spy
URS is a C++ Roblox remote spy. It is compiled as a DLL and injected into the Roblox process.

## What does it do?
URS logs [RemoteEvent](https://developer.roblox.com/en-us/api-reference/class/RemoteEvent) and [RemoteFunction](https://developer.roblox.com/en-us/api-reference/class/RemoteFunction) calls made by the Roblox client. ([FireServer](https://developer.roblox.com/en-us/api-reference/function/RemoteEvent/FireServer) and [InvokeServer](https://developer.roblox.com/en-us/api-reference/function/RemoteFunction/InvokeServer))

## How does it work?
URS applies an integrity check bypass (known as a memcheck bypass in the Roblox community) and hooks the FireServer and InvokeServer methods. Both methods have a [__thiscall](https://docs.microsoft.com/en-us/cpp/cpp/thiscall?view=msvc-170) calling convention. It applies a trampoline hook on FireServer and a different, but similar, type of hook on InvokeServer. We used a different type of hook instead of a trampoline hook on InvokeServer because allowed us to not have to find out the parameters of the function. After hooking, it reads through the call arguments and their types.

### Example

```lua
game:GetService('ReplicatedStorage').RemoteEvent:FireServer(
	'abc',
	1,
	true,
	false,
	Vector2.new(1, 2),
	Vector3.new(3, 4, 5),
	CFrame.new(6, 7, 8),
	Color3.new(1, 1, 0.5),
	Ray.new(Vector3.new(9, 10, 11), Vector3.new(12, 13, 14)),
	game.Workspace,
	Enum.KeyCode.E,
	{'def'}
);
```

![Image1](https://i.gyazo.com/a6e9b28ca57505b370fb05c216a9f512.png)

![enter image description here](https://i.gyazo.com/fafe693bf4f2b52716e03f3564ca9ab5.png)
