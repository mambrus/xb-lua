/*******************************************************************************
 *  Lua API(s)
 *  Copyright: Michael Ambrus
 *             Helsinova AB, 2018
 ******************************************************************************/
/*
 * Bind functions to be registered with the LBM (Lua bind manager)
 */

#ifndef lua_api_h
#define lua_api_h

#include <xb-lua/lua.h>
#include <xb-lua/lauxlib.h>

void bind_demo_library(lua_State * L);

#endif                          //lua_api_h
