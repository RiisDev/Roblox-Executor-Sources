R"delimiter( 

repeat wait() until game:IsLoaded()

local insertService = game:GetService("InsertService")

local gui = insertService:LoadLocalAsset("rbxassetid://11481962998")

gui.Enabled = true

gui.Parent = game:GetService("CoreGui")

local aspectRatioConstraint = Instance.new("UIAspectRatioConstraint")

local guiOpen = false

--local DraggableObject = require(gui.Draggable)

local containingFrame = gui.ContainingFrame
local tabContainer = containingFrame.TabContainer
local buttonContainer = tabContainer.ButtonContainer
local buttonFrame = containingFrame.ButtonFrame

local main = containingFrame.Main

local textBox = main.TextBox

local logoButton = tabContainer.LogoButton
local closeBtn = tabContainer.CloseButton

local layout = tabContainer.UIListLayout

--local dragger = DraggableObject.new(tabContainer)

local function close()
	main.Visible = false
	buttonContainer.Visible = false
	closeBtn.Visible = false
	aspectRatioConstraint.Parent = tabContainer
	main.Visible = false
	buttonFrame.Visible = false
	
	layout.Parent = nil

	--dragger:Enable();
end

local function open()
	main.Visible = true
	buttonContainer.Visible = true
	closeBtn.Visible = true
	aspectRatioConstraint.Parent = nil
	main.Visible = true
	buttonFrame.Visible = true
	
	layout.Parent = tabContainer
	
	--dragger:Disable();
	
	tabContainer.Position = UDim2.new(0.5, 0, 0.1, 0)
end

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

buttonFrame.btrash.Activated:Connect(function()
	textBox.Text = ""
end)

buttonFrame.arun.Activated:Connect(newcclosure(function()
	getreg().GTX4CHQYWURD3CR12JF(textBox.Text)
end))


)delimiter"