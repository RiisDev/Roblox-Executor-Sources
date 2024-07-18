# EX-7 Script Executor

EX-7 is the first ever script execution I have created (with Quad Core)
It uses the very notorious yet functional way of bytecode "compilation"
this in broad terms means the "compilation" of roblox bytecode instead of the conversion to vanilla bytecode to roblox bytecode

It uses roblox's own bytecode serializer from the leaked source code of the game (all credits representing relations to the bytecode serializer go to the game developers) to take the Lua inputted and convert it to Roblox bytecode, you can from there use Roblox's deserializer (which is on the client) to convert that bytecode to a Roblox proto, and from there feed it to Roblox's Lua VM

This is however sadly non functional due to a change in the serializer that somehow makes the first constant in the proto completely
ruined. Although fixable, EX-7 would still require the solved encryption of certain Lua opcodes.

I mean, although there are a lot of skidded parts in this, it did function pretty well and i've personally shown where the parts that are copied reside.

any future products from EX-7's ending are not as disgusting and revolting as it so plz dont take this to be all I do
