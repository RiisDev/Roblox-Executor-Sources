# Tervylla

This project, Tervylla achieves script execution by using bytecode conversion, converting vanilla lua bytecode to Roblox's bytecode.

How does this achieve script execution? This uses Roblox's serializer and serializes
vanilla lua and converts it to Roblox's bytecode, then deserializes the converted bytecode.
Then it exports a proto, and it sends the proto to Roblox's Virtual Machine, achieving "full" script execution.

However, this has it's drawbacks from using this method of script execution, it can get patched really easily if Roblox decides to change the encryptions, will break bytecode conversion and require new encryptions to work again. 

If you want to know more about bytecode conversion, read this: http://louka.io/BytecodeConversion.pdf
