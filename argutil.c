/*******************************************************************************
 *  Helpers for handling argv, argc
 *  Copyright: Michael Ambrus, 2016
 ******************************************************************************/
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "argutil.h"

/* Allocate new_argv from string. Result must be freed when done. Side-effect:
   In-variable "string" will get all spaces converted to \0 */
int args2argv(char ***new_argv, char *cmd, char *args)
{
    int f, g, i, j = strlen(args), k;

    /* TBD: args could be trimmed and cleaned from duplicate spaces. It
       should work nevertheless. */

    /* Find out how many argv's there are in args. Result to be used to
       allocate memory for new argv. */

    /* Ignore leading white-spaces. First argv is at position stored in k */
    for (k = 0; k < j && args[k] == ' '; k++)
        args[k] = '\0';

    for (i = k, g = 0; i < j; i++) {
        /* Test below takes care of not counting intermediate spaces */
        if (args[i] == ' ') {
            if ((args[i + 1] != ' ') && (args[i + 1] != '\0')) {
                g++;
            }
        }
    }

    /*The last one has no space */
    g++;

    /* Add one more for "exec name" */
    g++;

    /* allocate the new argv */
    *new_argv = calloc(g, sizeof(char *));
    (*new_argv)[0] = cmd;
    (*new_argv)[1] = &args[k];

    /*Populate the new argv */
    for (i = k, f = 2; i < j; i++) {
        if (args[i] == ' ') {
            args[i] = '\0';
            if ((args[i + 1] != ' ') && (args[i + 1] != '\0')) {
                (*new_argv)[f] = &args[i + 1];
                f++;
            }
        }
    }
    return g;
}
