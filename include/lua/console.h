/*******************************************************************************
 *  Lua console (luac)
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#ifndef lua_console_h
#define lua_console_h

#include "lua.h"
#include "lauxlib.h"

typedef void (*bind_f) (lua_State * L);

int lua_main(int argc, char **argv);

void lua_bind(lua_State * L);

#endif                          //lua_console_h
