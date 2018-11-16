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
#include <liblog/log.h>
#include <tcp-tap/server.h>
#include "service_terminal.h"
#undef  NDEBUG
#include <assert.h>

void *terminal_service(void *inarg)
{
    int s, fd;
    char **lua_argv;
    int lua_argc;
    int p_terminal = (long)inarg;

    LOGD("Console child service starts and will use socket[port] : [%d]\n",
         p_terminal);

    s = init_server(p_terminal, "@ANY@");
    LOGD("Console service started at socket[port]: %d[%d]\n", s, p_terminal);

    while (1) {
        fd = open_server(s);
        fprintf(stderr,
                "Console servicing fd:%d for socket[port]: %d[%d]\n", fd, s,
                p_terminal);

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
    }
}

int start_service_terminal(int p_terminal)
{
    int childpid = 0;

    /* Regardless of how liblog is configured, make sure all output is to
     * syslog and not to stderr */
    log_syslog_config(0);

#ifdef SERVICE_CONSOLE_NOT_FORKED
    /*I.e. if not forked, then threaded */

    pthread_t t_thread;
    int rc;

    childpid = 1;
    rc = pthread_create
        (&t_thread, NULL, terminal_service, (void *)((intptr_t) p_terminal));
    if (rc != 0)
#else
    assert((childpid = fork()) >= 0);
#endif

    if (childpid == 0) {
        /* Child executes this */
        terminal_service((void *)((intptr_t) p_terminal));

        /* Should never execute */
        perror("exec error");
        exit(-1);
    }

    /* Parent executes this */
    return childpid;
}
