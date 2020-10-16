// -- Builtins Library -- //
// --- Header: builtins.h --- //
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "shadosh.h"

int builtin_handler(char** args) {
    int count = 4, checkArgs = 0;
    char* cmdArr[count];
    char* user;

    cmdArr[0] = "cd";
    cmdArr[1] = "exit";
    cmdArr[2] = "help";
    cmdArr[3] = "test";

    for (int i = 0; i < count; i++)
        if (strcmp(args[0], cmdArr[i]) == 0) {
            checkArgs = i + 1;
            break;
        }
    
    switch (checkArgs) {
        case 1:
            chdir(args[1]);
            return 1;
        case 2:
            exit(0);
        case 3:
            /* usage(); */
            /* help_menu(); */
            return 1;
        case 4:
            user = getenv("USER");
            printf("\n Test %s Test.", user);
            return 1;
        default:
            break;
    }

    return 0;
}
