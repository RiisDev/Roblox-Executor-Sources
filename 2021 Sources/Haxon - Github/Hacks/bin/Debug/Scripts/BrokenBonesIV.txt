local player = game:GetService("Players").LocalPlayer
local Respawn
pcall(function() Respawn = getsenv(game:GetService("ReplicatedFirst"):FindFirstChildOfClass("LocalScript")).Respawn end)
_G.farm=true
while wait(0.5) do
if _G.farm then
local char = player.Character
local root = char.HumanoidRootPart
for j=1,3 do
char:SetPrimaryPartCFrame(CFrame.new(-400,700,-250))
wait(0.5)
for k=1,19 do
if root:FindFirstChild("BodyForce") then root.BodyForce:Destroy() end
root.Velocity = Vector3.new(0,-500,0)
wait(0.1)
end
end
for j=1,30 do
char:SetPrimaryPartCFrame(CFrame.new(0,26,-172))
root.Velocity = Vector3.new()
wait()
end
wait(4)
if Respawn then
player.PlayerGui.Gui.Summary:TweenPosition(UDim2.new(0.5, 0, 1.5, 0), "In", "Back", 0.75, true)
Respawn(workspace.CurrentCamera.CFrame, workspace.CurrentCamera.Focus)
player.PlayerGui.Gui.Summary.Visible=false
else
repeat wait(0.1) until not player.PlayerGui.Gui.Summary.Visible
end
end
end