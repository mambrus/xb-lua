/*******************************************************************************
 *  Lua bind manager (LBM)
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#ifndef luabind_h
#define luabind_h

#include <xb-lua/lua.h>
#include <xb-lua/lauxlib.h>

typedef void (*bind_f) (lua_State * L);

void lua_bind(lua_State * L);
int lua_registerlibrary(bind_f f);

#endif                          //luabind_h
