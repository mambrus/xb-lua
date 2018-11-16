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
#include <xb-lua/lauxlib.h>
#include <xb-lua/service_terminal.h>
#include <xb-lua/luabind.h>
#include <lua/console.h>
#include <string.h>
#include <tcp-tap/server.h>
#include "lapi.h"
#undef  NDEBUG
#include <assert.h>

#define PORT 2772
#define HIF "@ANY@"

int main(int argc, char **argv)
{
    int rc;

    rc = start_service_terminal(PORT);

    printf("Terminal service running at localhost under port %d: %d\n", PORT,
           rc);
	assert(lua_registerlibrary(bind_demo_library) > 0);
#include <xb-lua/lauxlib.h>
    printf("In paralell to stdio session starting now...\n");

    while (!feof(stdin)) {
        char **lua_argv;
        int lua_argc;

        if (argc > 1) {
            char args_buffer[PATH_MAX] = { 0 };
            int i, rc;

            for (i = 1; i < argc; i++) {
                strncat(&args_buffer[strnlen(args_buffer, PATH_MAX)], argv[i],
                        NAME_MAX);
                args_buffer[strnlen(args_buffer, PATH_MAX)] = ' ';
            }
            printf("lua %s\n", args_buffer);

            lua_argc = args2argv(&lua_argv, "lua", args_buffer);
            rc = lua_main(lua_argc, lua_argv);
            free(lua_argv);

            return rc;
        } else {
            printf("lua interactive:\n");

            lua_argc = args2argv(&lua_argv, "lua", "--");
            lua_main(lua_argc, lua_argv);
            free(lua_argv);
        }
    }
    return 0;
}
