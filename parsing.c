// -- Parsing Library -- //
// --- Header: parsing.h
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parsing.h"
#include "types.h"

#define MAXLIST 100 // Max cmds

cmd_t* parse_args(char* in) {
    char *copy = strndup(in, sizeof(&in));
    for (int i = 0; i < MAXLIST; i++) {
        args[i] = strsep(&in, " ");
        if(args[i] == NULL)
            break;
        if(strlen(args[i]) == 0)
            i--;
    }
}

int parse_semi(char* in, char** cmds) {
    int i;
    for (i = 0; i < MAXLIST; i++) {      
        cmds[i] = strsep(&in ,";");         
        if (cmds[i] == NULL)                
            break;                          
    }
    /* printf("TEST i: %d\n", i); */
    return i;
}

pipes_t* parse_pipes(char* in) {
    pipes_t* pipe; // Struct
    char *copy = strndup(in, sizeof(&in));
    for (int i = 0; i < 2; i++) {
        pipe->args[i] = strsep(&in ,"|");
        if (piped[i] == NULL)
            break;
    }

    if (piped[1] == NULL)
        return 0;
    else
        return 1;
}
