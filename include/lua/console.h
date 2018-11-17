/*******************************************************************************
 *  Lua console (luac)
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#ifndef lua_console_h
#define lua_console_h

#include "lua.h"
#include "lauxlib.h"

int lua_main(int argc, char **argv);
int start_service_terminal(int port, const char *ifname);

#endif                          //lua_console_h
