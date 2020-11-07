#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shadosh.h"

extern Node* new_node(enum node_e nt, ...) {
    va_list ap;
    Node* nd;

    va_start(ap, nt); //Requires the last fixed parameter (to get the address)
    switch (nt) {
        default:
            printf("Welp this shouldn't be happening, please put in a report for this!\n");
            exit(1);
        case ndArgs: case ndBody:
            nd = malloc(sizeof(Node));
            nd->un[0].p = va_arg(ap, Node *);
            nd->un[1].p = va_arg(ap, Node *);
            printf("ARGS: %s\n", nd->un[0].p->un[0].cmds);
        case ndPipe:
            break;
    }
    va_end(ap);

    return nd;
}
