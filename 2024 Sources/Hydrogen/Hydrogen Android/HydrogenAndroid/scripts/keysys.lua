R"delimiter( 

repeat wait() until game:IsLoaded()

local insertService = game:GetService("InsertService")

local gui = insertService:LoadLocalAsset("rbxassetid://11481964053")

gui.Parent = game:GetService("CoreGui")

local aspectRatioConstraint = Instance.new("UIAspectRatioConstraint")

local guiOpen = false

--local DraggableObject = require(gui.Draggable)

local containingFrame = gui.ContainingFrame
local tabContainer = containingFrame.TabContainer

local layout = tabContainer.UIListLayout

local main = containingFrame.Main

local textBox = main.TextBox

local logoButton = tabContainer.LogoButton
local closeBtn = tabContainer.CloseButton
local getKeyBtn = main.GetKey
local verifyKeyBtn = main.VerifyKey
local status = tabContainer.Status
local keyInput = main.TextBox

--local dragger = DraggableObject.new(tabContainer)

if getreg().PEOU2IGKB4VKSBGPYQADA then
	keyInput.Text = getreg().PEOU2IGKB4VKSBGPYQADA
end

local function close()
	main.Visible = false
	closeBtn.Visible = false
	aspectRatioConstraint.Parent = tabContainer
	layout.Parent = nil
	main.Visible = false
	status.Visible = false

	--dragger:Enable();
end

local function open()
	main.Visible = true
	closeBtn.Visible = true
	aspectRatioConstraint.Parent = nil
	main.Visible = true
	status.Visible = true
	
	layout.Parent = tabContainer
	
	--dragger:Disable();

	tabContainer.Position = UDim2.new(0.5, 0, 0.1, 0)
end

local function setErrorStatus(text)
	status.TextColor3 = Color3.new(1, 0, 0.0156863)
	status.Text = text
end

local function setInfoStatus(text)
	status.TextColor3 = Color3.new(244, 244, 244)
	status.Text = text
end

close()

closeBtn.Activated:Connect(function()
	close()
end)

--[[
local timeDown = nil

logoButton.MouseButton1Down:Connect(function()
	timeDown = time()
end)

logoButton.MouseButton1Up:Connect(function()
	if time() - timeDown < 0.2 then
		open()
	end
end)
]]--
logoButton.Activated:Connect(function()
	open()
end)

getKeyBtn.Activated:Connect(newcclosure(function()
	setInfoStatus("Link Copied...")

	getreg().PEOU2IGKB4VKSBGPYQAD();
end))

local semver = {
	_VERSION     = '1.2.1',
	_DESCRIPTION = 'semver for Lua',
	_URL         = 'https://github.com/kikito/semver.lua',
	_LICENSE     = [[
    MIT LICENSE

    Copyright (c) 2015 Enrique García Cota

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of tother software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and tother permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ]]
} do
	local function checkPositiveInteger(number, name)
		assert(number >= 0, name .. ' must be a valid positive number')
		assert(math.floor(number) == number, name .. ' must be an integer')
	end

	local function present(value)
		return value and value ~= ''
	end

	-- splitByDot("a.bbc.d") == {"a", "bbc", "d"}
	local function splitByDot(str)
		str = str or ""
		local t, count = {}, 0
		str:gsub("([^%.]+)", function(c)
			count = count + 1
			t[count] = c
		end)
		return t
	end

	local function parsePrereleaseAndBuildWithSign(str)
		local prereleaseWithSign, buildWithSign = str:match("^(-[^+]+)(+.+)$")
		if not (prereleaseWithSign and buildWithSign) then
			prereleaseWithSign = str:match("^(-.+)$")
			buildWithSign      = str:match("^(+.+)$")
		end
		assert(prereleaseWithSign or buildWithSign, ("The parameter %q must begin with + or - to denote a prerelease or a build"):format(str))
		return prereleaseWithSign, buildWithSign
	end

	local function parsePrerelease(prereleaseWithSign)
		if prereleaseWithSign then
			local prerelease = prereleaseWithSign:match("^-(%w[%.%w-]*)$")
			assert(prerelease, ("The prerelease %q is not a slash followed by alphanumerics, dots and slashes"):format(prereleaseWithSign))
			return prerelease
		end
	end

	local function parseBuild(buildWithSign)
		if buildWithSign then
			local build = buildWithSign:match("^%+(%w[%.%w-]*)$")
			assert(build, ("The build %q is not a + sign followed by alphanumerics, dots and slashes"):format(buildWithSign))
			return build
		end
	end

	local function parsePrereleaseAndBuild(str)
		if not present(str) then return nil, nil end

		local prereleaseWithSign, buildWithSign = parsePrereleaseAndBuildWithSign(str)

		local prerelease = parsePrerelease(prereleaseWithSign)
		local build = parseBuild(buildWithSign)

		return prerelease, build
	end

	local function parseVersion(str)
		local sMajor, sMinor, sPatch, sPrereleaseAndBuild = str:match("^(%d+)%.?(%d*)%.?(%d*)(.-)$")
		assert(type(sMajor) == 'string', ("Could not extract version number(s) from %q"):format(str))
		local major, minor, patch = tonumber(sMajor), tonumber(sMinor), tonumber(sPatch)
		local prerelease, build = parsePrereleaseAndBuild(sPrereleaseAndBuild)
		return major, minor, patch, prerelease, build
	end


	-- return 0 if a == b, -1 if a < b, and 1 if a > b
	local function compare(a,b)
		return a == b and 0 or a < b and -1 or 1
	end

	local function compareIds(myId, otherId)
		if myId == otherId then return  0
		elseif not myId    then return -1
		elseif not otherId then return  1
		end

		local selfNumber, otherNumber = tonumber(myId), tonumber(otherId)

		if selfNumber and otherNumber then -- numerical comparison
			return compare(selfNumber, otherNumber)
			-- numericals are always smaller than alphanums
		elseif selfNumber then
			return -1
		elseif otherNumber then
			return 1
		else
			return compare(myId, otherId) -- alphanumerical comparison
		end
	end

	local function smallerIdList(myIds, otherIds)
		local myLength = #myIds
		local comparison

		for i=1, myLength do
			comparison = compareIds(myIds[i], otherIds[i])
			if comparison ~= 0 then
				return comparison == -1
			end
			-- if comparison == 0, continue loop
		end

		return myLength < #otherIds
	end

	local function smallerPrerelease(mine, other)
		if mine == other or not mine then return false
		elseif not other then return true
		end

		return smallerIdList(splitByDot(mine), splitByDot(other))
	end

	local methods = {}

	function methods:nextMajor()
		return semver(self.major + 1, 0, 0)
	end
	function methods:nextMinor()
		return semver(self.major, self.minor + 1, 0)
	end
	function methods:nextPatch()
		return semver(self.major, self.minor, self.patch + 1)
	end

	local mt = { __index = methods }
	function mt:__eq(other)
		return self.major == other.major and
			self.minor == other.minor and
			self.patch == other.patch and
			self.prerelease == other.prerelease
		-- notice that build is ignored for precedence in semver 2.0.0
	end
	function mt:__lt(other)
		if self.major ~= other.major then return self.major < other.major end
		if self.minor ~= other.minor then return self.minor < other.minor end
		if self.patch ~= other.patch then return self.patch < other.patch end
		return smallerPrerelease(self.prerelease, other.prerelease)
		-- notice that build is ignored for precedence in semver 2.0.0
	end
	-- This works like the "pessimisstic operator" in Rubygems.
	-- if a and b are versions, a ^ b means "b is backwards-compatible with a"
	-- in other words, "it's safe to upgrade from a to b"
	function mt:__pow(other)
		if self.major == 0 then
			return self == other
		end
		return self.major == other.major and
			self.minor <= other.minor
	end
	function mt:__tostring()
		local buffer = { ("%d.%d.%d"):format(self.major, self.minor, self.patch) }
		if self.prerelease then table.insert(buffer, "-" .. self.prerelease) end
		if self.build      then table.insert(buffer, "+" .. self.build) end
		return table.concat(buffer)
	end

	local function new(major, minor, patch, prerelease, build)
		assert(major, "At least one parameter is needed")

		if type(major) == 'string' then
			major,minor,patch,prerelease,build = parseVersion(major)
		end
		patch = patch or 0
		minor = minor or 0

		checkPositiveInteger(major, "major")
		checkPositiveInteger(minor, "minor")
		checkPositiveInteger(patch, "patch")

		local result = {major=major, minor=minor, patch=patch, prerelease=prerelease, build=build}
		return setmetatable(result, mt)
	end

	setmetatable(semver, { __call = function(_, ...) return new(...) end })
	semver._VERSION= semver(semver._VERSION)
end

verifyKeyBtn.Activated:Connect(newcclosure(function()

	setInfoStatus("Verifying...")

	local android_json = game:HttpGet("https://api.hydrogen.sh/releases/ANDROID")
	android_json = game:GetService"HttpService":JSONDecode(android_json)

	local _,current_v = getgenv().identifyexecutor()

	if not current_v then
		setErrorStatus("A new update is available!")
		return
	end

	local latest_v = android_json.data.version

	if semver(current_v) < semver(latest_v) then
		setErrorStatus("A new update is available! Download on the Website")
		return
	end

	local url = string.format("https://api.hydrogen.sh/gateway/auth?key=%s&hwid=%s", keyInput.Text, getreg().TDXP4J2MQBH163PZT1S1())

	local status, res = pcall(game.HttpGet, game, url)

	if not status then
		if getreg().PEOU2IGKB4VKSBGPYQADA then
			setErrorStatus("Key Expired")
			getreg().TDXP4J2MQBH163PZT1S169()
			getreg().PEOU2IGKB4VKSBGPYQADA = nil
		else
			setErrorStatus("Invalid Key")
		end

		return
	end

	if string.find(res, "expiresAt") then
		setInfoStatus("Verified")
		getreg().TDXP4J2MQBH163PZT1S0(keyInput.Text)
		gui:Destroy()
	else
		if getreg().PEOU2IGKB4VKSBGPYQADA then
			setErrorStatus("Key Expired")
		else
			setErrorStatus("Invalid Key")
		end
	end
end, true))

)delimiter"