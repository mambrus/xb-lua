/*******************************************************************************
 *  TCP/IP based service and protocol: terminal
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <argutil.h>
#include <pthread.h>
#include <lua/console.h>
#include <liblog/log.h>
#include <liblog/assure.h>
#include <tcp-tap/server.h>
#include "service_terminal.h"

struct sockdef {
    int port;
    const char *ifname;
};

struct service {
    struct sockdef sockdef;
    pthread_t thread;
};

static void *connection_manager(void *inarg);
static void *terminal_service(void *inarg);

static void *connection_manager(void *inarg)
{
    int s, fd;
    pthread_t t;
    struct service *tservice = (struct service *)inarg;
    struct service service;

    /* Make deep-copy inarg to corr stack variable, then free original. This
     * prevents memory leaks if thread is killed either by accident or
     * intentionally. IOW: just a better management of memory overall */
    memcpy(&service, tservice, sizeof(struct service));
    free(tservice);
    tservice = NULL;

    LOGD("Connection manager thread starts [if:port] : [%s:%d]\n",
         service.sockdef.ifname, service.sockdef.port);
    s = init_server(service.sockdef.port, service.sockdef.ifname);
    LOGD("Listening at socket (%d)\n", s);

    while (1) {
        ASSERT((fd = open_server(s)) >= 0);
        LOGD("Connection opened (%d)\n", fd);

        pthread_create(&t, NULL, terminal_service, (void *)((intptr_t) fd));
    }

    return NULL;
}

static void *terminal_service(void *inarg)
{
    int wpid, status;
    int childpid = 0;
    char **lua_argv;
    int lua_argc;
    int fd = (long)inarg;

    LOGD("%s operates on fd (%d)\n", __func__, fd);

    ASSERT((childpid = fork()) >= 0);
    if (childpid == 0) {
        close(0);
        dup(fd);
        close(1);
        dup(fd);
        close(2);
        dup(fd);

        lua_argc = args2argv(&lua_argv, "lua", "--");
        lua_main(lua_argc, lua_argv);
        free(lua_argv);
    }
    /* Parent executes this */
    do {
        wpid = waitpid(childpid, &status, WUNTRACED);
        LOGD("CHILD: Wants to exit with status %d\n", status);
        ASSERT(wpid >= 0);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    LOGD("CHILD: Exits with status %d\n", status);

    return NULL;
}

int start_service_terminal(int port, const char *ifname)
{
    struct service *service;

    /* Regardless of how liblog is configured, make sure all output is to
     * syslog and not to stderr */
    log_syslog_config(0);
    assert(service = malloc(sizeof(struct service)));
    service->sockdef.port = port;
    service->sockdef.ifname = ifname;

    return pthread_create(&service->thread, NULL, connection_manager, service);
}
