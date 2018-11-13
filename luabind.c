/*******************************************************************************
 *  Lua bind manager (LBM)
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#include "luabind.h"

#ifndef MAX_REG_FUNCS
#define MAX_REG_FUNCS 20
#endif

bind_f bindfun[MAX_REG_FUNCS];
int fun_idx = 0;

/* Register libraries in table with specific Lua VM */
void lua_bind(lua_State * L)
{
    int i;
    for (i = 0; i < fun_idx; i++) {
        (bindfun[i]) (L);
    }
}

/* Register bind-functions to be used for by the LBM trampoline, registering
 * each lua-library */
int lua_registerlibrary(bind_f f)
{
    if (fun_idx >= MAX_REG_FUNCS)
        return 0;

    bindfun[fun_idx++] = f;
    return fun_idx;
}
