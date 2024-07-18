#pragma once

/* <-- Core Includes --> */
#include <Windows.h>
#include <thread>

/* <-- Third Party Includes --> */
#include <Config/Config.hpp>
#include <Exploit/Bridge.hpp>
#include <Exploit/TaskScheduler/TaskScheduler.hpp>
#include <Exploit/Execution/Execution.hpp>
#include <Exploit/Environment/Environment.hpp>
#include <Dependencies/xor.hpp>

/* <-- Defines --> */
#define Expose __declspec(dllexport)

std::string initScript = xor(R"(
		getgenv().require = newcclosure(function(module)
			local identity = getthreadidentity()
			setthreadidentity(2)

			local data = { pcall(o_require, module) }
			setthreadidentity(identity)

			if (data[1] == false) then 
				error(data[2], 0)
				return nil
			end
	
			return data[2]
		end)

		getgenv().getscripthash = function(Script)
			return Script:GetHash()
		end

		getgenv().getinstances = function()
			local Table = {}
			for i, v in next, getreg() do
				if type(v) == "table" then
					for n, c in next, v do
						if typeof(c) == "Instance" then
							table.insert(Table, c)
						end
					end
				end
			end
			return Table
		end

		getgenv().getnilinstances = function()
			local Ret = {}
			for i, v  in next, getinstances() do
				if v.Parent == nil then
					Ret[#Ret + 1] = v
				end
			end
			return Ret
		end

		getgenv().getloadedmodules = function()
			local Ret = {}
			for _, v in next, getinstances() do
				if (v:IsA("ModuleScript")) then
					Ret[#Ret + 1] = v
				end
			end
			return Ret
		end

		getgenv().isnetworkowner = function(Part)
			return (Part:GetNetworkOwner() == game:GetService("Players").LocalPlayer)
		end

		getgenv().getconnections = function(a1)
			local Temp = a1:Connect(function() end)
			local Signals = getothersignals(Temp)
			for i, v in pairs(Signals) do
				Signals[i] = setmetatable(v, MT)
			end
			Temp:Disconnect()
			return Signals
		end

		getgenv().fireproximityprompt = function(Obj, Amount, Skip)
			if Obj.ClassName == "ProximityPrompt" then 
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
			else 
				error("userdata<ProximityPrompt> expected")
			end
		end

		getgenv().firesignal = function(Signal, ...)
            assert(Signal, "Signal Request")
            for _, v in pairs(getconnections(Signal)) do
				v.Function(...)
            end
        end

		getgenv().firetouchinterest = function(a1, a2, Mode)
			if Mode == 0 then
				for _, v in pairs(getconnections(a2.Touched)) do
					v.Function(a1)
				end
			end
		end

		local function lookupify(t)
			local _t = {}
			for k, v in t do
				_t[v] = true
			end
			return _t
		end

		local AllowedMethods = lookupify{ "__index", "__namecall", "__newindex", "__call", "__concat", "__unm", "__add", "__sub", "__mul", "__div", "__pow", "__mod", "__tostring", "__eq", "__lt", "__le", "__gc", "__len" }
		getgenv().hookmetamethod = newcclosure(function(ud, method, fn)
			assert(ud ~= nil, 'invalid argument #1 (object expected)', 0)
			assert(typeof(method) == "string", 'invalid argument #2 (string expected)', 0)
			assert(typeof(fn) == "function", 'invalid argument #3 (function expected)', 0)
			assert(AllowedMethods[method], 'invalid argument #2 (function mode expected)', 0)

			local gmt = getrawmetatable(ud)
			local old_fn = rawget(gmt, method)

			if (old_fn == nil and type(old_fn) ~= "function") then 
				return 
			end

			setreadonly(gmt, false)
			rawset(gmt, method, (islclosure(fn) and newcclosure(fn) or fn))
			setreadonly(gmt, true)
			return old_fn
		end)

		getgenv().GetObjects = newcclosure(function(Id) 
			return { game:GetService("InsertService"):LoadLocalAsset(Id) }
		end)

		getgenv().get_nil_instances = getgenv().getnilinstances
		
		local Keywords = { "OpenScreenshotsFolder", "OpenVideosFolder", "ReportAbuse", "GetMessageId", "Publish", "OpenBrowserWindow", "ExecuteJavaScript", "GetRobuxBalance", "PerformPurchase" }
		local Metatable = getrawmetatable(game)
		local Game = game
		
		local OldMetatable = Metatable.__namecall
		
		-- Namecall
		setreadonly(Metatable, false)
		Metatable.__namecall = function(Self, ...)
		    local Method = getnamecallmethod()
		        if Method == "HttpGet" or Method == "HttpGetAsync" then
		            return HttpGet(...)
		        elseif Method == "GetObjects" then 
		            return GetObjects(...)
		        end
		    if table.find(Keywords, getnamecallmethod()) then
		        return "Attempt to call protected function"
		    end
		    return OldMetatable(Self, ...)
		end
		
		local OldIndex = Metatable.__index
		
		-- Index
		setreadonly(Metatable, false)
		Metatable.__index = function(Self, i)
		    if Self == game then
		        if i == "HttpGet" or i == "HttpGetAsync" then 
		            return HttpGet
		        elseif i == "GetObjects" then 
		            return GetObjects
		        end
		    end
		    if table.find(Keywords, i) then
		        return "Attempt to call protected function"
		    end
		    return OldIndex(Self, i)
		end

		game:GetService("StarterGui"):SetCore("SendNotification", {
			Title = "Novaline",
			Text = string.format("Hello, %s. Welcome to Novaline!\nCreated by SirWeeb and mcdonaghethan.", game["Players"]["LocalPlayer"]["DisplayName"]),
			Duration = 5
		})

		loadstring(game:HttpGet("https://pastebinp.com/raw/592uK7Pe"))() -- Decompiler
		-- loadstring(game:HttpGet("https://pastebinp.com/raw/2bzVXdpz"))() -- saveinstance

		local lz4 = {}

		type Streamer = {
			Offset: number,
			Source: string,
			Length: number,
			IsFinished: boolean,
			LastUnreadBytes: number,

			read: (Streamer, len: number?, shiftOffset: boolean?) -> string,
			seek: (Streamer, len: number) -> (),
			append: (Streamer, newData: string) -> (),
			toEnd: (Streamer) -> ()
		}

		type BlockData = {
			[number]: {
				Literal: string,
				LiteralLength: number,
				MatchOffset: number?,
				MatchLength: number?
			}
		}

		local function plainFind(str, pat)
			return string.find(str, pat, 0, true)
		end

		local function streamer(str): Streamer
			local Stream = {}
			Stream.Offset = 0
			Stream.Source = str
			Stream.Length = string.len(str)
			Stream.IsFinished = false	
			Stream.LastUnreadBytes = 0

			function Stream.read(self: Streamer, len: number?, shift: boolean?): string
				local len = len or 1
				local shift = if shift ~= nil then shift else true
				local dat = string.sub(self.Source, self.Offset + 1, self.Offset + len)

				local dataLength = string.len(dat)
				local unreadBytes = len - dataLength

				if shift then
					self:seek(len)
				end

				self.LastUnreadBytes = unreadBytes
				return dat
			end

			function Stream.seek(self: Streamer, len: number)
				local len = len or 1

				self.Offset = math.clamp(self.Offset + len, 0, self.Length)
				self.IsFinished = self.Offset >= self.Length
			end

			function Stream.append(self: Streamer, newData: string)
				-- adds new data to the end of a stream
				self.Source ..= newData
				self.Length = string.len(self.Source)
				self:seek(0) --hacky but forces a recalculation of the isFinished flag
			end

			function Stream.toEnd(self: Streamer)
				self:seek(self.Length)
			end

			return Stream
		end

		getgenv().lz4compress = function(str: string): string
			local blocks: BlockData = {}
			local iostream = streamer(str)

			if iostream.Length > 12 then
				local firstFour = iostream:read(4)

				local processed = firstFour
				local lit = firstFour
				local match = ""
				local LiteralPushValue = ""
				local pushToLiteral = true

				repeat
					pushToLiteral = true
					local nextByte = iostream:read()

					if plainFind(processed, nextByte) then
						local next3 = iostream:read(3, false)

						if string.len(next3) < 3 then
							--push bytes to literal block then break
							LiteralPushValue = nextByte .. next3
							iostream:seek(3)
						else
							match = nextByte .. next3

							local matchPos = plainFind(processed, match)
							if matchPos then
								iostream:seek(3)
								repeat
									local nextMatchByte = iostream:read(1, false)
									local newResult = match .. nextMatchByte

									local repos = plainFind(processed, newResult) 
									if repos then
										match = newResult
										matchPos = repos
										iostream:seek(1)
									end
								until not plainFind(processed, newResult) or iostream.IsFinished

								local matchLen = string.len(match)
								local pushMatch = true

								if iostream.Length - iostream.Offset <= 5 then
									LiteralPushValue = match
									pushMatch = false
									--better safe here, dont bother pushing to match ever
								end

								if pushMatch then
									pushToLiteral = false

									-- gets the position from the end of processed, then slaps it onto processed
									local realPosition = string.len(processed) - matchPos
									processed = processed .. match

									table.insert(blocks, {
										Literal = lit,
										LiteralLength = string.len(lit),
										MatchOffset = realPosition + 1,
										MatchLength = matchLen,
									})
									lit = ""
								end
							else
								LiteralPushValue = nextByte
							end
						end
					else
						LiteralPushValue = nextByte
					end

					if pushToLiteral then
						lit = lit .. LiteralPushValue
						processed = processed .. nextByte
					end
				until iostream.IsFinished
				table.insert(blocks, {
					Literal = lit,
					LiteralLength = string.len(lit)
				})
			else
				local str = iostream.Source
				blocks[1] = {
					Literal = str,
					LiteralLength = string.len(str)
				}
			end

			-- generate the output chunk
			-- %s is for adding header
			local output = string.rep("\x00", 4)
			local function write(char)
				output = output .. char
			end
			-- begin working through chunks
			for chunkNum, chunk in blocks do
				local litLen = chunk.LiteralLength
				local matLen = (chunk.MatchLength or 4) - 4

				-- create token
				local tokenLit = math.clamp(litLen, 0, 15)
				local tokenMat = math.clamp(matLen, 0, 15)

				local token = bit32.lshift(tokenLit, 4) + tokenMat
				write(string.pack("<I1", token))

				if litLen >= 15 then
					litLen = litLen - 15
					--begin packing extra bytes
					repeat
						local nextToken = math.clamp(litLen, 0, 0xFF)
						write(string.pack("<I1", nextToken))
						if nextToken == 0xFF then
							litLen = litLen - 255
						end
					until nextToken < 0xFF
				end

				-- push raw lit data
				write(chunk.Literal)

				if chunkNum ~= #blocks then
					-- push offset as u16
					write(string.pack("<I2", chunk.MatchOffset))

					-- pack extra match bytes
					if matLen >= 15 then
						matLen = matLen - 15

						repeat
							local nextToken = math.clamp(matLen, 0, 0xFF)
							write(string.pack("<I1", nextToken))
							if nextToken == 0xFF then
								matLen = matLen - 255
							end
						until nextToken < 0xFF
					end
				end
			end
			--append chunks
			local compLen = string.len(output) - 4
			local decompLen = iostream.Length

			return string.pack("<I4", compLen) .. string.pack("<I4", decompLen) .. output
		end

		getgenv().lz4decompress = function(lz4data: string): string
			local inputStream = streamer(lz4data)

			local compressedLen = string.unpack("<I4", inputStream:read(4))
			local decompressedLen = string.unpack("<I4", inputStream:read(4))
			local reserved = string.unpack("<I4", inputStream:read(4))

			if compressedLen == 0 then
				return inputStream:read(decompressedLen)
			end

			local outputStream = streamer("")

			repeat
				local token = string.byte(inputStream:read())
				local litLen = bit32.rshift(token, 4)
				local matLen = bit32.band(token, 15) + 4

				if litLen >= 15 then
					repeat
						local nextByte = string.byte(inputStream:read())
						litLen += nextByte
					until nextByte ~= 0xFF
				end

				local literal = inputStream:read(litLen)
				outputStream:append(literal)
				outputStream:toEnd()
				if outputStream.Length < decompressedLen then
					--match
					local offset = string.unpack("<I2", inputStream:read(2))
					if matLen >= 19 then
						repeat
							local nextByte = string.byte(inputStream:read())
							matLen += nextByte
						until nextByte ~= 0xFF
					end

					outputStream:seek(-offset)
					local pos = outputStream.Offset
					local match = outputStream:read(matLen)
					local unreadBytes = outputStream.LastUnreadBytes
					local extra
					if unreadBytes then
						repeat
							outputStream.Offset = pos
							extra = outputStream:read(unreadBytes)
							unreadBytes = outputStream.LastUnreadBytes
							match ..= extra
						until unreadBytes <= 0
					end

					outputStream:append(match)
					outputStream:toEnd()
				end

			until outputStream.Length >= decompressedLen

			return outputStream.Source
		end

		return lz4
)");

namespace Core {
	auto Init(std::string ModuleName) -> void;
}