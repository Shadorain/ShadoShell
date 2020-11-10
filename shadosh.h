// -- Main Header  -- //

// Includes
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#ifdef CONFIG_H
#include "config.h"
#endif

// Typedefs
typedef long align_t;
typedef struct Node Node;
typedef struct Pipe Pipe;
typedef struct Word Word;
typedef struct Wordlist Wordlist;
typedef struct Block Block;

typedef enum node_e { ndPipe, ndArgs, ndBody, ndIf, ndElse, ndCompound, ndBasic, ndOR, ndAND, ndWord } node_e;

typedef enum bool {
	FALSE, TRUE
} bool;

// Structs
struct Node {
    node_e nt;
    union {
        char *w;
        int i;
        Node *p;
    } *un;
};

struct Pipe {
    int l, r; // Left sect ; Right sect
};

struct Word {
    char *w;
};

struct Wordlist {
    char *w;
    Wordlist *wl;
};

// Defines
#define EOF (-1)
#ifndef NULL
#define NULL 0
#endif
#define nalnew(x) ((x *) nalloc(sizeof(x)))
#define mmem(x) ((x *) malloc(sizeof(x)))
#define offsetof(t, m) ((size_t) (((char *) &((t *) 0)->m) - (char *)0))


/* ------------------------------------------------------------------------ */ 
// nalloc.c
extern Block *newblock(void);
extern void *nalloc(size_t);
extern void nfree(void);
extern void restoreblock(Block *);

// parse.y
extern Node *tree;
extern int yyparse(void);

// node.c
extern Node* new_node(enum node_e, ...);

