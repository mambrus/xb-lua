/***
 *  Lua API bindings for demo
 */
/*
 *  Copyright: Michael Ambrus,
 *             Helsinova AB, 2018
 */
#include <xb-lua/lua.h>
#include <xb-lua/lauxlib.h>
#include <xb-lua/lualib.h>
#include <xb-lua/luabind.h>
#include "lapi.h"

#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(X)  ((void)(X))
#endif

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(X) UNUSED_VARIABLE(X)
#endif

#ifndef UNUSED_RETURN_VALUE
#define UNUSED_RETURN_VALUE(X) UNUSED_VARIABLE(X)
#endif

/***
sum of arguments

@function sum
@tparam first argument
@tparam second argument
@treturn sum of the arguments
*/
static int l_sum(lua_State * L)
{
    double arg1 = luaL_checknumber(L, 1);
    double arg2 = luaL_checknumber(L, 2);

    lua_pushnumber(L, arg1 + arg2);

    return 1;
}

/***
print the sum of the arguments

@function printsum
@tparam first argument
@tparam second argument
@treturn print the sum of the arguments
*/
static int l_printsum(lua_State * L)
{
    double sum;
    int narg;

    narg = lua_gettop(L);
    lua_settop(L, narg);

    l_sum(L);

    /* Result is on the stack below input arguments to this func.
     * Fish it up.  */
    sum = luaL_checknumber(L, narg + 1);

    printf("%.4f\n", sum);

    return 0;
}

/* Registration part below */
/* ------------------------------------------------------------------------- */

static const struct luaL_Reg this_lib[] = {
    {"sum", l_sum},
    {"printsum", l_printsum},
    {NULL, NULL}                /* sentinel */
};

/* Lua or LBM uses this to register the library in a VM */
static int do_reg(lua_State * L)
{
    luaL_newlib(L, this_lib);
    return 1;
}

/* LBM uses this to register the library in a VM */
void bind_demo_library(lua_State * L)
{
    luaL_requiref(L, "demo", do_reg, 1);
    lua_pop(L, 1);
}
