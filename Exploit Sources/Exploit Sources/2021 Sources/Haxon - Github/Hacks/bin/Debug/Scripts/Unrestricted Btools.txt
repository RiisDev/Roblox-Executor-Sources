--[[
    Unrestricted BTools
	By: OpenGamerTips

	Keybinds:
	Press P for BTools
	Press L to unlock the Workspace
	Press K to enable the backpack/other core elements.
]]--

local player = game.Players.LocalPlayer
local mouse = player:GetMouse()

mouse.KeyDown:connect(function(key)
    key = string.lower(key)

	if key == "p" then
		for item = 1, 4 do
			HopperBin = Instance.new("HopperBin", game.Players.LocalPlayer.Backpack)
			HopperBin.BinType = item
		end
	elseif key == "l" then
		function UnlockWorkspace(a)
			for n,part in pairs(a:GetChildren()) do
				if(part:IsA("BasePart")) then 
					part.Locked = false 
				end
				UnlockWorkspace(part)
			end
		end
		UnlockWorkspace(workspace)
	elseif key == "k" then
		local StarterGui = game:GetService("StarterGui")
		StarterGui:SetCoreGuiEnabled(Enum.CoreGuiType.All, true)
	end
end)
