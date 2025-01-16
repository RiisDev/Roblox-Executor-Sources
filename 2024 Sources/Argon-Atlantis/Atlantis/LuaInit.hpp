#include <iostream>
#include <Windows.h>
#include <string>

#include "oxorany/oxorany_include.h"
#include "XorStr.hpp"


auto skibidi = R"--(
    setreadonly(getgenv().debug,false)
    getgenv().debug.traceback = getrenv().debug.traceback
    getgenv().debug.profilebegin = getrenv().debug.profilebegin
    getgenv().debug.profileend = getrenv().debug.profileend
    getgenv().debug.getmetatable = getgenv().getrawmetatable
    getgenv().debug.setmetatable = getgenv().setrawmetatable
    getgenv().debug.info = getrenv().debug.info
    getgenv().debug.loadmodule = getrenv().debug.loadmodule

    getgenv().syn_mouse1press = mouse1press
    getgenv().syn_mouse2click = mouse2click
    getgenv().syn_mousemoverel = movemouserel
    getgenv().syn_mouse2release = mouse2up
    getgenv().syn_mouse1release = mouse1up
    getgenv().syn_mouse2press = mouse2down
    getgenv().syn_mouse1click = mouse1click
    getgenv().syn_newcclosure = newcclosure
    getgenv().syn_clipboard_set = setclipboard
    getgenv().syn_clipboard_get = getclipboard
    getgenv().syn_islclosure = islclosure
    getgenv().syn_iscclosure = iscclosure
    getgenv().syn_getsenv = getsenv
    getgenv().syn_getscripts = getscripts
    getgenv().syn_getgenv = getgenv
    getgenv().syn_getinstances = getinstances
    getgenv().syn_getreg = getreg
    getgenv().syn_getrenv = getrenv
    getgenv().syn_getnilinstances = getnilinstances
    getgenv().syn_fireclickdetector = fireclickdetector
    getgenv().syn_getgc = getgc
    
)--";

