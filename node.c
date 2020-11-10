#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shadosh.h"

extern Node* new_node(enum node_e nt, ...) {
    va_list ap;
    Node* nd;
    char *w;

    va_start(ap, nt); //Requires the last fixed parameter (to get the address)
    printf("%d\n", nt);
    switch (nt) {
        default:
            printf("Welp this shouldn't be happening, please put in a report for this!\n");
            exit(1);
        case ndWord:
            nd = malloc(sizeof(Node)); //nalloc(offsetof(Node, un[3])); 
            nd->un = calloc(4, sizeof(Node));
            nd->un[0].w = va_arg(ap, char *);
            /* nd->un[2].i = va_arg(ap, int); */
            break;
        case ndBasic:
            nd = malloc(sizeof(Node)); //nalloc(offsetof(Node, un[3])); 
            nd->un = malloc(sizeof(Node));
            nd->un[0].w = va_arg(ap, char *);
            nd->un[1].w = va_arg(ap, char *);
            /* nd->un[2].i = va_arg(ap, int); */
            break;
        /* case ndArgs: case ndBody: */
        /*     nd = nalloc(sizeof(Node)); */
        /*     nd->un[0].p = va_arg(ap, Node *); */
        /*     nd->un[1].p = va_arg(ap, Node *); */
            /* printf("ARGS: %s\n", nd->un[0].p->un[0].cmds); */
        case ndPipe:
            break;
    }
    va_end(ap);

    return nd;
}

extern Wordlist *add_wd(char *w) {
	Wordlist *wl = NULL;
	if (w != NULL) {
		wl = mmem(Wordlist);
		wl->w = w;
		wl->wl = NULL;
	}
	return wl;
}
