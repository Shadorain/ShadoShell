// -- Main Header  -- //

// Includes
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#ifdef CONFIG_H
#include "config.h"
#endif

// Typedefs
typedef struct Node Node;
typedef struct Pipe Pipe;
typedef struct Word Word;
typedef struct Wordlist Wordlist;

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
    char *w, *m;
    bool q;
};

struct Wordlist {
    char *w, *m;
    Wordlist *wl;
};

// Defines
#define EOF (-1)
#ifndef NULL
#define NULL 0
#endif
#define malnew(x) ((x *) malloc(sizeof(x)))


/* ------------------------------------------------------------------------ */ 
// parse.y
extern Node *tree;
extern int yyparse(void);

// node.c
extern Node* new_node(enum node_e, ...);

