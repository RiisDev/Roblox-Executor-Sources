R"delimiter( 

local bit = {
    badd = newcclosure(function(a, b)
        return a + b
    end),

    bsub = newcclosure(function(a, b)
        return a - b
    end),

    bdiv = newcclosure(function(a, b)
        return a / b
    end),

    bmul = newcclosure(function(a, b)
        return a * b
    end),

    tobit = newcclosure(function(a)
        a = a % (2 ^ 32)
        if a >= 0x80000000 then
            a = a - (2 ^ 32)
        end
        return a
    end),

    bswap = newcclosure(function(a)
        a = a % (2 ^ 32)
        local a = bit32.band(a, 0xff)
        a = bit32.rshift(a, 8)
        local b = bit32.band(a, 0xff)
        a = bit32.rshift(a, 8)
        local c = bit32.band(a, 0xff)
        a = bit32.rshift(a, 8)
        local d = bit32.band(a, 0xff)
        return bit32.tobit(bit32.lshift(bit32.lshift(bit32.lshift(a, 8) + b, 8) + c, 8) + d)
    end),

    ror = newcclosure(function(a, b)
       return bit32.tobit(bit32.rrotate(a % 2 ^ 32, b % 32))
    end),

    rol = newcclosure(function(a, b)
        return bit32.tobit(bit32.lrotate(a % 2 ^ 32, b % 32))
    end)
}

for i, v in bit32 do
    bit[i] = v
end

getgenv().bit = bit
getgenv().bit32 = bit

setreadonly(bit, true)

)delimiter"