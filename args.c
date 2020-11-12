#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shadosh.h"

extern Args add_word(Args a, char *w) {
    int i;
    for (i=0; i < MAXLEN; i++) {
        if(a.args[i] != NULL)
            a.args[i] = w;
    }

    return a;
}

Args init_args(Args args) {
    if(args.main != NULL)
        for (int i=0; i < MAXLEN; i++) {
            args.args = calloc(MAXLEN, sizeof(char *));
            args.args[i] = malloc(sizeof(char *));
            args.args[i] = strsep(&args.main, " ");
            if(args.args[i] == NULL)
                break;
            printf("Args [%d]: %s\n", i, args.args[i]);
        }
    return args;
}
