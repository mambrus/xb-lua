/*******************************************************************************
 *  TCP/IP based service and protocol: terminalinal
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <argutil.h>
#include <pthread.h>
#include <lua/console.h>
#include <tcp-tap/server.h>
#include <liblog/log.h>
#include "service_terminal.h"
#undef  NDEBUG
#include <assert.h>

void *terminal_service(void *inarg)
{
    int s, fd;
    char **lua_argv;
    int lua_argc;
    int p_console = (long)inarg;
   
    LOGI("Console child (ROMAN) service starts and will use socket[port] : [%d]\n",
         p_console);

    s = init_server(p_console, "@ANY@");
    LOGI("Console service started at socket[port]: %d[%d]\n", s, p_console);

    while (1) {
        LOGI("Console service is waiting for connection");
        fd = open_server(s);
        LOGI("Console service accepted connection");
        // Make a copy of existing standard streams
        int stdin_copy = dup(0);
        int stdout_copy = dup(1);
        int stderr_copy = dup(2);

        /* Note that if run as a thread, all other threads usage of
           stdio will also interact with the same socket */
        close(0);
        dup(fd);
        close(1);
        dup(fd);
        close(2);
        dup(fd);

        while (!feof(stdin)) {
            lua_argc = args2argv(&lua_argv, "lua", "--");
            lua_main(lua_argc, lua_argv);
            free(lua_argv);
        }

        // Copy back original standard streams
        dup2(stdin_copy, 0);
        dup2(stdout_copy, 1);
        dup2(stderr_copy, 2);

        // Close copies as not needed anymore
        close(stdin_copy);
        close(stdout_copy);
        close(stderr_copy);

        // Clean errors
        clearerr(stdin);
        clearerr(stdout);
        clearerr(stderr);
    }
}

int start_service_terminal(int p_terminal)
{
    pthread_t t_thread;
    int rc = -1;

    signal(SIGPIPE, SIG_IGN);

    rc = pthread_create
        (&t_thread, NULL, terminal_service, (void *)((intptr_t) p_terminal));

    /* Parent executes this */
    return rc;
}
