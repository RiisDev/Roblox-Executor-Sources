-- Credit: https://v3rmillion.net/showthread.php?tid=1075607
-- Yielding in C functions freezes the game until done. I will solve this in another project.

local doinit = true
if doinit then

local supported = {
    "request",
    "newlclosure",
    "newcclosure",
    "getsupportedfunctions",
    "isluau",
    "istaaprclosure",
    "isexecutorclosure",
    "getexecutorname",
    "getexecutorversion",
    "identifyexecutor",
    "fireproximityprompt",
    "clonefunction",
    "getcallingscript",
    "getobjects",
    "httpget",
    "gamecrash",
    "mouse1down",
    "mouse1up",
    "mouse1click",
    "mouse2down",
    "mouse2up",
    "mouse2click",
    "keydown",
    "keyup",
    "keyclick",
    "getsenv",
    "getmenv",
    "getgenv",
    "getrenv",
    "getreg",
    "loadstring",
    "getrunningscripts",
    "getmodulescripts",
    "getloadedmodules",
    "getscripts",
    "checkcaller",
    "randomstring",
    "getclipboard",
    "setclipboard",
    "readfile",
    "isfile",
    "listfiles",
    "writefile",
    "makefolder",
    "appendfile",
    "isfolder",
    "delfile",
    "delfolder",
    "loadfile",
    "dofile",
    "getfpscap",
    "setfpscap",
    "getrenderproperty",
    "setrenderproperty",
    "getnilinstances",
    "messagebox",
    "rconsoleprint",
    "rconsolewarn",
    "rconsoleerr",
    "rconsoleinfo",
    "rconsolesettitle",
    "rconsolecreate",
    "rconsoledestroy",
    "isrbxactive",
    "mousemoveabs",
    "mousemoverel",
    "mousescroll",
    "getidentity",
    "setidentity",
    "isreadonly",
    "setreadonly",
    "hookmetamethod",
    "getrawmetatable",
    "setrawmetatable",
    "getnamecallmethod",
    "setnamecallmethod",
    "iscclosure",
    "islclosure",
    "getsimulationradius",
    "setsimulationradius",
}
local unique = {
    "Self explanatory: listfolders(path: string) -> table",
    "Self explanatory: getexecutorversion() -> string",
    "Self explanatory: istaaprclosure() -> boolean",
    "New Lua closure: newlclosure() -> function",
    "Self explanatory: getsupportedfunctions() -> table",
    "Self explanatory: getuniquefunctions() -> table",
    "Self explanatory: gamecrash()",
    "Self explanatory: randomstring(length: number?) -> string",
    "Returns the address of the current lua state being used for the execution thread, as an integer: getthread() -> number",
    "Returns the address of the TValue passed as an argument, as an integer: getaddress(TValue: any) -> number",
    "Returns the address of the Instance passed as an argument, as an integer: getinstanceaddress(inst: Instance) -> number",
    "Creates an int3 exception in the DLL: __debugbreak()",
    "Converts an integer to hexadecimal, represented as a string: tohex(num: number) -> string",
    "Directly calls lua_gettop: gettop() -> number",
    "Directly calls lua_settop: settop(top: number)",
    "Directly gets ts->hash: gethash(ts: string) -> number",
    "Directly sets ts->hash: sethash(ts: string, h: number)",
    "Calls the internal print function: rawprint(type: number, text: string)",
	"Same as hookmetamethod, but handles storing and calling the old function for you (RETURN VALUE OF YOUR FUNCTION IS IGNORED, USE FOR LOGGING ONLY): safehookmetamethod(object: table, method: string, func: function)",
	"Returns your HWID, hashed with MD5: gethwid() -> string",
	"An addition to the table library that is the opposite of table.freeze: table.unfreeze/table.thaw(tab: table)",
}

local ___hwidkey___ = "no"
local terrain = workspace.FindFirstChildOfClass(workspace, "Terrain")
local WriteVoxels = terrain.WriteVoxels
local function __send_to_C(message, ...)
	-- no metamethods allowed
	local returns = {WriteVoxels(terrain, ___hwidkey___ or "keynil", message, ...)}
    if returns[1] == 9992 and type(returns[2]) == "string" then -- secret error code
        error(returns[2])
    else
        return unpack(returns)
    end
end
___hwidkey___ = __send_to_C("gethwid")
randomstring = function(length)
    local str = ""
    for _ = 1, length or 20 do
        local type = math.random(1,3)
        if type == 1 then
            str = str .. string.char(math.random(97,122)) -- Lowercase
        elseif type == 2 then
            str = str .. string.char(math.random(65,90)) -- Uppercase
        elseif type == 3 then
            str = str .. string.char(math.random(48,57)) -- Numbers
        end
    end
    return str
end
local fakescript = Instance.new("LocalScript")
fakescript.Name = randomstring()
script = fakescript
function getscripts()
	local a = {}
	for b, c in next, getreg() do
		if type(c) == "table" then
			for d, e in next, c do
				if typeof(e) == "Instance" and (e:IsA("LocalScript") or e:IsA("ModuleScript")) then
					table.insert(a, e)
				end
			end
		end
	end
	return a
end
function getmodulescripts()
    local scripts = {}
    for _, script in ipairs(getreg()) do
        if script:IsA("ModuleScript") then
            scripts[#scripts + 1] = script
        end
    end
    return scripts
end
getmodules = getmodulescripts
function getrunningscripts()
    local scripts = {}
    for _, script in ipairs(game:GetService("Players").LocalPlayer:GetDescendants()) do
        if script:IsA("LocalScript") or script:IsA("ModuleScript") then
            scripts[#scripts + 1] = script
        end
    end
    return scripts
end
function getsupportedfunctions()
    return supported
end
function getuniquefunctions()
    return unique
end
function getexecutorname()
    return "TaaprWare"
end
function getexecutorversion()
    return "V2.2.0"
end
function identifyexecutor()
    return getexecutorname(), getexecutorversion()
end
local GetService = game.GetService
function checkcaller()
	return getidentity() > 3
end
istaaprclosure = checkcaller
isexecutorclosure = checkcaller
function islclosure(func)
	assert(type(func) == "function", "invalid argument #1 to 'islclosure' (function expected, got " .. type(func) .. ") ", 2)
	local success = pcall(function()
		return setfenv(func, getfenv(func))
	end)
	return success
end
function iscclosure(func)
	assert(type(func) == "function", "invalid argument #1 to 'iscclosure' (function expected, got " .. type(func) .. ") ", 2)
    return not islclosure(func)
end
VirtualInputManager = game:GetService("VirtualInputManager")
local SendMouseButtonEvent = VirtualInputManager.SendMouseButtonEvent
local SendKeyEvent = VirtualInputManager.SendKeyEvent
Mouse = game:GetService("Players").LocalPlayer
function mouse1down() SendMouseButtonEvent(VirtualInputManager, Mouse.X, Mouse.Y, 0, true, nil, 0) end
mouse1press = mouse1down
function mouse1up() SendMouseButtonEvent(VirtualInputManager, Mouse.X, Mouse.Y, 0, true, nil, 0) end
mouse1release = mouse1up
function mouse1click(downtime) mouse1down() task.wait(downtime or 0) mouse1up() end
function mouse2down() SendMouseButtonEvent(VirtualInputManager, Mouse.X, Mouse.Y, 1, false, nil, 0) end
mouse2press = mouse2down
function mouse2up() SendMouseButtonEvent(VirtualInputManager, Mouse.X, Mouse.Y, 1, false, nil, 0) end
mouse2release = mouse2up
function mouse2click(downtime) mouse2down() task.wait(downtime or 0) mouse2up() end
function presskey(keycode) SendKeyEvent(VirtualInputManager, true, keycode, false, workspace) end
keypress = presskey
keydown = presskey
function releasekey(keycode) SendKeyEvent(VirtualInputManager, false, keycode, false, workspace) end
keyrelease = releasekey
keyup = releasekey
function clickkey(downtime) presskey() task.wait(downtime or 0) releasekey() end
keyclick = clickkey
function request(options)
	assert(type(options) == "table", "invalid argument #1 to 'request' (table expected, got " .. type(options) .. ") ", 2)
	local Event = Instance.new("BindableEvent")
	local RequestInternal = game:GetService("HttpService").RequestInternal
	local Request = RequestInternal(game:GetService("HttpService"), options)
	local Start = Request.Start
	local Response
	Start(Request, function(state, response)
		Response = response
		Event:Fire()
	end)
	Event.Event:Wait()
	return Response
end
function httpget(url)
	assert(type(url) == "string", "invalid argument #1 to 'httpget' (string expected, got " .. type(url) .. ") ", 2)
    local response = request({
        Url = url;
        Method = "GET";
    }).Body
	task.wait() -- the request function likes to consistently return at an extremely inconvenient time when the WriteVoxels hook is reverted, so this waits for another scheduler cycle
	return response
end
function newlclosure(f)
	assert(type(f) == "function", "invalid argument #1 to 'newlclosure' (function expected, got " .. type(f) .. ") ", 2)
    return function(...)
        return f(...)
    end
end
isluau = function()
	return _VERSION == "Luau"
end
clonefunction = function(p1)
	assert(type(p1) == "function", "invalid argument #1 to 'clonefunction' (function expected, got " .. type(p1) .. ") ", 2)
	local A = p1
	local B = xpcall(setfenv, function(p2, p3)
		return p2, p3
	end, p1, getfenv(p1))
	if B then
		return function(...)
			return A(...)
		end
	end
	return coroutine.wrap(function(...)
		while true do
			A = coroutine.yield(A(...))
		end
	end)
end
fireproximityprompt = function(Obj, Amount, Skip)
	assert(typeof(Obj) == "Instance", "invalid argument #1 to 'fireproximityprompt' (ProximityPrompt expected, got " .. type(Spoof) .. ") ")
	assert(Obj.ClassName == "ProximityPrompt", "invalid argument #1 to 'fireproximityprompt' (ProximityPrompt expected, got " .. type(Spoof) .. ") ")
    assert(type(Amount) == "number", "invalid argument #2 to 'fireproximityprompt' (number expected, got " .. type(Amount) .. ") ", 2)
	Amount = Amount or 1
    local PromptTime = Obj.HoldDuration
    if Skip then
        Obj.HoldDuration = 0
    end
    for i = 1, Amount do
        Obj:InputHoldBegin()
        if not Skip then
            wait(Obj.HoldDuration)
        end
        Obj:InputHoldEnd()
    end
    Obj.HoldDuration = PromptTime
end
function getidentity()
    return __send_to_C("getidentity")
end
function setidentity(new_identity)
    assert(type(new_identity) == "number", "invalid argument #1 to 'setidentity' (number expected, got " .. type(new_identity) .. ") ", 2)
    return __send_to_C("setidentity", new_identity)
end
function securecall(Closure, Spoof, ...)
    assert(type(Closure) == "function", "invalid argument #1 to 'securecall' (function expected, got " .. type(Closure) .. ") ", 2)
	assert(typeof(Spoof) == "Instance", "invalid argument #2 to 'securecall' (LocalScript or ModuleScript expected, got " .. type(Spoof) .. ") ")
	assert(Spoof.ClassName == "LocalScript" or Spoof.ClassName == "ModuleScript", "invalid argument #2 to 'securecall' (LocalScript or ModuleScript expected, got " .. Spoof.ClassName .. ") ")
	local OldScript = getfenv().script
	local OldThreadID = getidentity()
	getfenv().script = Spoof
	setidentity(2)
	local Success, Err = pcall(Closure, ...)
	setidentity(OldThreadID)
	getfenv().script = OldScript
	if not Success and Err then
		error(Err)
	end
end
function getcallingscript(lvl)
	--[[lvl = lvl and lvl + 1 or 1
	local func = setfenv(lvl, getfenv(lvl))
	return getfenv(func).script]]
	return getfenv(0).script
end
secure_call = securecall
http_request = request
http = {
    request = request;
}
getobjects = function(k)
    assert(type(k) == "string", "invalid argument #1 to 'getobjects' (string expected, got " .. type(k) .. ") ", 2)
	return {
		game:GetService("InsertService"):LoadLocalAsset(k)
	}
end
gamecrash = function()
    while true do end
end
bit = bit32
getgenv = function()
    return __send_to_C("getgenv")
end
gettop = function()
    return __send_to_C("gettop")
end
settop = function(top)
	assert(type(top) == "number", "invalid argument #1 to 'setclipboard' (number expected, got " .. type(top) .. ") ", 2)
    return __send_to_C("settop", top)
end
__debugbreak = function()
    return __send_to_C("__debugbreak")
end
gethwid = function()
    return __send_to_C("gethwid")
end
getreg = function()
    return __send_to_C("getreg")
end
getrenv = function()
    return __send_to_C("getrenv")
end
getthread = function()
    return __send_to_C("getthread")
end
getaddress = function(any)
    return __send_to_C("getaddress", any)
end
getclipboard = function()
    return __send_to_C("getclipboard")
end
setclipboard = function(str)
	assert(type(str) == "string", "invalid argument #1 to 'setclipboard' (string expected, got " .. type(str) .. ") ", 2)
    return __send_to_C("setclipboard", str)
end
getinstanceaddress = function(inst)
    assert(typeof(inst) == "Instance", "invalid argument #1 to 'getinstanceaddress' (Instance expected, got " .. typeof(inst) .. ") ", 2)
    return __send_to_C("getinstanceaddress", inst)
end
tohex = function(number)
    assert(type(number) == "number", "invalid argument #1 to 'tohex' (number expected, got " .. type(number) .. ") ", 2)
    return string.format("%X", number)
end
readfile = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'readfile' (string expected, got " .. type(path) .. ") ", 2)
    return __send_to_C("readfile", path)
end
isfile = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'isfile' (string expected, got " .. type(path) .. ") ", 2)
    return __send_to_C("isfile", path)
end
listfiles = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'listfiles' (string expected, got " .. type(path) .. ") ", 2)
    return {__send_to_C("listfiles", path)}
end
listfolders = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'listfolders' (string expected, got " .. type(path) .. ") ", 2)
    return {__send_to_C("listfolders", path)}
end
writefile = function(path, data)
    assert(type(path) == "string", "invalid argument #1 to 'writefile' (string expected, got " .. type(path) .. ") ", 2)
    assert(type(data) == "string", "invalid argument #2 to 'writefile' (string expected, got " .. type(data) .. ") ", 2)
    __send_to_C("writefile", path, data)
end
makefolder = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'makefolder' (string expected, got " .. type(path) .. ") ", 2)
    __send_to_C("makefolder", path)
end
appendfile = function(path, data)
    assert(type(path) == "string", "invalid argument #1 to 'appendfile' (string expected, got " .. type(path) .. ") ", 2)
    assert(type(data) == "string", "invalid argument #2 to 'appendfile' (string expected, got " .. type(data) .. ") ", 2)
	local success, ret = pcall(function()
		return __send_to_C("writefile", path, __send_to_C("readfile", path)..data)
	end)
    if not success then
		error("Failed appendfile")
	end
end
isfolder = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'isfolder' (string expected, got " .. type(path) .. ") ", 2)
    return __send_to_C("isfolder", path)
end
delfile = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'delfile' (string expected, got " .. type(path) .. ") ", 2)
    __send_to_C("delfile", path)
end
delfolder = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'delfolder' (string expected, got " .. type(path) .. ") ", 2)
    __send_to_C("delfolder", path)
end
loadfile = function(path, chunkname)
    assert(type(path) == "string", "invalid argument #1 to 'loadfile' (string expected, got " .. type(path) .. ") ", 2)
    assert(type(chunkname) == "nil" or type(chunkname) == "string", "invalid argument #2 to 'loadfile' (nil or string expected, got " .. type(chunkname) .. ") ", 2)
    return loadstring(__send_to_C("readfile", path), chunkname)
end
dofile = function(path)
    assert(type(path) == "string", "invalid argument #1 to 'dofile' (string expected, got " .. type(path) .. ") ", 2)
    return loadfile(path)()
end
newcclosure = function(p1)
	return coroutine_wrap(function(...)
		while true do
			coroutine_yield(p1(...))
		end
	end)
end
getfpscap = function()
    return __send_to_C("getfpscap")
end
setfpscap = function(fps)
    assert(type(fps) == "number", "invalid argument #1 to 'setfpscap' (number expected, got " .. type(fps) .. ") ", 2)
    return __send_to_C("setfpscap", fps)
end
--getrawmetatable = function(tab)
    --assert(type(tab) == "table", "invalid argument #1 to 'getrawmetatable' (table expected, got " .. type(tab) .. ") ", 2)
    --return __send_to_C("getrawmetatable", tab)
--end
gethash = function(ts)
    assert(type(ts) == "string", "invalid argument #1 to 'sethash' (string expected, got " .. type(ts) .. ") ", 2)
    return __send_to_C("gethash", ts)
end
sethash = function(ts, h)
    assert(type(ts) == "string", "invalid argument #1 to 'sethash' (string expected, got " .. type(ts) .. ") ", 2)
    assert(type(h) == "number", "invalid argument #2 to 'sethash' (number expected, got " .. type(h) .. ") ", 2)
    return __send_to_C("sethash", ts, h)
end
--[[
"messagebox",
"rconsoleprint",
"rconsolewarn",
"rconsoleerr",
"rconsoleinfo",
"rconsolesettitle",
"rconsolecreate",
"rconsoledestroy",
"rconsoleinput",
]]
messagebox = function(text, caption, _type)
    assert(type(text) == "string", "invalid argument #1 to 'messagebox' (string expected, got " .. type(text) .. ") ", 2)
    assert(type(caption) == "string", "invalid argument #2 to 'messagebox' (number expected, got " .. type(caption) .. ") ", 2)
    assert(type(_type) == "number", "invalid argument #3 to 'messagebox' (number expected, got " .. type(_type) .. ") ", 3)
    return __send_to_C("messagebox", text, caption, _type)
end
rconsolesettitle = function(text)
    assert(type(text) == "string", "invalid argument #1 to 'rconsolesettitle' (string expected, got " .. type(text) .. ") ", 2)
    return __send_to_C("rconsolesettitle", text)
end
consolesettitle = rconsolesettitle
rconsoleprint = function(text)
    assert(type(text) == "string", "invalid argument #1 to 'rconsoleprint' (string expected, got " .. type(text) .. ") ", 2)
    return __send_to_C("rconsoleprint", text)
end
consoleprint = rconsoleprint
-- compatibility
rconsolewarn = rconsoleprint
consolewarn = rconsoleprint
rconsoleerr = rconsoleprint
consoleerr = rconsoleprint
rconsoleinfo = rconsoleprint
consoleinfo = rconsoleprint

rconsoleclear = function(text)
    assert(type(text) == "string", "invalid argument #1 to 'rconsoleprint' (string expected, got " .. type(text) .. ") ", 2)
    return __send_to_C("rconsoleprint", text)
end
consoleclear = rconsoleclear
rconsoleinput = function()
    return __send_to_C("rconsoleinput")
end
consoleinput = rconsoleinput
isrbxactive = function()
	return __send_to_C("isrbxactive")
end
rawprint = function(_type, text)
    assert(type(_type) == "number", "invalid argument #1 to 'rawprint' (number expected, got " .. type(_type) .. ") ", 2)
    assert(type(text) == "string", "invalid argument #2 to 'rawprint' (string expected, got " .. type(text) .. ") ", 2)
	return __send_to_C("rawprint", _type, text)
end
mousemoveabs = function(x, y)
    assert(type(x) == "number", "invalid argument #1 to 'mousemoveabs' (number expected, got " .. type(x) .. ") ", 2)
    assert(type(y) == "number", "invalid argument #2 to 'mousemoveabs' (string expected, got " .. type(y) .. ") ", 2)
	return __send_to_C("mousemoveabs", x, y)
end
mousemoverel = function(x, y)
    assert(type(x) == "number", "invalid argument #1 to 'mousemoverel' (number expected, got " .. type(x) .. ") ", 2)
    assert(type(y) == "number", "invalid argument #2 to 'mousemoverel' (string expected, got " .. type(y) .. ") ", 2)
	return __send_to_C("mousemoverel", x, y)
end
mousescroll = function(amount)
    assert(type(amount) == "number", "invalid argument #1 to 'mousescroll' (number expected, got " .. type(amount) .. ") ", 2)
	return __send_to_C("mousescroll", amount)
end
getreadonly = function(tab)
    assert(type(tab) == "table", "invalid argument #1 to 'getreadonly' (table expected, got " .. type(tab) .. ") ", 2)
	return table.isfrozen(tab)
end
isreadonly = getreadonly
setreadonly = function(tab, readonly)
    assert(type(tab) == "table", "invalid argument #1 to 'setreadonly' (table expected, got " .. type(tab) .. ") ", 2)
    assert(type(readonly) == "boolean", "invalid argument #2 to 'setreadonly' (boolean expected, got " .. type(tab) .. ") ", 2)
	return __send_to_C("setreadonly", tab, readonly)
end
getrawmetatable = function(object)
    assert(type(object) == "table" or type(object) == "userdata", "invalid argument #1 to 'getrawmetatable' (table or userdata expected, got " .. type(object) .. ") ", 2)
	return __send_to_C("getrawmetatable", object)
end
setrawmetatable = function(object, metatable)
    assert(type(object) == "table" or type(object) == "userdata", "invalid argument #1 to 'setrawmetatable' (table or userdata expected, got " .. type(object) .. ") ", 2)
    assert(type(metatable) == "table", "invalid argument #2 to 'setrawmetatable' (table expected, got " .. type(metatable) .. ") ", 2)
	return __send_to_C("setrawmetatable", object, metatable)
end
getnamecallmethod = function()
	return __send_to_C("getnamecallmethod")
end
setnamecallmethod = function(method)
    assert(type(method) == "string", "invalid argument #1 to 'setnamecallmethod' (string expected, got " .. type(method) .. ") ", 2)
	return __send_to_C("setnamecallmethod", method)
end
-- cursed hookmetamethod incoming
local coregui = game:GetService("CoreGui")
local FindFirstChild = coregui.FindFirstChild
local string_find = string.find
local table_remove = table.remove
local debug_traceback = debug.traceback
local task_wait = task.wait
local __error__ = error
local __pcall__ = pcall
local __unpack__ = unpack
local __tick__ = tick
local __taapr__internal_global_hooklist-- = __send_to_C("__getglobalhooklist__")
if not __taapr__internal_global_hooklist then
	__taapr__internal_global_hooklist = {}
	--__send_to_C("__setglobalhooklist__", __taapr__internal_global_hooklist)
end
local __taapr__internal__namecall__hooklist = {}
local __taapr__internal__newindex__hooklist = {}
local __taapr__internal__index__hooklist = {}
local function ____hookmetamethod____taapr____internal____(object, method, func)
	-- plusgiant5#7930
	metatable = getrawmetatable(object)
	if not metatable then
		error("Call to 'hookmetamethod' failed: "..tostring(object).." has no metatable")
	end
	if not metatable[method] then
		error("Call to 'hookmetamethod' failed: There is no '"..method.."' metamethod of "..tostring(object))
	end
	local old = metatable[method]
	local hookedtick = __tick__()
	setreadonly(metatable, false)
	metatable[method] = func--[[function(...)
		if __tick__() - hookedtick < 0.05 then -- random errors lol
			return old(...)
		end
		-- note that pcall can have any amount of returns above 1
		local rets = {__pcall__(function(...)
			return func(...)
		end, ...)}
		local success = rets[1] -- first return is success
		if not success and __tick__() - hookedtick > 0.05 then -- random errors lol
			-- corescripts also like to make random errors
			local iscorescript = string_find(debug_traceback(), "CoreGui") -- insecure but fast, a more secure way would be stack walking with getfenv() and checking if each script is inside coregui
			if not iscorescript then
				__error__(rets[2]) -- second return is error message if !success
			end
		end
		table_remove(rets, 1) -- pop the boolean "success" return
		return __unpack__(rets) -- return ALL the returns of func
	end]]
	setreadonly(metatable, true)
	--task_wait(0.1)
	return old
end
hookmetamethod = function(object, method, func)
    assert(type(object) == "table" or type(object) == "userdata", "invalid argument #1 to 'hookmetamethod' (table or userdata expected, got " .. type(object) .. ") ", 2)
    assert(type(method) == "string", "invalid argument #2 to 'hookmetamethod' (string expected, got " .. type(method) .. ") ", 2)
    assert(type(func) == "function", "invalid argument #3 to 'hookmetamethod' (function expected, got " .. type(func) .. ") ", 2)
	local oldfunc
	if method == "__index" then
		oldfunc = __taapr__internal__index__hooklist[#__taapr__internal__index__hooklist]
		if not oldfunc then
			error("Call to 'hookmetamethod' failed: "..tostring(object).."'s "..method.." has no base hooklist entry!")
		end
		table.insert(__taapr__internal__index__hooklist, func)
	elseif method == "__newindex" then
		oldfunc = __taapr__internal__newindex__hooklist[#__taapr__internal__newindex__hooklist]
		if not oldfunc then
			error("Call to 'hookmetamethod' failed: "..tostring(object).."'s "..method.." has no base hooklist entry!")
		end
		table.insert(__taapr__internal__newindex__hooklist, func)
	elseif method == "__namecall" then
		oldfunc = __taapr__internal__namecall__hooklist[#__taapr__internal__namecall__hooklist]
		if not oldfunc then
			error("Call to 'hookmetamethod' failed: "..tostring(object).."'s "..method.." has no base hooklist entry!")
		end
		table.insert(__taapr__internal__namecall__hooklist, func)
	else
		return ____hookmetamethod____taapr____internal____(object, method, func)
	end
	return oldfunc
end
safehookmetamethod = function(object, method, func)
    assert(type(object) == "table" or type(object) == "userdata", "invalid argument #1 to 'safehookmetamethod' (table or userdata expected, got " .. type(object) .. ") ", 2)
    assert(type(method) == "string", "invalid argument #2 to 'safehookmetamethod' (string expected, got " .. type(method) .. ") ", 2)
    assert(type(func) == "function", "invalid argument #3 to 'safehookmetamethod' (function expected, got " .. type(func) .. ") ", 2)
	metatable = getrawmetatable(object)
	if not metatable[method] then
		error("Call to 'safehookmetamethod' failed: There is no '"..method.."' method of "..tostring(object))
	end
	old = hookmetamethod(game, method, function(...)
		--[[local rets = {pcall(function(...)
			return func(...)
		end, ...)}
		local success = rets[1] -- first return is success]]
		func(...)
		return old(...)
	end)
end

local __taapr____plr__ = game:GetService("Players").LocalPlayer
getsimulationradius = function()
    return __send_to_C("getsimulationradius", __taapr____plr__)
end
getsimradius = getsimulationradius
setsimulationradius = function(radius)
    assert(type(radius) == "number", "invalid argument #1 to 'setsimulationradius' (number expected, got " .. type(radius) .. ") ", 2)
    return __send_to_C("setsimulationradius", __taapr____plr__, radius)
end
setsimradius = setsimulationradius

-- fake ones
gethiddenproperty = function(instance, property)
    assert(typeof(instance) == "Instance", "invalid argument #1 to 'setsimulationradius' (Instance expected, got " .. typeof(instance) .. ") ", 2)
    assert(type(radius) == "number", "invalid argument #1 to 'setsimulationradius' (number expected, got " .. type(radius) .. ") ", 2)
	if property == "SimulationRadius" or property == "MaximumSimulationRadius" then
		return getsimulationradius()
	end
end
gethiddenprop = gethiddenproperty
sethiddenproperty = function(instance, property, value)
	if property == "SimulationRadius" or property == "MaximumSimulationRadius" then
		setsimulationradius(value)
	end
end
sethiddenprop = sethiddenproperty

-- console always exists
rconsolecreate = function() end
consolecreate = rconsolecreate
rconsoledestroy = function() end
consoledestroy = rconsoledestroy

mousemoveabs = function(x, y)
    assert(type(x) == "number" or typeof(x) == "Vector3", "invalid argument #1 to 'mousemoveabs' (string expected, got " .. typeof(x) .. ") ", 2)
    assert(type(y) == "number", "invalid argument #2 to 'mousemoveabs' (string expected, got " .. type(y) .. ") ", 2)
	if typeof(x) == "Vector3" then
		y = x.Y
		x = x.X
	end
    return __send_to_C("mousemoveabs")
end

getinstances = function()
    local a = {}
	for b, c in next, getreg() do
		if type(c) == "table" then
			for d, e in next, c do
				if typeof(e) == "Instance" then
					table.insert(a, e)
				end
			end
		end
	end;
	return a
end
getnilinstances = function()
    local a = {}
	for b, c in next, getreg() do
		if type(c) == "table" then
			for d, e in next, c do
				if typeof(e) == "Instance" and e.Parent == nil then
					table.insert(a, e)
				end
			end
		end
	end
	return a
end
getthreads = function()
    local threads = {}
	for _, thread in pairs(getreg()) do
		if type(thread) == "thread" then
			threads[#threads + 1] = thread
		end
	end
	return threads
end
getthreadidentity = getidentity
setthreadidentity = setidentity
getthreadcontext = getidentity
setthreadcontext = setidentity
getsenv = function(p1)
	assert(typeof(p1) == "Instance", "invalid argument #1 to 'getsenv' (Instance expected, got " .. typeof(p1) .. ")", 2)
	assert(p1:IsA("LocalScript"), "invalid argument #2 to 'getsenv' (LocalScript expected, got " .. p1.ClassName .. ")", 2)
	for _, v in next, getreg() do
		if type(v) == "function" and getfenv(v).script == p1 then
			return getfenv(v)
		end
	end
	error("Script environment could not be found.")
end
getmenv = function(p1)
	assert(typeof(p1) == "Instance", "invalid argument #1 to 'getmenv' (Instance expected, got " .. typeof(p1) .. ")", 2)
	assert(p1:IsA("ModuleScript"), "invalid argument #2 to 'getmenv' (ModuleScript expected, got " .. p1.ClassName .. ")", 2)
	for _, v in next, getreg() do
		if type(v) == "function" and getfenv(v).script == p1 then
			return getfenv(v)
		end
	end
	error("Module environment could not be found.")
end

--fake functions here for more script support
getloadedmodules = getmodulescripts
disassemble = function(source)
    --https://github.com/TheSeaweedMonster/Celery/blob/main/decompile.lua
    if getscriptbytecode then
        return "-- getscriptbytecode is implemented but disassemble/decompile isn't??? Spam plusgiant6!"
    else
        return "-- disassemble/decompile not supported until getscriptbytecode is implemented"
    end
end
decompile = disassemble
replaceclosure = hookfunction
do
	local faketable = table.clone(table)
	faketable.unfreeze = function(tab)
		assert(type(tab) == "table", "invalid argument #1 to 'unfreeze' (table expected, got " .. type(tab) .. ")", 2)
		setreadonly(tab, false)
	end
	faketable.thaw = function(tab)
		assert(type(tab) == "table", "invalid argument #1 to 'thaw' (table expected, got " .. type(tab) .. ")", 2)
		setreadonly(tab, false)
	end
	table.freeze(faketable)
	table = faketable
end



--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------





loadstring = function(lua_source_code, chunkname)
    assert(type(lua_source_code) == "string", "invalid argument #1 to 'loadstring' (string expected, got " .. type(lua_source_code) .. ") ", 2)
	if chunkname == nil then
		return __send_to_C("loadstring", lua_source_code, nil)
	else
		return __send_to_C("loadstring", lua_source_code, tostring(chunkname))
	end
end




-- hooks for extra functions, with extra long names because if you accidentally edited one of them it would break your client
local ishooked = pcall(function()
	return game:GetService("CoreGui")["[ishooked]"] -- Comment out everything after return to disable metamethod hook
end)
if not ishooked then
	local function __self__httpget(self, ...)
		if self ~= game then
			error("Expected ':' not '.' calling member function HttpGet")
		end
		return httpget(...)
	end
	local function __self__getobjects(self, ...)
		if self ~= game then
			error("Expected ':' not '.' calling member function GetObjects")
		end
		return getobjects(...)
	end
	__taapr__internal__old__index__function = ____hookmetamethod____taapr____internal____(game, "__index", function(self, key)
		if self == game then
			--if checkcaller() then
				if key == "HttpGet" then
					return __self__httpget
				elseif key == "GetObjects" then
					return __self__getobjects
				end
			--end
		elseif self == game.CoreGui then -- because devs cant put an instance called "[ishooked]" into here
			if checkcaller() and key == "[ishooked]" then
				return true
			end
		end
		--[[for level = #hooklist, 1, -1 do
			local hook = hooklist[level]
			
		end]]
		-- one line, infinite hooks :brain:
		return __taapr__internal__index__hooklist[#__taapr__internal__index__hooklist](self, key)
	end)
	table.insert(__taapr__internal__index__hooklist, __taapr__internal__old__index__function)
	__taapr__internal__old__newindex__function = ____hookmetamethod____taapr____internal____(game, "__newindex", function(self, ...)
		-- no default hooks needed here
		return __taapr__internal__newindex__hooklist[#__taapr__internal__newindex__hooklist](self, ...)
	end)
	table.insert(__taapr__internal__newindex__hooklist, __taapr__internal__old__newindex__function)
	__taapr__internal__old__namecall__function = ____hookmetamethod____taapr____internal____(game, "__namecall", function(self, ...)
		if self == game then
			if checkcaller() then
				local method = getnamecallmethod()
				if method == "HttpGet" then
					return httpget(...)
				elseif method == "GetObjects" then
					return getobjects(...)
				end
			end
		end
		return __taapr__internal__namecall__hooklist[#__taapr__internal__namecall__hooklist](self, ...)
	end)
	table.insert(__taapr__internal__namecall__hooklist, __taapr__internal__old__namecall__function)
	task.wait(0.1) -- give metamethod hooks all the time in the world to activate
	print("TaaprWare V2 init done! (ignore the following errors)")
end

end
