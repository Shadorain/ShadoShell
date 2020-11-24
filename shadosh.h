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

struct Wordlist {
    char *word;
    char op[2];
    Wordlist *next;
};

// Defines
#define home getenv("HOME")
#define hist_file "/home/shadow/.shado_hist"
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
// parse.y
extern int yyparse(void);
extern void init_sh();

// node.c
/* extern Node* new_node(enum node_e, ...); */
/* extern Wordlist *last_w(Wordlist *word); */
/* extern Wordlist *add_wd(char *w); */

// list.c
Wordlist *append(Wordlist** head_ref, char *w);
void print(Wordlist **head_ref);
void clean_list(Wordlist **head_ref);
char **arrayify(Wordlist **head_ref);
char *stringify(char **arr, int len);

// exec.c
extern int exec_cmd(char **args);

// history.c
int hist_write(char **line);
int del_hist();
