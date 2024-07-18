--[[
	Made By: Jmuse#3982
	Full credit to the original script makers
	Website: http://rbxcheats.com/
]]


local PrisonLife = Instance.new("ScreenGui")
local MainGUI = Instance.new("Frame")
local topcolorframe = Instance.new("Frame")
local sidebar = Instance.new("Frame")
local LOCAL = Instance.new("Frame")
local GunsFrame = Instance.new("Frame")
local gunstitle = Instance.new("TextLabel")
local m4a1 = Instance.new("TextButton")
local ak47 = Instance.new("TextButton")
local shotgun = Instance.new("TextButton")
local m9 = Instance.new("TextButton")
local OtherFrame = Instance.new("Frame")
local othertitle = Instance.new("TextLabel")
local speedtext = Instance.new("TextBox")
local speed = Instance.new("TextButton")
local jumpheight = Instance.new("TextBox")
local jump = Instance.new("TextButton")
local btools = Instance.new("TextButton")
local TeamsFrame = Instance.new("Frame")
local teamstitle = Instance.new("TextLabel")
local inmate = Instance.new("TextButton")
local police = Instance.new("TextButton")
local crime = Instance.new("TextButton")
local na = Instance.new("TextButton")
local RAGE = Instance.new("Frame")
local KillFrame = Instance.new("Frame")
local killtitle = Instance.new("TextLabel")
local killall = Instance.new("TextButton")
local killaura = Instance.new("TextButton")
local killtext = Instance.new("TextBox")
local kill = Instance.new("TextButton")
local MISC = Instance.new("Frame")
local ModFrame = Instance.new("Frame")
local modtitle = Instance.new("TextLabel")
local modm4 = Instance.new("TextButton")
local modak = Instance.new("TextButton")
local modshotgun = Instance.new("TextButton")
local modm9 = Instance.new("TextButton")
local MiscFrame = Instance.new("Frame")
local misctitle = Instance.new("TextLabel")
local doors = Instance.new("TextButton")
local fences = Instance.new("TextButton")
local arrest = Instance.new("TextButton")
local flycar = Instance.new("TextButton")
local TPLIST = Instance.new("Frame")
local OtherFrame_2 = Instance.new("Frame")
local tptitle = Instance.new("TextLabel")
local proom = Instance.new("TextButton")
local prcells = Instance.new("TextButton")
local crime_2 = Instance.new("TextButton")
local yard = Instance.new("TextButton")
local TeamsFrame_2 = Instance.new("Frame")
local teamstitle_2 = Instance.new("TextLabel")
local na_2 = Instance.new("TextButton")
local TextBox = Instance.new("TextBox")
local INFO = Instance.new("Frame")
local OtherFrame_3 = Instance.new("Frame")
local infotitle = Instance.new("TextLabel")
local TextLabel = Instance.new("TextLabel")
local TextLabel_2 = Instance.new("TextLabel")
local TextLabel_3 = Instance.new("TextLabel")
local TextLabel_4 = Instance.new("TextLabel")
local TextLabel_5 = Instance.new("TextLabel")
local TextLabel_6 = Instance.new("TextLabel")
local TextLabel_7 = Instance.new("TextLabel")
local TextLabel_8 = Instance.new("TextLabel")
local info = Instance.new("TextButton")
local vlocal = Instance.new("TextButton")
local misc = Instance.new("TextButton")
local rage = Instance.new("TextButton")
local tplist = Instance.new("TextButton")
--Properties:
PrisonLife.Name = "Prison Life"
PrisonLife.Parent = game.CoreGui

MainGUI.Name = "MainGUI"
MainGUI.Parent = PrisonLife
MainGUI.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
MainGUI.BorderColor3 = Color3.new(0.0392157, 0.0392157, 0.0392157)
MainGUI.BorderSizePixel = 5
MainGUI.Position = UDim2.new(0.389766276, 0, 0.260442257, 0)
MainGUI.Size = UDim2.new(0, 524, 0, 389)
MainGUI.Visible = false
MainGUI.Active = true
MainGUI.Draggable = true

topcolorframe.Name = "topcolorframe"
topcolorframe.Parent = MainGUI
topcolorframe.BackgroundColor3 = Color3.new(1, 0, 0)
topcolorframe.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
topcolorframe.BorderSizePixel = 0
topcolorframe.Size = UDim2.new(0, 524, 0, 1)

sidebar.Name = "sidebar"
sidebar.Parent = MainGUI
sidebar.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
sidebar.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
sidebar.Position = UDim2.new(0.0162459817, 0, 0.0231362469, 0)
sidebar.Size = UDim2.new(0, 100, 0, 372)

LOCAL.Name = "LOCAL"
LOCAL.Parent = MainGUI
LOCAL.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
LOCAL.BorderColor3 = Color3.new(0.0392157, 0.0392157, 0.0392157)
LOCAL.BorderSizePixel = 0
LOCAL.Position = UDim2.new(0.22137405, 0, 0.0268495064, 0)
LOCAL.Size = UDim2.new(0, 408, 0, 378)
LOCAL.Visible = false

GunsFrame.Name = "GunsFrame"
GunsFrame.Parent = LOCAL
GunsFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
GunsFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
GunsFrame.Position = UDim2.new(0.00733424025, 0, 0.0094598867, 0)
GunsFrame.Size = UDim2.new(0, 193, 0, 123)

gunstitle.Name = "gunstitle"
gunstitle.Parent = GunsFrame
gunstitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
gunstitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
gunstitle.Position = UDim2.new(0.0518134832, 0, -0.0370370448, 0)
gunstitle.Size = UDim2.new(0, 45, 0, 11)
gunstitle.Font = Enum.Font.SourceSansBold
gunstitle.Text = "Guns"
gunstitle.TextColor3 = Color3.new(1, 1, 1)
gunstitle.TextSize = 14
gunstitle.TextWrapped = true

m4a1.Name = "m4a1"
m4a1.Parent = GunsFrame
m4a1.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
m4a1.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
m4a1.Position = UDim2.new(0.0310880821, 0, 0.225383908, 0)
m4a1.Size = UDim2.new(0, 83, 0, 27)
m4a1.Font = Enum.Font.SourceSansBold
m4a1.Text = "M4A1"
m4a1.TextColor3 = Color3.new(1, 1, 1)
m4a1.TextSize = 14
m4a1.MouseButton1Click:connect(function()
	  local Weapon = {"M4A1"}
for i,v in pairs(workspace.Prison_ITEMS.giver:GetChildren()) do
if v.Name == Weapon[1] then
local lol = workspace.Remote.ItemHandler:InvokeServer(v.ITEMPICKUP)
end
end
end)

ak47.Name = "ak47"
ak47.Parent = GunsFrame
ak47.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
ak47.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
ak47.Position = UDim2.new(0.531088054, 0, 0.225383908, 0)
ak47.Size = UDim2.new(0, 83, 0, 27)
ak47.Font = Enum.Font.SourceSansBold
ak47.Text = "AK-47"
ak47.TextColor3 = Color3.new(1, 1, 1)
ak47.TextSize = 14
ak47.MouseButton1Click:connect(function()
	local Weapon = {"AK-47"}
for i,v in pairs(workspace.Prison_ITEMS.giver:GetChildren()) do
if v.Name == Weapon[1] then
local lol = workspace.Remote.ItemHandler:InvokeServer(v.ITEMPICKUP)
end
end
end)

shotgun.Name = "shotgun"
shotgun.Parent = GunsFrame
shotgun.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
shotgun.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
shotgun.Position = UDim2.new(0.0310880821, 0, 0.539205074, 0)
shotgun.Size = UDim2.new(0, 83, 0, 27)
shotgun.Font = Enum.Font.SourceSansBold
shotgun.Text = "Shotgun"
shotgun.TextColor3 = Color3.new(1, 1, 1)
shotgun.TextSize = 14
shotgun.MouseButton1Click:connect(function()
 local Weapon = {"Remington 870"}
for i,v in pairs(workspace.Prison_ITEMS.giver:GetChildren()) do
if v.Name == Weapon[1] then
local lol = workspace.Remote.ItemHandler:InvokeServer(v.ITEMPICKUP)
end
end
end)

m9.Name = "m9"
m9.Parent = GunsFrame
m9.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
m9.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
m9.Position = UDim2.new(0.531088054, 0, 0.539205074, 0)
m9.Size = UDim2.new(0, 83, 0, 27)
m9.Font = Enum.Font.SourceSansBold
m9.Text = "M9"
m9.TextColor3 = Color3.new(1, 1, 1)
m9.TextSize = 14
m9.MouseButton1Click:connect(function()
   local Weapon = {"M9"}
for i,v in pairs(workspace.Prison_ITEMS.giver:GetChildren()) do
if v.Name == Weapon[1] then
local lol = workspace.Remote.ItemHandler:InvokeServer(v.ITEMPICKUP)
end
end
end)

OtherFrame.Name = "OtherFrame"
OtherFrame.Parent = LOCAL
OtherFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
OtherFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
OtherFrame.Position = UDim2.new(0.00733424164, 0, 0.382285297, 0)
OtherFrame.Size = UDim2.new(0, 397, 0, 226)

othertitle.Name = "othertitle"
othertitle.Parent = OtherFrame
othertitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
othertitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
othertitle.Position = UDim2.new(0.0518134832, 0, -0.0370370448, 0)
othertitle.Size = UDim2.new(0, 45, 0, 11)
othertitle.Font = Enum.Font.SourceSansBold
othertitle.Text = "Other"
othertitle.TextColor3 = Color3.new(1, 1, 1)
othertitle.TextSize = 14
othertitle.TextWrapped = true

speedtext.Name = "speedtext"
speedtext.Parent = OtherFrame
speedtext.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
speedtext.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
speedtext.Position = UDim2.new(0.0228751265, 0, 0.238738745, 0)
speedtext.Size = UDim2.new(0, 235, 0, 27)
speedtext.Font = Enum.Font.SourceSans
speedtext.Text = "[Number Value Here]"
speedtext.TextColor3 = Color3.new(1, 1, 1)
speedtext.TextSize = 14

speed.Name = "speed"
speed.Parent = OtherFrame
speed.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
speed.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
speed.Position = UDim2.new(0.646937907, 0, 0.235901609, 0)
speed.Size = UDim2.new(0, 130, 0, 27)
speed.Font = Enum.Font.SourceSansBold
speed.Text = "Set Speed"
speed.TextColor3 = Color3.new(1, 1, 1)
speed.TextSize = 14
speed.MouseButton1Click:connect(function()
	game:GetService("Players").LocalPlayer.Character.Humanoid.WalkSpeed = speedtext.Text
end)

jumpheight.Name = "jumpheight"
jumpheight.Parent = OtherFrame
jumpheight.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
jumpheight.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
jumpheight.Position = UDim2.new(0.0228751265, 0, 0.438738734, 0)
jumpheight.Size = UDim2.new(0, 235, 0, 27)
jumpheight.Font = Enum.Font.SourceSans
jumpheight.Text = "[Number Value Here]"
jumpheight.TextColor3 = Color3.new(1, 1, 1)
jumpheight.TextSize = 14

jump.Name = "jump"
jump.Parent = OtherFrame
jump.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
jump.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
jump.Position = UDim2.new(0.646937907, 0, 0.435901612, 0)
jump.Size = UDim2.new(0, 130, 0, 27)
jump.Font = Enum.Font.SourceSansBold
jump.Text = "Set Jump"
jump.TextColor3 = Color3.new(1, 1, 1)
jump.TextSize = 14
jump.MouseButton1Click:connect(function()
	game.Players.LocalPlayer.Character.Humanoid.JumpPower = jumpheight.Text
end)

btools.Name = "btools"
btools.Parent = OtherFrame
btools.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
btools.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
btools.Position = UDim2.new(0.0247010551, 0, 0.637095511, 0)
btools.Size = UDim2.new(0, 374, 0, 27)
btools.Font = Enum.Font.SourceSansBold
btools.Text = "Btools"
btools.TextColor3 = Color3.new(1, 1, 1)
btools.TextSize = 14
btools.MouseButton1Click:connect(function()
	wait(0.1)
local tool1 = Instance.new("HopperBin",game.Players.LocalPlayer.Backpack)
local tool2 = Instance.new("HopperBin",game.Players.LocalPlayer.Backpack)
local tool3 = Instance.new("HopperBin",game.Players.LocalPlayer.Backpack)
tool1.BinType = "Clone"
tool2.BinType = "Hammer"
tool3.BinType = "Grab"
end)

TeamsFrame.Name = "TeamsFrame"
TeamsFrame.Parent = LOCAL
TeamsFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TeamsFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
TeamsFrame.Position = UDim2.new(0.507477641, 0, 0.0094598867, 0)
TeamsFrame.Size = UDim2.new(0, 193, 0, 123)

teamstitle.Name = "teamstitle"
teamstitle.Parent = TeamsFrame
teamstitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
teamstitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
teamstitle.Position = UDim2.new(0.0518134832, 0, -0.0370370448, 0)
teamstitle.Size = UDim2.new(0, 45, 0, 11)
teamstitle.Font = Enum.Font.SourceSansBold
teamstitle.Text = "Teams"
teamstitle.TextColor3 = Color3.new(1, 1, 1)
teamstitle.TextSize = 14
teamstitle.TextWrapped = true

inmate.Name = "inmate"
inmate.Parent = TeamsFrame
inmate.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
inmate.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
inmate.Position = UDim2.new(0.0310880821, 0, 0.225383908, 0)
inmate.Size = UDim2.new(0, 83, 0, 27)
inmate.Font = Enum.Font.SourceSansBold
inmate.Text = "Inmate"
inmate.TextColor3 = Color3.new(1, 1, 1)
inmate.TextSize = 14
inmate.MouseButton1Click:connect(function()
	Workspace.Remote.TeamEvent:FireServer("Bright orange")
end)

police.Name = "police"
police.Parent = TeamsFrame
police.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
police.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
police.Position = UDim2.new(0.531088054, 0, 0.225383908, 0)
police.Size = UDim2.new(0, 83, 0, 27)
police.Font = Enum.Font.SourceSansBold
police.Text = "Police"
police.TextColor3 = Color3.new(1, 1, 1)
police.TextSize = 14
police.MouseButton1Click:connect(function()
	Workspace.Remote.TeamEvent:FireServer("Bright blue")
end)

crime.Name = "crime"
crime.Parent = TeamsFrame
crime.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
crime.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
crime.Position = UDim2.new(0.531088054, 0, 0.539205074, 0)
crime.Size = UDim2.new(0, 83, 0, 27)
crime.Font = Enum.Font.SourceSansBold
crime.Text = "Criminal"
crime.TextColor3 = Color3.new(1, 1, 1)
crime.TextSize = 14
crime.MouseButton1Click:connect(function()
	-- 02hacks Criminal Team v1
LP = game.Players.LocalPlayer

RE = LP.Character.HumanoidRootPart.Position
LP.Character.HumanoidRootPart.CFrame = CFrame.new(-919.958, 95.327, 2138.189)
wait(0.075)
LP.Character.HumanoidRootPart.CFrame = CFrame.new(RE)
end)

na.Name = "na"
na.Parent = TeamsFrame
na.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
na.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
na.Position = UDim2.new(0.0284973793, 0, 0.539205074, 0)
na.Size = UDim2.new(0, 83, 0, 27)
na.Font = Enum.Font.SourceSansBold
na.Text = "N/A"
na.TextColor3 = Color3.new(1, 1, 1)
na.TextSize = 14
na.MouseButton1Click:connect(function()
	Workspace.Remote.TeamEvent:FireServer("Medium stone grey")
end)

RAGE.Name = "RAGE"
RAGE.Parent = MainGUI
RAGE.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
RAGE.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
RAGE.BorderSizePixel = 0
RAGE.Position = UDim2.new(0.221000001, 0, 0.0270000007, 0)
RAGE.Size = UDim2.new(0, 408, 0, 378)
RAGE.Visible = false

KillFrame.Name = "KillFrame"
KillFrame.Parent = RAGE
KillFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
KillFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
KillFrame.Position = UDim2.new(0.00733424164, 0, 0.00945988391, 0)
KillFrame.Size = UDim2.new(0, 226, 0, 155)

killtitle.Name = "killtitle"
killtitle.Parent = KillFrame
killtitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
killtitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
killtitle.Position = UDim2.new(0.0291434582, 0, -0.0345339626, 0)
killtitle.Size = UDim2.new(0, 45, 0, 11)
killtitle.Font = Enum.Font.SourceSansBold
killtitle.Text = "Kill"
killtitle.TextColor3 = Color3.new(1, 1, 1)
killtitle.TextSize = 14
killtitle.TextWrapped = true

killall.Name = "killall"
killall.Parent = KillFrame
killall.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killall.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killall.Position = UDim2.new(0.0964354724, 0, 0.128225803, 0)
killall.Size = UDim2.new(0, 83, 0, 27)
killall.Font = Enum.Font.SourceSansBold
killall.Text = "Kill All"
killall.TextColor3 = Color3.new(1, 1, 1)
killall.TextSize = 14
killall.MouseButton1Click:connect(function()
	workspace.Remote.TeamEvent:FireServer("Medium stone grey")
 
game.Workspace.Remote.ItemHandler:InvokeServer(workspace.Prison_ITEMS.giver["Remington 870"].ITEMPICKUP)
 
wait(0.5)
function kill(a)
local A_1 =
{
[1] =
{
["RayObject"] = Ray.new(Vector3.new(845.555908, 101.429337, 2269.43945), Vector3.new(-391.152252, 8.65560055, -83.2166901)),
["Distance"] = 3.2524313926697,
["Cframe"] = CFrame.new(840.310791, 101.334137, 2267.87988, 0.0636406094, 0.151434347, -0.986416459, 0, 0.988420188, 0.151741937, 0.997972965, -0.00965694897, 0.0629036576),
["Hit"] = a.Character.Head
},
  [2] =
{
["RayObject"] = Ray.new(Vector3.new(845.555908, 101.429337, 2269.43945), Vector3.new(-392.481476, -8.44939327, -76.7261353)),
["Distance"] = 3.2699294090271,
["Cframe"] = CFrame.new(840.290466, 101.184189, 2267.93506, 0.0964837447, 0.0589403138, -0.993587971, 4.65661287e-10, 0.998245299, 0.0592165813, 0.995334625, -0.00571343815, 0.0963144377),
["Hit"] = a.Character.Head
},
[3] =
{
["RayObject"] = Ray.new(Vector3.new(845.555908, 101.429337, 2269.43945), Vector3.new(-389.21701, -2.50536323, -92.2163162)),
["Distance"] = 3.1665518283844,
["Cframe"] = CFrame.new(840.338867, 101.236496, 2267.80371, 0.0166504811, 0.0941716284, -0.995416701, 1.16415322e-10, 0.995554805, 0.0941846818, 0.999861419, -0.00156822044, 0.0165764652),
["Hit"] = a.Character.Head
},
[4] =
{
["RayObject"] = Ray.new(Vector3.new(845.555908, 101.429337, 2269.43945), Vector3.new(-393.353973, 3.13988972, -72.5452042)),
["Distance"] = 3.3218522071838,
["Cframe"] = CFrame.new(840.277222, 101.285957, 2267.9707, 0.117109694, 0.118740402, -0.985994935, -1.86264515e-09, 0.992826641, 0.119563118, 0.993119001, -0.0140019981, 0.116269611),
["Hit"] = a.Character.Head
},
[5] =
{
["RayObject"] = Ray.new(Vector3.new(845.555908, 101.429337, 2269.43945), Vector3.new(-390.73172, 3.2097764, -85.5477524)),
["Distance"] = 3.222757101059,
["Cframe"] = CFrame.new(840.317993, 101.286423, 2267.86035, 0.0517584644, 0.123365127, -0.991010666, 0, 0.992340803, 0.123530701, 0.99865967, -0.00639375951, 0.0513620302),
["Hit"] = a.Character.Head
}
}
local A_2 = game.Players.LocalPlayer.Backpack["Remington 870"]
local Event = game:GetService("ReplicatedStorage").ShootEvent
Event:FireServer(A_1, A_2)
Event:FireServer(A_1, A_2)
end
 
for i,v in pairs(game.Players:GetChildren())do
if v.Name ~= game.Players.LocalPlayer.Name then
kill(v)
end
end
wait(1)
workspace.Remote.TeamEvent:FireServer("Bright orange")
end)

killaura.Name = "killaura"
killaura.Parent = KillFrame
killaura.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killaura.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killaura.Position = UDim2.new(0.538623512, 0, 0.128225803, 0)
killaura.Size = UDim2.new(0, 83, 0, 27)
killaura.Font = Enum.Font.SourceSansBold
killaura.Text = "Kill Aura"
killaura.TextColor3 = Color3.new(1, 1, 1)
killaura.TextSize = 14
killaura.MouseButton1Click:connect(function()
	game:GetService("StarterGui"):SetCore("SendNotification", {
Title = "Info:";
Text = "Press E To Kill People Near You!";
})

	plr = game:service'Players'.LocalPlayer
char = plr.Character
mouse = plr:GetMouse()
on = false

mouse.KeyDown:connect(function(key)
  if key == "e" then
      on = true
  end
end)

mouse.KeyUp:connect(function(key)
  if key == "e" then
      on = false
  end
end)

while wait(0.1) do
  for i, b in pairs(game.Players:GetChildren()) do
      if b.Name ~= plr.Name and not b:IsFriendsWith(plr.UserId) and on then
          for i = 1, 10 do
              game.ReplicatedStorage.meleeEvent:FireServer(b)
          end
      end
  end
end


while true do
 wait(0.01)
 for i,v in pairs (game.Players:GetChildren()) do
   game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players[v.Name].Character.HumanoidRootPart.CFrame
   game.ReplicatedStorage.meleeEvent:FireServer(v.Name)
   wait(.8)
 end
end
end)

killtext.Name = "killtext"
killtext.Parent = KillFrame
killtext.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killtext.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
killtext.Position = UDim2.new(0.0929203555, 0, 0.429032266, 0)
killtext.Size = UDim2.new(0, 183, 0, 27)
killtext.Font = Enum.Font.SourceSansBold
killtext.Text = ""
killtext.TextColor3 = Color3.new(1, 1, 1)
killtext.TextSize = 14

kill.Name = "kill"
kill.Parent = KillFrame
kill.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
kill.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
kill.Position = UDim2.new(0.0920107663, 0, 0.724999964, 0)
kill.Size = UDim2.new(0, 182, 0, 27)
kill.Font = Enum.Font.SourceSansBold
kill.Text = "Kill Player"
kill.TextColor3 = Color3.new(1, 1, 1)
kill.TextSize = 14
kill.MouseButton1Click:connect(function()
	game.Workspace.Remote.ItemHandler:InvokeServer(workspace.Prison_ITEMS.giver["Remington 870"].ITEMPICKUP) 

wait(0.1)
Workspace.Remote.TeamEvent:FireServer("Medium stone grey")

local A_1 = 
{
	[1] = 
{
	["RayObject"] = Ray.new(Vector3.new(827.412415, 101.489777, 2296.84326), Vector3.new(277.738678, 6.89340925, 287.773712)), 
	["Distance"] = 4.7204174995422, 
	["Cframe"] = CFrame.new(832.049377, 101.392006, 2300.97168, 0.843892097, -0.0554918349, 0.533635378, 0, 0.994636595, 0.103430569, -0.536512911, -0.0872842371, 0.839366019), 
	["Hit"] = game.Workspace[killtext.Text].Head
}, 
	[2] = 
{
	["RayObject"] = Ray.new(Vector3.new(827.412415, 101.489777, 2296.84326), Vector3.new(303.047546, 21.3568707, 260.203888)), 
	["Distance"] = 4.8114862442017, 
	["Cframe"] = CFrame.new(832.390259, 101.550629, 2300.74097, 0.738044441, -0.112958886, 0.665229917, 7.45057971e-09, 0.985887885, 0.16740793, -0.674752235, -0.123554483, 0.727628946), 
	["Hit"] = game.Workspace[killtext.Text].Head
}, 
	[3] = 
{
	["RayObject"] = Ray.new(Vector3.new(827.412415, 101.489777, 2296.84326), Vector3.new(296.800507, 7.00420141, 268.067932)), 
	["Distance"] = 4.444625377655, 
	["Cframe"] = CFrame.new(832.185486, 101.391617, 2300.70264, 0.775115669, -0.0692948848, 0.628007889, 7.45057971e-09, 0.993967533, 0.109675139, -0.631819367, -0.0850109085, 0.770439863), 
	["Hit"] = game.Workspace[killtext.Text].Head
}, 
	[4] = 
{
	["RayObject"] = Ray.new(Vector3.new(827.412415, 101.489777, 2296.84326), Vector3.new(284.930573, 11.9850616, 280.483368)), 
	["Distance"] = 4.6211166381836, 
	["Cframe"] = CFrame.new(832.10083, 101.445007, 2300.86963, 0.820150614, -0.0735745132, 0.567397356, 0, 0.991697431, 0.128593579, -0.572147667, -0.105466105, 0.81334126), 
	["Hit"] = game.Workspace[killtext.Text].Head
}, 
	[5] = 
{
	["RayObject"] = Ray.new(Vector3.new(827.412415, 101.489777, 2296.84326), Vector3.new(294.625824, 2.15741801, 270.538269)), 
	["Distance"] = 4.4639973640442, 
	["Cframe"] = CFrame.new(832.169434, 101.341301, 2300.73438, 0.784266233, -0.0537625961, 0.618090749, -3.7252903e-09, 0.99623847, 0.086654529, -0.620424569, -0.0679602176, 0.781316102), 
	["Hit"] = game.Workspace[killtext.Text].Head
}
}
local A_2 = game.Players.LocalPlayer.Backpack["Remington 870"]
local Event = game:GetService("ReplicatedStorage").ShootEvent
Event:FireServer(A_1, A_2)

wait(0.5)
workspace.Remote.TeamEvent:FireServer("Bright orange")
end)

MISC.Name = "MISC"
MISC.Parent = MainGUI
MISC.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
MISC.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
MISC.BorderSizePixel = 0
MISC.Position = UDim2.new(0.221000001, 0, 0.0270000007, 0)
MISC.Size = UDim2.new(0, 408, 0, 378)
MISC.Visible = false

ModFrame.Name = "ModFrame"
ModFrame.Parent = MISC
ModFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
ModFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
ModFrame.Position = UDim2.new(0.00733424025, 0, 0.0094598867, 0)
ModFrame.Size = UDim2.new(0, 193, 0, 123)

modtitle.Name = "modtitle"
modtitle.Parent = ModFrame
modtitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
modtitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
modtitle.Position = UDim2.new(0.0518134832, 0, -0.0370370448, 0)
modtitle.Size = UDim2.new(0, 45, 0, 11)
modtitle.Font = Enum.Font.SourceSansBold
modtitle.Text = "Mod Guns"
modtitle.TextColor3 = Color3.new(1, 1, 1)
modtitle.TextSize = 14
modtitle.TextWrapped = true

modm4.Name = "modm4"
modm4.Parent = ModFrame
modm4.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modm4.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modm4.Position = UDim2.new(0.0310880821, 0, 0.225383908, 0)
modm4.Size = UDim2.new(0, 83, 0, 27)
modm4.Font = Enum.Font.SourceSansBold
modm4.Text = "M4A1"
modm4.TextColor3 = Color3.new(1, 1, 1)
modm4.TextSize = 14
modm4.MouseButton1Click:connect(function()
S = game.Players.LocalPlayer.Backpack["M4A1"].GunStates
rs = require(S)
for i,v in next, rs do 
    rs.Spread = 0
    rs.FireRate = 0
    rs.CurrentAmmo = math.huge
    rs.Bullets = 30
    rs.AutoFire = true
end
end)

modak.Name = "modak"
modak.Parent = ModFrame
modak.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modak.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modak.Position = UDim2.new(0.531088054, 0, 0.225383908, 0)
modak.Size = UDim2.new(0, 83, 0, 27)
modak.Font = Enum.Font.SourceSansBold
modak.Text = "AK-47"
modak.TextColor3 = Color3.new(1, 1, 1)
modak.TextSize = 14
modak.MouseButton1Click:connect(function()
S = game.Players.LocalPlayer.Backpack["AK-47"].GunStates
rs = require(S)
for i,v in next, rs do 
    rs.Spread = 0
    rs.FireRate = 0
    rs.CurrentAmmo = math.huge
    rs.Bullets = 30
    rs.AutoFire = true
end
end)

modshotgun.Name = "modshotgun"
modshotgun.Parent = ModFrame
modshotgun.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modshotgun.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modshotgun.Position = UDim2.new(0.0310880821, 0, 0.539205074, 0)
modshotgun.Size = UDim2.new(0, 83, 0, 27)
modshotgun.Font = Enum.Font.SourceSansBold
modshotgun.Text = "Shotgun"
modshotgun.TextColor3 = Color3.new(1, 1, 1)
modshotgun.TextSize = 14
modshotgun.MouseButton1Click:connect(function()
	S = game.Players.LocalPlayer.Backpack["Remington 870"].GunStates
rs = require(S)
for i,v in next, rs do 
	rs.Spread = 0
    rs.FireRate = 0
    rs.CurrentAmmo = math.huge
    rs.Bullets = 30
    rs.AutoFire = true
end
end)

modm9.Name = "modm9"
modm9.Parent = ModFrame
modm9.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modm9.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
modm9.Position = UDim2.new(0.531088054, 0, 0.539205074, 0)
modm9.Size = UDim2.new(0, 83, 0, 27)
modm9.Font = Enum.Font.SourceSansBold
modm9.Text = "M9"
modm9.TextColor3 = Color3.new(1, 1, 1)
modm9.TextSize = 14
modm9.MouseButton1Click:connect(function()
	S = game.Players.LocalPlayer.Backpack["M9"].GunStates
rs = require(S)
for i,v in next, rs do 
    rs.Spread = 0
    rs.FireRate = 0
    rs.CurrentAmmo = math.huge
    rs.Bullets = 30
    rs.AutoFire = true
end
end)

MiscFrame.Name = "MiscFrame"
MiscFrame.Parent = MISC
MiscFrame.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
MiscFrame.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
MiscFrame.Position = UDim2.new(0.507477641, 0, 0.0094598867, 0)
MiscFrame.Size = UDim2.new(0, 193, 0, 123)

misctitle.Name = "misctitle"
misctitle.Parent = MiscFrame
misctitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
misctitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
misctitle.Position = UDim2.new(0.0518134832, 0, -0.0370370448, 0)
misctitle.Size = UDim2.new(0, 45, 0, 11)
misctitle.Font = Enum.Font.SourceSansBold
misctitle.Text = "Misc"
misctitle.TextColor3 = Color3.new(1, 1, 1)
misctitle.TextSize = 14
misctitle.TextWrapped = true

doors.Name = "doors"
doors.Parent = MiscFrame
doors.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
doors.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
doors.Position = UDim2.new(0.0310880821, 0, 0.225383908, 0)
doors.Size = UDim2.new(0, 83, 0, 27)
doors.Font = Enum.Font.SourceSansBold
doors.Text = "Remove Doors"
doors.TextColor3 = Color3.new(1, 1, 1)
doors.TextSize = 14
doors.MouseButton1Down:connect(function()
	game.Workspace.Doors:Destroy()
end)

fences.Name = "fences"
fences.Parent = MiscFrame
fences.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
fences.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
fences.Position = UDim2.new(0.531088054, 0, 0.225383908, 0)
fences.Size = UDim2.new(0, 83, 0, 27)
fences.Font = Enum.Font.SourceSansBold
fences.Text = "Remove Fences"
fences.TextColor3 = Color3.new(1, 1, 1)
fences.TextSize = 14
fences.MouseButton1Down:connect(function()
	game.Workspace.Prison_Fences:Destroy()
end)

arrest.Name = "arrest"
arrest.Parent = MiscFrame
arrest.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
arrest.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
arrest.Position = UDim2.new(0.531088054, 0, 0.539205074, 0)
arrest.Size = UDim2.new(0, 83, 0, 27)
arrest.Font = Enum.Font.SourceSansBold
arrest.Text = "Click Arrest"
arrest.TextColor3 = Color3.new(1, 1, 1)
arrest.TextSize = 14
arrest.MouseButton1Down:connect(function()
	local mouse = game.Players.LocalPlayer:GetMouse()
   local arrestEvent = game.Workspace.Remote.arrest
   mouse.Button1Down:connect(function()
   local obj = mouse.Target
   local response = arrestEvent:InvokeServer(obj)
   end)
end)

flycar.Name = "flycar"
flycar.Parent = MiscFrame
flycar.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
flycar.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
flycar.Position = UDim2.new(0.0284973793, 0, 0.539205074, 0)
flycar.Size = UDim2.new(0, 83, 0, 27)
flycar.Font = Enum.Font.SourceSansBold
flycar.Text = "Fly Car"
flycar.TextColor3 = Color3.new(1, 1, 1)
flycar.TextSize = 14
flycar.MouseButton1Down:connect(function()
	 local hint = Instance.new("Hint",game.Players.LocalPlayer.PlayerGui)
   hint.Text = "Press Z To Toggle"
   hint.Name = game.JobId
repeat wait()
    until game.Players.LocalPlayer and game.Players.LocalPlayer.Character and game.Players.LocalPlayer.Character:findFirstChild("Torso") and game.Players.LocalPlayer.Character:findFirstChild("Humanoid")
local mouse = game.Players.LocalPlayer:GetMouse()
repeat wait() until mouse
local plr = game.Players.LocalPlayer
local torso = plr.Character.Torso
local flying = true
local deb = true
local ctrl = {f = 0, b = 0, l = 0, r = 0}
local lastctrl = {f = 0, b = 0, l = 0, r = 0}
local maxspeed = 500
local speed = 0
 
function Fly()
local bg = Instance.new("BodyGyro", torso)
bg.P = 9e4
bg.maxTorque = Vector3.new(9e9, 9e9, 9e9)
bg.cframe = torso.CFrame
local bv = Instance.new("BodyVelocity", torso)
bv.velocity = Vector3.new(0,0.1,0)
bv.maxForce = Vector3.new(9e9, 9e9, 9e9)
repeat wait()
plr.Character.Humanoid.PlatformStand = false
if ctrl.l + ctrl.r ~= 0 or ctrl.f + ctrl.b ~= 0 then
speed = speed+125.0+(speed/maxspeed)
if speed > maxspeed then
speed = maxspeed
end
elseif not (ctrl.l + ctrl.r ~= 0 or ctrl.f + ctrl.b ~= 0) and speed ~= 0 then
speed = speed-250
if speed < 0 then
speed = 0
end
end
if (ctrl.l + ctrl.r) ~= 0 or (ctrl.f + ctrl.b) ~= 0 then
bv.velocity = ((game.Workspace.CurrentCamera.CoordinateFrame.lookVector * (ctrl.f+ctrl.b)) + ((game.Workspace.CurrentCamera.CoordinateFrame * CFrame.new(ctrl.l+ctrl.r,(ctrl.f+ctrl.b)*.2,0).p) - game.Workspace.CurrentCamera.CoordinateFrame.p))*speed
lastctrl = {f = ctrl.f, b = ctrl.b, l = ctrl.l, r = ctrl.r}
elseif (ctrl.l + ctrl.r) == 0 and (ctrl.f + ctrl.b) == 0 and speed ~= 0 then
bv.velocity = ((game.Workspace.CurrentCamera.CoordinateFrame.lookVector * (lastctrl.f+lastctrl.b)) + ((game.Workspace.CurrentCamera.CoordinateFrame * CFrame.new(lastctrl.l+lastctrl.r,(lastctrl.f+lastctrl.b)*.2,0).p) - game.Workspace.CurrentCamera.CoordinateFrame.p))*speed
else
bv.velocity = Vector3.new(0,0.1,0)
end
bg.cframe = game.Workspace.CurrentCamera.CoordinateFrame * CFrame.Angles(-math.rad((ctrl.f+ctrl.b)*50*speed/maxspeed),0,0)
until not flying
ctrl = {f = 0, b = 0, l = 0, r = 0}
lastctrl = {f = 0, b = 0, l = 0, r = 0}
speed = 0
bg:Destroy()
bv:Destroy()
plr.Character.Humanoid.PlatformStand = false
end
mouse.KeyDown:connect(function(key)
if key:lower() == "z" then
if flying then flying = false
else
flying = true
Fly()
end
elseif key:lower() == "w" then
ctrl.f = 1
elseif key:lower() == "s" then
ctrl.b = -1
elseif key:lower() == "a" then
ctrl.l = -1
elseif key:lower() == "d" then
ctrl.r = 1
end
end)
mouse.KeyUp:connect(function(key)
if key:lower() == "w" then
ctrl.f = 0
elseif key:lower() == "s" then
ctrl.b = 0
elseif key:lower() == "a" then
ctrl.l = 0
elseif key:lower() == "d" then
ctrl.r = 0
end
wait(5)
hint:Destroy()
end)
Fly()
end)

TPLIST.Name = "TP LIST"
TPLIST.Parent = MainGUI
TPLIST.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TPLIST.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TPLIST.BorderSizePixel = 0
TPLIST.Position = UDim2.new(0.221000001, 0, 0.0270000007, 0)
TPLIST.Size = UDim2.new(0, 408, 0, 378)
TPLIST.Visible = false

OtherFrame_2.Name = "OtherFrame"
OtherFrame_2.Parent = TPLIST
OtherFrame_2.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
OtherFrame_2.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
OtherFrame_2.Position = UDim2.new(0.00733424164, 0, 0.00926943216, 0)
OtherFrame_2.Size = UDim2.new(0, 193, 0, 123)

tptitle.Name = "tptitle"
tptitle.Parent = OtherFrame_2
tptitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
tptitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
tptitle.Position = UDim2.new(0.0515263528, 0, -0.0318927914, 0)
tptitle.Size = UDim2.new(0, 45, 0, 11)
tptitle.Font = Enum.Font.SourceSansBold
tptitle.Text = "TP"
tptitle.TextColor3 = Color3.new(1, 1, 1)
tptitle.TextSize = 14
tptitle.TextWrapped = true

proom.Name = "proom"
proom.Parent = OtherFrame_2
proom.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
proom.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
proom.Position = UDim2.new(0.0309999995, 0, 0.224999994, 0)
proom.Size = UDim2.new(0, 83, 0, 27)
proom.Font = Enum.Font.SourceSansBold
proom.Text = "Police Room"
proom.TextColor3 = Color3.new(1, 1, 1)
proom.TextSize = 14
proom.MouseButton1Click:connect(function()
	game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = CFrame.new(836, 99, 2271)
end)

prcells.Name = "prcells"
prcells.Parent = OtherFrame_2
prcells.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
prcells.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
prcells.Position = UDim2.new(0.531000018, 0, 0.224999994, 0)
prcells.Size = UDim2.new(0, 83, 0, 27)
prcells.Font = Enum.Font.SourceSansBold
prcells.Text = "Prison Cells"
prcells.TextColor3 = Color3.new(1, 1, 1)
prcells.TextSize = 14
prcells.MouseButton1Click:connect(function()
	game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = CFrame.new(918, 97.73, 2447)
end)

crime_2.Name = "crime"
crime_2.Parent = OtherFrame_2
crime_2.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
crime_2.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
crime_2.Position = UDim2.new(0.0309999995, 0, 0.538999975, 0)
crime_2.Size = UDim2.new(0, 83, 0, 27)
crime_2.Font = Enum.Font.SourceSansBold
crime_2.Text = "Crime Base"
crime_2.TextColor3 = Color3.new(1, 1, 1)
crime_2.TextSize = 14
crime_2.MouseButton1Click:connect(function()
	game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = CFrame.new(-943.704, 91.632, 2056.925)
end)

yard.Name = "yard"
yard.Parent = OtherFrame_2
yard.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
yard.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
yard.Position = UDim2.new(0.531000018, 0, 0.538999975, 0)
yard.Size = UDim2.new(0, 83, 0, 27)
yard.Font = Enum.Font.SourceSansBold
yard.Text = "Yard"
yard.TextColor3 = Color3.new(1, 1, 1)
yard.TextSize = 14
yard.MouseButton1Click:connect(function()
	game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = CFrame.new(779.092, 96.001, 2451.114)
end)

TeamsFrame_2.Name = "TeamsFrame"
TeamsFrame_2.Parent = TPLIST
TeamsFrame_2.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TeamsFrame_2.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
TeamsFrame_2.Position = UDim2.new(0.507477641, 0, 0.0094598867, 0)
TeamsFrame_2.Size = UDim2.new(0, 193, 0, 123)

teamstitle_2.Name = "teamstitle"
teamstitle_2.Parent = TeamsFrame_2
teamstitle_2.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
teamstitle_2.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
teamstitle_2.Position = UDim2.new(0.062176168, 0, -0.0370370522, 0)
teamstitle_2.Size = UDim2.new(0, 70, 0, 11)
teamstitle_2.Font = Enum.Font.SourceSansBold
teamstitle_2.Text = "TP To Player"
teamstitle_2.TextColor3 = Color3.new(1, 1, 1)
teamstitle_2.TextSize = 14
teamstitle_2.TextWrapped = true

na_2.Name = "na"
na_2.Parent = TeamsFrame_2
na_2.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
na_2.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
na_2.Position = UDim2.new(0.0284974091, 0, 0.539205134, 0)
na_2.Size = UDim2.new(0, 180, 0, 27)
na_2.Font = Enum.Font.SourceSansBold
na_2.Text = "Teleport To Player"
na_2.TextColor3 = Color3.new(1, 1, 1)
na_2.TextSize = 14
na_2.MouseButton1Click:connect(function()
    local tp_namedplayer = TextBox.Text
    local tp_player = game:GetService("Players")[tp_namedplayer]
    local PLR = game:GetService("Players").LocalPlayer
    local p = TextBox.Text
   
    if tp_player then
        for i = 1,1 do
        wait(.08)
        PLR.Character.HumanoidRootPart.CFrame = tp_player.Character.HumanoidRootPart.CFrame + Vector3.new(0, 0, -1)
        end
    end
end)

TextBox.Parent = TeamsFrame_2
TextBox.BackgroundColor3 = Color3.new(0.117647, 0.117647, 0.117647)
TextBox.BorderColor3 = Color3.new(0.117647, 0.117647, 0.117647)
TextBox.Position = UDim2.new(0.0259067342, 0, 0.219512224, 0)
TextBox.Size = UDim2.new(0, 179, 0, 27)
TextBox.Font = Enum.Font.SourceSansBold
TextBox.Text = ""
TextBox.TextColor3 = Color3.new(1, 1, 1)
TextBox.TextSize = 14

INFO.Name = "INFO"
INFO.Parent = MainGUI
INFO.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
INFO.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
INFO.BorderSizePixel = 0
INFO.Position = UDim2.new(0.221000001, 0, 0.0270000007, 0)
INFO.Size = UDim2.new(0, 408, 0, 378)

OtherFrame_3.Name = "OtherFrame"
OtherFrame_3.Parent = INFO
OtherFrame_3.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
OtherFrame_3.BorderColor3 = Color3.new(0.235294, 0.235294, 0.235294)
OtherFrame_3.Position = UDim2.new(0.00733424164, 0, 0.0118968496, 0)
OtherFrame_3.Size = UDim2.new(0, 397, 0, 365)

infotitle.Name = "infotitle"
infotitle.Parent = OtherFrame_3
infotitle.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
infotitle.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
infotitle.Position = UDim2.new(0.0543323755, 0, -0.0139822904, 0)
infotitle.Size = UDim2.new(0, 45, 0, 11)
infotitle.Font = Enum.Font.SourceSansBold
infotitle.Text = "Info"
infotitle.TextColor3 = Color3.new(1, 1, 1)
infotitle.TextSize = 14
infotitle.TextWrapped = true

TextLabel.Parent = OtherFrame_3
TextLabel.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel.Position = UDim2.new(0.0977189764, 0, 0.169863015, 0)
TextLabel.Size = UDim2.new(0, 322, 0, 19)
TextLabel.Font = Enum.Font.SourceSansBold
TextLabel.Text = "GUI Made By: Jmuse#3982"
TextLabel.TextColor3 = Color3.new(1, 1, 1)
TextLabel.TextSize = 14

TextLabel_2.Parent = OtherFrame_3
TextLabel_2.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_2.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_2.Position = UDim2.new(0.0977189764, 0, 0.221917808, 0)
TextLabel_2.Size = UDim2.new(0, 322, 0, 19)
TextLabel_2.Font = Enum.Font.SourceSansBold
TextLabel_2.Text = "Youtube: Jmuse"
TextLabel_2.TextColor3 = Color3.new(1, 1, 1)
TextLabel_2.TextSize = 14

TextLabel_3.Parent = OtherFrame_3
TextLabel_3.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_3.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_3.Position = UDim2.new(0.105275653, 0, 0.273972601, 0)
TextLabel_3.Size = UDim2.new(0, 322, 0, 19)
TextLabel_3.Font = Enum.Font.SourceSansBold
TextLabel_3.Text = "Website: http://rbxcheats.com/"
TextLabel_3.TextColor3 = Color3.new(1, 1, 1)
TextLabel_3.TextSize = 14

TextLabel_4.Parent = OtherFrame_3
TextLabel_4.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_4.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_4.Position = UDim2.new(0.0952000841, 0, 0.326027393, 0)
TextLabel_4.Size = UDim2.new(0, 322, 0, 19)
TextLabel_4.Font = Enum.Font.SourceSansBold
TextLabel_4.Text = "DM Me On Discord For Server Invite"
TextLabel_4.TextColor3 = Color3.new(1, 1, 1)
TextLabel_4.TextSize = 14

TextLabel_5.Parent = OtherFrame_3
TextLabel_5.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_5.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_5.Position = UDim2.new(0.105275653, 0, 0.479452044, 0)
TextLabel_5.Size = UDim2.new(0, 322, 0, 19)
TextLabel_5.Font = Enum.Font.SourceSansBold
TextLabel_5.Text = "Most Scripts Made By: Jake11Price"
TextLabel_5.TextColor3 = Color3.new(1, 1, 1)
TextLabel_5.TextSize = 14

TextLabel_6.Parent = OtherFrame_3
TextLabel_6.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_6.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_6.Position = UDim2.new(0.105275653, 0, 0.531506896, 0)
TextLabel_6.Size = UDim2.new(0, 322, 0, 19)
TextLabel_6.Font = Enum.Font.SourceSansBold
TextLabel_6.Text = "Fly Car Script By: Exodots"
TextLabel_6.TextColor3 = Color3.new(1, 1, 1)
TextLabel_6.TextSize = 14

TextLabel_7.Parent = OtherFrame_3
TextLabel_7.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_7.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_7.Position = UDim2.new(0.105275653, 0, 0.712328792, 0)
TextLabel_7.Size = UDim2.new(0, 322, 0, 19)
TextLabel_7.Font = Enum.Font.SourceSansBold
TextLabel_7.Text = "Version: v2.2"
TextLabel_7.TextColor3 = Color3.new(1, 1, 1)
TextLabel_7.TextSize = 14

TextLabel_8.Parent = OtherFrame_3
TextLabel_8.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_8.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
TextLabel_8.Position = UDim2.new(0.105275653, 0, 0.764383614, 0)
TextLabel_8.Size = UDim2.new(0, 322, 0, 19)
TextLabel_8.Font = Enum.Font.SourceSansBold
TextLabel_8.Text = "Mod Gun Scripts Won't Work For Free Exploits!"
TextLabel_8.TextColor3 = Color3.new(1, 0, 0)
TextLabel_8.TextSize = 14

info.Name = "info"
info.Parent = MainGUI
info.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
info.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
info.Position = UDim2.new(0.0223664101, 0, 0.612118721, 0)
info.Size = UDim2.new(0, 93, 0, 50)
info.Font = Enum.Font.SourceSansBold
info.Text = "INFO"
info.TextColor3 = Color3.new(1, 1, 1)
info.TextSize = 14
info.MouseButton1Click:connect(function()
	INFO.Visible = true
	LOCAL.Visible = false
	MISC.Visible = false
	RAGE.Visible = false
	TPLIST.Visible = false
end)

vlocal.Name = "local"
vlocal.Parent = MainGUI
vlocal.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
vlocal.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
vlocal.Position = UDim2.new(0.0223664101, 0, 0.0521187522, 0)
vlocal.Size = UDim2.new(0, 93, 0, 50)
vlocal.Font = Enum.Font.SourceSansBold
vlocal.Text = "LOCAL"
vlocal.TextColor3 = Color3.new(1, 1, 1)
vlocal.TextSize = 14
vlocal.MouseButton1Click:connect(function()
	INFO.Visible = false
	LOCAL.Visible = true
	MISC.Visible = false
	RAGE.Visible = false
	TPLIST.Visible = false
end)

misc.Name = "misc"
misc.Parent = MainGUI
misc.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
misc.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
misc.Position = UDim2.new(0.0223664101, 0, 0.33211875, 0)
misc.Size = UDim2.new(0, 93, 0, 50)
misc.Font = Enum.Font.SourceSansBold
misc.Text = "MISC"
misc.TextColor3 = Color3.new(1, 1, 1)
misc.TextSize = 14
misc.MouseButton1Click:connect(function()
	INFO.Visible = false
	LOCAL.Visible = false
	MISC.Visible = true
	RAGE.Visible = false
	TPLIST.Visible = false
end)

rage.Name = "rage"
rage.Parent = MainGUI
rage.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
rage.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
rage.Position = UDim2.new(0.0223664101, 0, 0.192118794, 0)
rage.Size = UDim2.new(0, 93, 0, 50)
rage.Font = Enum.Font.SourceSansBold
rage.Text = "RAGE"
rage.TextColor3 = Color3.new(1, 1, 1)
rage.TextSize = 14
rage.MouseButton1Click:connect(function()
	INFO.Visible = false
	LOCAL.Visible = false
	MISC.Visible = false
	RAGE.Visible = true
	TPLIST.Visible = false
end)

tplist.Name = "tplist"
tplist.Parent = MainGUI
tplist.BackgroundColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
tplist.BorderColor3 = Color3.new(0.0784314, 0.0784314, 0.0784314)
tplist.Position = UDim2.new(0.0223664101, 0, 0.472118735, 0)
tplist.Size = UDim2.new(0, 93, 0, 50)
tplist.Font = Enum.Font.SourceSansBold
tplist.Text = "TP LIST"
tplist.TextColor3 = Color3.new(1, 1, 1)
tplist.TextSize = 14
tplist.MouseButton1Click:connect(function()
	INFO.Visible = false
	LOCAL.Visible = false
	MISC.Visible = false
	RAGE.Visible = false
	TPLIST.Visible = true
end)
-- Scripts:
function SCRIPT_ZVLF84_FAKESCRIPT() -- MainGUI.LocalScript 
	local script = Instance.new('LocalScript')
	script.Parent = MainGUI
	game.Players.LocalPlayer:GetMouse().KeyDown:connect(function(key)
	if key:lower() == "f" then
	    if script.Parent.Visible == false then
	        script.Parent.Visible = true
	    else
	        script.Parent.Visible = false
	    end
	end
	end)

end
coroutine.resume(coroutine.create(SCRIPT_ZVLF84_FAKESCRIPT))


game:GetService("StarterGui"):SetCore("SendNotification", {
Title = "Info:";
Text = "Prison Life GUI v2.2 Has Loaded";
})
wait(1)
game:GetService("StarterGui"):SetCore("SendNotification", {
Title = "Info:";
Text = "Press [F] To Open GUI";
})
wait(1)
game:GetService("StarterGui"):SetCore("SendNotification", {
Title = "Info:";
Text = "Made By: Jmuse#3982";
})

function zigzag(X) return math.acos(math.cos(X*math.pi))/math.pi end
       
        counter = 0
       
        while wait(0.1)do
         game.Players.LocalPlayer.PlayerGui.Home.hud.StaminaFrame.BackgroundColor3 = Color3.fromHSV(zigzag(counter),1,1)
         
         counter = counter + 0.01
        end

--[[
	Made By: Jmuse#3982
	Full credit to the original script makers
	Website: http://rbxcheats.com/
]]