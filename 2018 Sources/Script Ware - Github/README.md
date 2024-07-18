# Script-Ware

## PEOPLE ARE OPENING ISSUES ASKING IF THIS IS THE NEW SCRIPT-WARE V2, IT IS NOT. THIS IS -> https://script-ware.com/ <- 
## THIS IS NOT THE SOURCE CODE FOR THE CURRENT SCRIPT-WARE. IT IS THE SOURCE CODE FOR A 2017 PROJECT ALSO NAMED SCRIPT-WARE

This is script-ware's full script execution source, this works if you update the proto struct and addresses, i've included elysians call encryption and eternals shit non-functional trash for the enc (which you could try to fix, however i couldnt and neither could he)

if you update proto struct and addresses, you're able to call closures because op_call's encryption was not changed and this has it updated.

This has practically everything the old source had, i've commented upon the code of which i have used from other people (not much)
but remember! This is **roblox exploiting!** you're not permitted to use a SINGLE piece of code from someone else without a moron screaming that you're incapable

# How does it actually work?
Script-Ware uses a very reputable and widely used method of execution named "Proto Conversion" this method was founded by Brandon (chirality) in mid-ish 2015. When Roblox runs a script, the client (you basically) recieves the bytecode of every script on the game from the server, Roblox then will deserialize the bytecode, this "deserialize" means that the bytecode will be decompressed and converted into a **proto struct**, a proto struct is essientally a struct of information of the lclosure. In order to therefore run scripts, we can create a vanilla lua proto and convert it to roblox's proto format and then feed it to the lua vm which will run it

# Reference to the old source being leaked
"leaking it because he's only made 10% of it"
ahem... that was very rude. script-ware was made by me and quad core, idk where you got _that_ from.
yes sure, it's always used elysians encryption for call/jmp however that does not mean i've only created
a **TENTH** of the applications code. >:(

If you saw the core of it, it's basically that inside a gui, this has the following things:
- pretty much all the enc works apart from eternals broken shit
- callcheck bypass that is functional
- getobjects implementation that works
- current custom implementations to the environment:
- toclipboard
- loadstring
- printconsole
- errorconsole
- writefile
- readfile
- runfile
- getrawmetatable

# Omg, can I use this?
you can take this, make ur own project and sell it for all i care, you'll be helping the ebola type exploiting community
die, and that is what we **ALL** want :)
