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
            break;
        case ndBasic:
            nd = malloc(sizeof(Node)); //nalloc(offsetof(Node, un[3])); 
            nd->un = malloc(2*sizeof(Node));
            nd->un[0].n = va_arg(ap, Node *);
            nd->un[1].n = va_arg(ap, Node *);
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

/* extern Wordlist *last_w(Wordlist *word) { */
/*     while (word->next) { */
/*       word = mmem(Wordlist); */
/*       word = word->next; */
/*     } */
/*     return word; */
/* } */

/* extern Wordlist *add_wd(char *w) { */
/*     int i = 0; */
/* 	Wordlist *wl = NULL; */
/* 	if (w != NULL) { */
/* 		wl = mmem(Wordlist); */
/* 		wl->w = w; */
/* 		wl->wl = NULL; */
/* 	} */
/* 	return wl; */
/* } */

/* extern Wordlist *append_wl(Wordlist *wl1, Wordlist *wl2) { */
/* 	Wordlist *ph, *top; */
/* 	if (wl1 == NULL) */
/*        return wl2; */ 
/*     if (wl2 == NULL) */
/*         return wl1; */
/*     for (ph = top = mmem(Wordlist); 1; ph = ph->wl = mmem(Wordlist)) { */ 
/*         ph->w = wl1->w; */
/*         if ((wl1 = wl2->wl) == NULL) */
/*             break; */
/*     } */
/*     ph->wl = wl2; */
/* 	return top; */
/* } */
