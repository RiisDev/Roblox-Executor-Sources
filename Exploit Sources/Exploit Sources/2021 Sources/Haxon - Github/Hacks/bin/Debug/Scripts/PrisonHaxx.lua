-- Instances:
local GunModz = Instance.new("ScreenGui")
local mainDrag = Instance.new("Frame")
local main = Instance.new("Frame")
local m9 = Instance.new("TextButton")
local rem = Instance.new("TextButton")
local m4 = Instance.new("TextButton")
local ta = Instance.new("TextButton")
local ak = Instance.new("TextButton")
local TopBar = Instance.new("TextLabel")
--Properties:
GunModz.Name = "GunModz"
GunModz.Parent = game.CoreGui

mainDrag.Name = "mainDrag"
mainDrag.Parent = GunModz
mainDrag.Active = true
mainDrag.BackgroundColor3 = Color3.new(1, 1, 1)
mainDrag.BackgroundTransparency = 1
mainDrag.Position = UDim2.new(0.477227718, 0, 0.550653577, 0)
mainDrag.Size = UDim2.new(0, 486, 0, 252)

main.Name = "main"
main.Parent = mainDrag
main.BackgroundColor3 = Color3.new(1, 1, 1)
main.BackgroundTransparency = 0.5
main.BorderSizePixel = 0
main.Position = UDim2.new(0, 0, -0.00398406386, 0)
main.Size = UDim2.new(0, 486, 0, 252)

m9.Name = "m9"
m9.Parent = main
m9.BackgroundColor3 = Color3.new(1, 1, 1)
m9.BackgroundTransparency = 0.5
m9.BorderSizePixel = 0
m9.Position = UDim2.new(0.028806584, 0, 0.158730164, 0)
m9.Size = UDim2.new(0, 63, 0, 50)
m9.Font = Enum.Font.SourceSans
m9.Text = "M9"
m9.TextColor3 = Color3.new(0, 0, 0)
m9.TextScaled = true
m9.TextSize = 14
m9.TextWrapped = true

rem.Name = "rem"
rem.Parent = main
rem.BackgroundColor3 = Color3.new(1, 1, 1)
rem.BackgroundTransparency = 0.5
rem.BorderSizePixel = 0
rem.Position = UDim2.new(0.197530866, 0, 0.158730164, 0)
rem.Size = UDim2.new(0, 63, 0, 50)
rem.Font = Enum.Font.SourceSans
rem.Text = "Remmington"
rem.TextColor3 = Color3.new(0, 0, 0)
rem.TextScaled = true
rem.TextSize = 14
rem.TextWrapped = true

m4.Name = "m4"
m4.Parent = main
m4.BackgroundColor3 = Color3.new(1, 1, 1)
m4.BackgroundTransparency = 0.5
m4.BorderSizePixel = 0
m4.Position = UDim2.new(0.374485612, 0, 0.158730164, 0)
m4.Size = UDim2.new(0, 63, 0, 50)
m4.Font = Enum.Font.SourceSans
m4.Text = "M4A1"
m4.TextColor3 = Color3.new(0, 0, 0)
m4.TextScaled = true
m4.TextSize = 14
m4.TextWrapped = true

ta.Name = "ta"
ta.Parent = main
ta.BackgroundColor3 = Color3.new(1, 1, 1)
ta.BackgroundTransparency = 0.5
ta.BorderSizePixel = 0
ta.Position = UDim2.new(0.54320991, 0, 0.158730164, 0)
ta.Size = UDim2.new(0, 63, 0, 50)
ta.Font = Enum.Font.SourceSans
ta.Text = "Taser"
ta.TextColor3 = Color3.new(0, 0, 0)
ta.TextScaled = true
ta.TextSize = 14
ta.TextWrapped = true

ak.Name = "ak"
ak.Parent = main
ak.BackgroundColor3 = Color3.new(1, 1, 1)
ak.BackgroundTransparency = 0.5
ak.BorderSizePixel = 0
ak.Position = UDim2.new(0.703703761, 0, 0.158730164, 0)
ak.Size = UDim2.new(0, 63, 0, 50)
ak.Font = Enum.Font.SourceSans
ak.Text = "AK-47"
ak.TextColor3 = Color3.new(0, 0, 0)
ak.TextScaled = true
ak.TextSize = 14
ak.TextWrapped = true

TopBar.Name = "TopBar"
TopBar.Parent = mainDrag
TopBar.BackgroundColor3 = Color3.new(1, 1, 1)
TopBar.BackgroundTransparency = 0.5
TopBar.BorderSizePixel = 0
TopBar.Size = UDim2.new(0, 486, 0, 28)
TopBar.Font = Enum.Font.SourceSans
TopBar.Text = "GunModz | By Guy998"
TopBar.TextColor3 = Color3.new(0, 0, 0)
TopBar.TextScaled = true
TopBar.TextSize = 14
TopBar.TextWrapped = true
TopBar.TextXAlignment = Enum.TextXAlignment.Left
-- Scripts:

mainDrag.Draggable = true

m9.MouseButton1Down:connect(function()
	local Player = game.Players.LocalPlayer.Name
local Gun = "M9" -- < -- Gun Name
local Run = game:GetService("RunService")

Gun = game.Players[Player].Character[Gun]
local Mouse = game.Players.LocalPlayer:GetMouse()
local Down = false
local Sound = Gun.Handle.FireSound

function CreateRay(Point_A, Point_B)
local Ray = Ray.new(Point_A, (Point_B - Point_A).Unit * (2 ^ 31 - 1))
local Part, Pos = workspace:FindPartOnRay(Ray, game.Players.LocalPlayer.Character)
local Dist = (Point_A - Pos).Magnitude
local CFrame = CFrame.new(Point_A, Pos) * CFrame.new(0, 0, -Dist / 2)

return CFrame, Dist, Ray
end

function FireLaser(target)
coroutine.resume(coroutine.create(function()
local C, D, R = CreateRay(Gun.Muzzle.CFrame.p, target.CFrame.p)
local Bullet = Instance.new("Part", Gun)
Bullet.BrickColor = BrickColor.Yellow()
Bullet.Material = "Neon"
Bullet.Anchored = true
Bullet.CanCollide = false
Bullet.Size = Vector3.new(0.2, 0.2, D)
Bullet.CFrame = C

local bulletTable = {}
table.insert(bulletTable, {
Hit = target,
Distance = D,
Cframe = C,
RayObject = R
})

game.ReplicatedStorage.ShootEvent:FireServer(bulletTable, Gun)
local C = Sound:Clone()
C.Parent = Gun
C:Play()
wait(0.05)
Bullet:Remove()
end))
end

Mouse.Button1Down:Connect(function()
Down = true
end)


Mouse.Button1Up:Connect(function()
Down = false
end)

while Run.Stepped:wait() do
if Down == true then
game.ReplicatedStorage.SoundEvent:FireServer(Sound, Gun)
FireLaser(Mouse.Target)
end
end
end)

rem.MouseButton1Down:connect(function()
	local Player = game.Players.LocalPlayer.Name
local Gun = "Remington 870" -- < -- Gun Name
local Run = game:GetService("RunService")

Gun = game.Players[Player].Character[Gun]
local Mouse = game.Players.LocalPlayer:GetMouse()
local Down = false
local Sound = Gun.Handle.FireSound

function CreateRay(Point_A, Point_B)
local Ray = Ray.new(Point_A, (Point_B - Point_A).Unit * (2 ^ 31 - 1))
local Part, Pos = workspace:FindPartOnRay(Ray, game.Players.LocalPlayer.Character)
local Dist = (Point_A - Pos).Magnitude
local CFrame = CFrame.new(Point_A, Pos) * CFrame.new(0, 0, -Dist / 2)

return CFrame, Dist, Ray
end

function FireLaser(target)
coroutine.resume(coroutine.create(function()
local C, D, R = CreateRay(Gun.Muzzle.CFrame.p, target.CFrame.p)
local Bullet = Instance.new("Part", Gun)
Bullet.BrickColor = BrickColor.Yellow()
Bullet.Material = "Neon"
Bullet.Anchored = true
Bullet.CanCollide = false
Bullet.Size = Vector3.new(0.2, 0.2, D)
Bullet.CFrame = C

local bulletTable = {}
table.insert(bulletTable, {
Hit = target,
Distance = D,
Cframe = C,
RayObject = R
})

game.ReplicatedStorage.ShootEvent:FireServer(bulletTable, Gun)
local C = Sound:Clone()
C.Parent = Gun
C:Play()
wait(0.05)
Bullet:Remove()
end))
end

Mouse.Button1Down:Connect(function()
Down = true
end)


Mouse.Button1Up:Connect(function()
Down = false
end)

while Run.Stepped:wait() do
if Down == true then
game.ReplicatedStorage.SoundEvent:FireServer(Sound, Gun)
FireLaser(Mouse.Target)
end
end
end)

m4.MouseButton1Down:connect(function()
	local Player = game.Players.LocalPlayer.Name
local Gun = "M4A1" -- < -- Gun Name
local Run = game:GetService("RunService")

Gun = game.Players[Player].Character[Gun]
local Mouse = game.Players.LocalPlayer:GetMouse()
local Down = false
local Sound = Gun.Handle.FireSound

function CreateRay(Point_A, Point_B)
local Ray = Ray.new(Point_A, (Point_B - Point_A).Unit * (2 ^ 31 - 1))
local Part, Pos = workspace:FindPartOnRay(Ray, game.Players.LocalPlayer.Character)
local Dist = (Point_A - Pos).Magnitude
local CFrame = CFrame.new(Point_A, Pos) * CFrame.new(0, 0, -Dist / 2)

return CFrame, Dist, Ray
end

function FireLaser(target)
coroutine.resume(coroutine.create(function()
local C, D, R = CreateRay(Gun.Muzzle.CFrame.p, target.CFrame.p)
local Bullet = Instance.new("Part", Gun)
Bullet.BrickColor = BrickColor.Yellow()
Bullet.Material = "Neon"
Bullet.Anchored = true
Bullet.CanCollide = false
Bullet.Size = Vector3.new(0.2, 0.2, D)
Bullet.CFrame = C

local bulletTable = {}
table.insert(bulletTable, {
Hit = target,
Distance = D,
Cframe = C,
RayObject = R
})

game.ReplicatedStorage.ShootEvent:FireServer(bulletTable, Gun)
local C = Sound:Clone()
C.Parent = Gun
C:Play()
wait(0.05)
Bullet:Remove()
end))
end

Mouse.Button1Down:Connect(function()
Down = true
end)


Mouse.Button1Up:Connect(function()
Down = false
end)

while Run.Stepped:wait() do
if Down == true then
game.ReplicatedStorage.SoundEvent:FireServer(Sound, Gun)
FireLaser(Mouse.Target)
end
end
end)

ta.MouseButton1Down:connect(function()
	local Player = game.Players.LocalPlayer.Name
local Gun = "Taser" -- < -- Gun Name
local Run = game:GetService("RunService")

Gun = game.Players[Player].Character[Gun]
local Mouse = game.Players.LocalPlayer:GetMouse()
local Down = false
local Sound = Gun.Handle.FireSound

function CreateRay(Point_A, Point_B)
local Ray = Ray.new(Point_A, (Point_B - Point_A).Unit * (2 ^ 31 - 1))
local Part, Pos = workspace:FindPartOnRay(Ray, game.Players.LocalPlayer.Character)
local Dist = (Point_A - Pos).Magnitude
local CFrame = CFrame.new(Point_A, Pos) * CFrame.new(0, 0, -Dist / 2)

return CFrame, Dist, Ray
end

function FireLaser(target)
coroutine.resume(coroutine.create(function()
local C, D, R = CreateRay(Gun.Muzzle.CFrame.p, target.CFrame.p)
local Bullet = Instance.new("Part", Gun)
Bullet.BrickColor = BrickColor.Yellow()
Bullet.Material = "Neon"
Bullet.Anchored = true
Bullet.CanCollide = false
Bullet.Size = Vector3.new(0.2, 0.2, D)
Bullet.CFrame = C

local bulletTable = {}
table.insert(bulletTable, {
Hit = target,
Distance = D,
Cframe = C,
RayObject = R
})

game.ReplicatedStorage.ShootEvent:FireServer(bulletTable, Gun)
local C = Sound:Clone()
C.Parent = Gun
C:Play()
wait(0.05)
Bullet:Remove()
end))
end

Mouse.Button1Down:Connect(function()
Down = true
end)


Mouse.Button1Up:Connect(function()
Down = false
end)

while Run.Stepped:wait() do
if Down == true then
game.ReplicatedStorage.SoundEvent:FireServer(Sound, Gun)
FireLaser(Mouse.Target)
end
end
end)

ak.MouseButton1Down:connect(function()
	local Player = game.Players.LocalPlayer.Name
local Gun = "AK-47" -- < -- Gun Name
local Run = game:GetService("RunService")

Gun = game.Players[Player].Character[Gun]
local Mouse = game.Players.LocalPlayer:GetMouse()
local Down = false
local Sound = Gun.Handle.FireSound

function CreateRay(Point_A, Point_B)
local Ray = Ray.new(Point_A, (Point_B - Point_A).Unit * (2 ^ 31 - 1))
local Part, Pos = workspace:FindPartOnRay(Ray, game.Players.LocalPlayer.Character)
local Dist = (Point_A - Pos).Magnitude
local CFrame = CFrame.new(Point_A, Pos) * CFrame.new(0, 0, -Dist / 2)

return CFrame, Dist, Ray
end

function FireLaser(target)
coroutine.resume(coroutine.create(function()
local C, D, R = CreateRay(Gun.Muzzle.CFrame.p, target.CFrame.p)
local Bullet = Instance.new("Part", Gun)
Bullet.BrickColor = BrickColor.Yellow()
Bullet.Material = "Neon"
Bullet.Anchored = true
Bullet.CanCollide = false
Bullet.Size = Vector3.new(0.2, 0.2, D)
Bullet.CFrame = C

local bulletTable = {}
table.insert(bulletTable, {
Hit = target,
Distance = D,
Cframe = C,
RayObject = R
})

game.ReplicatedStorage.ShootEvent:FireServer(bulletTable, Gun)
local C = Sound:Clone()
C.Parent = Gun
C:Play()
wait(0.05)
Bullet:Remove()
end))
end

Mouse.Button1Down:Connect(function()
Down = true
end)


Mouse.Button1Up:Connect(function()
Down = false
end)

while Run.Stepped:wait() do
if Down == true then
game.ReplicatedStorage.SoundEvent:FireServer(Sound, Gun)
FireLaser(Mouse.Target)
end
end
end)