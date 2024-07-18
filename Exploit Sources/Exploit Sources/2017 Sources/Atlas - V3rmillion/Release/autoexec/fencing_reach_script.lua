------------Fencing Reach Script------------
local foil=game.Players.LocalPlayer.Backpack.Foil
a=Instance.new("SelectionBox",foil.Handle)
a.Adornee=foil.Handle
foil.Handle.Size=Vector3.new(0.2,0.2,60)
foil.Equipped:connect(function()
while wait() do
game.Workspace[game.Players.LocalPlayer.Name].Foil.GripPos=Vector3.new(0,0,-30)
end
end)