// -- Parsing Library -- //
// --- Header: parsing.h
#include <string.h>
#include <stdlib.h>

#include "parsing.h"

#define MAXLIST 100 // Max cmds

void parse_args(char* in, char** args) {
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
    return i;
}

int parse_pipes(char* in, char** piped) {
    for (int i = 0; i < 2; i++) {
        piped[i] = strsep(&in ,"|");
        if (piped[i] == NULL)
            break;
    }

    if (piped[1] == NULL)
        return 0;
    else
        return 1;
}

