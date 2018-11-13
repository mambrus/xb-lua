/*******************************************************************************
 *  Lua bind manager (LBM)
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#ifndef luabind_h
#define luabind_h

#include "lua.h"
#include "lauxlib.h"

typedef void (*bind_f) (lua_State * L);

int lua_main(int argc, char **argv);

void lua_bind(lua_State * L);
int lua_registerlibrary(bind_f f);

#endif                          //luabind_h
