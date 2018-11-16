--[[
==================
list_funcs.lua
==================

Author: Michael Ambrus
        November 2018
        Helsinova AB

This program lists all functions registered in global table
--]]

for i,v in pairs(_G) do
    if type(v) == "function" then
        print(i,v,debug.getinfo(v))
    end
end
