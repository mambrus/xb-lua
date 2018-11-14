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

int m_argc;
char ** m_argv;

int main(int argc, char **argv)
{
    int rc;
	m_argc=argc;
	m_argv=argv;

    rc = start_service_terminal(PORT);

    printf("Hellow world: %d\n", rc);
    while (1) {
        sleep(1);
    }
    return 0;
}
