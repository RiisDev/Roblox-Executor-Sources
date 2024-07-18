local tbl_remove = table.remove
local GetService = game.GetService
local httpget = httpget
local type = type

local HttpGet = function(self, url)
	return httpget(url)
end

local GetObjects = function(self, k)
	return { GetService(self, "InsertService"):LoadLocalAsset(k) }
end

local mt = getrawmetatable(game)
local index = mt.__index
local namecall = mt.__namecall
setreadonly(mt, false)
mt.__index = function(self, k)
	if checkcaller() then
		if type(k) == "string" then
			if k == "HttpGet" then
				return HttpGet
			elseif k == "GetObjects" then
				return GetObjects
			end
		end
	end
	return index(self, k)
end

mt.__namecall = function(self, ...)
	local args = { ... }
	local method = tbl_remove(args)
	if checkcaller() then				
		if type(method) == "string" then
			if method == "HttpGet" then
				return HttpGet(self, unpack(args))
			elseif method == "GetObjects" then
				return GetObjects(self, unpack(args))
			end
		end
	end
	return namecall(self, ...)
end

