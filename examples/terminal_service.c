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
#include <tcp-tap/clientserver.h>
#include "lapi.h"
#undef  NDEBUG
#include <assert.h>

#define PORT 2772
#define IFNAME "@ANY@"

int main(int argc, char **argv)
{
    assert(lua_registerlibrary(bind_demo_library) > 0);
    assert(start_service_terminal(PORT, IFNAME) == 0);

    printf("Terminal service running in paralell at [%s:%d]\n\n", IFNAME, PORT);

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

            lua_argc = args2argv(&lua_argv, "lua", args_buffer);
            rc = lua_main(lua_argc, lua_argv);
            free(lua_argv);

            return rc;
        } else {
            lua_argc = args2argv(&lua_argv, "lua", "--");
            lua_main(lua_argc, lua_argv);
            free(lua_argv);
        }
    }
    return 0;
}
