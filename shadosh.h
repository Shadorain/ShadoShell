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
typedef struct Args Args;
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
        Node *n;
    } *un;
};

struct Pipe {
    int l, r;
};

struct Args {
    char *main, **args;
};

/* struct Word { */
/*     char *w; */
/* }; */

struct Wordlist {
    char *w;
    Wordlist *next;
};

// Defines
#define EOF (-1)
#define MAXLEN 1024
#define EXIT_STATUS 0
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
extern void init_sh();

// node.c
extern Node* new_node(enum node_e, ...);
extern Wordlist *last_w(Wordlist *word);
/* extern Wordlist *add_wd(char *w); */

// args.c
extern Args add_word(Args a, char *w);
extern Args init_args(Args args);

// exec.c
extern int exec_cmd(Args args);
