/*******************************************************************************
 *  TCP/IP based service and protocol: terminalinal
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <argutil.h>
#include <pthread.h>
#include <lua/console.h>
#include <tcp-tap/server.h>
#include "service_terminal.h"
#undef  NDEBUG
#include <assert.h>

#define PORT 2772
#define HIF "@ANY@"

int main(int argc, char **argv)
{
    int rc;
    char **lua_argv;
    int lua_argc;

    rc = start_service_terminal(PORT);

    printf("Terminal service running at localhost under port %d: %d\n", PORT,
           rc);
    printf("In paralell to stdio session starting now...\n");
    while (1) {
        while (!feof(stdin)) {
            lua_argc = args2argv(&lua_argv, "lua", "--");
            lua_main(lua_argc, lua_argv);
            free(lua_argv);
        }
    }
    return 0;
}
