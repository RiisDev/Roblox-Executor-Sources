--[[
     Enable Reset Character
     By: OpenGamerTips

     NOTE:
     This script enables the abillity to reset your character, but this script executes the enabler every split second.
     Do not execute the script again, as it will stress your script executor.
]]--

while true do
     game:GetService("StarterGui"):SetCore("ResetButtonCallback", true)
     game:GetService("RunService").RenderStepped:Wait()
end
