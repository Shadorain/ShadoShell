// -- Header :: shadosh.c -- //
void close_pipes(int n_pipes, int (*piped)[2]);

// Includes
#include <sys/stat.h>
#include <assert.h>

// Typedefs
typedef struct Node Node;
typedef struct Pipe Pipe;

typedef enum node_e { ndPipe, ndArgs, ndBody, ndIf, ndElse, ndCompound, ndOR, ndAND } node_e;

// Structs
struct Node {
    node_e nt;
    union {
        char *cmds;
        int idk;
        Node *p;
    } *un;
};

struct Pipe {
    int l, r; // Left sect ; Right sect
};


/* ------------------------------------------------------------------------ */ 
// parse.y
extern Node *tree;
extern int yyparse(void);

// node.c
extern Node* new_node(enum node_e, ...);

