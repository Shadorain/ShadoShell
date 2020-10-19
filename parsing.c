// -- Parsing Library -- //
// --- Header: parsing.h
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parsing.h"
#include "types.h"
#include "builtins.h"

#define MAXLIST 1024 // Max cmds
#define TOK_SEP " \t\n\r"

char* check_tok(char** dup) {
    char *tok;
    while ((tok = strsep(dup, TOK_SEP)) && !*tok);

    return tok;
}

cmd_t* parse_args(char* in) {
    int i = 0;
    char* tok;
    char* dup = strndup(in, MAXLIST);

    cmd_t* cmd_s = calloc(sizeof(cmd_t) + MAXLIST * sizeof(char*), 1);

    while((tok = check_tok(&dup)))
        cmd_s->args[i++] = tok;

    cmd_s->main_cmd = cmd_s->args[0];
    cmd_s->redir[0] = cmd_s->redir[1] = -1;

    if(builtin_handler(cmd_s->args))
        cmd_s->builtin = 1;

    return cmd_s;
}

/* int parse_semi(char* in, char** cmds) { */
/*     int i; */
/*     for (i = 0; i < MAXLIST; i++) { */      
/*         cmds[i] = strsep(&in ,";"); */         
/*         if (cmds[i] == NULL) */                
/*             break; */                          
/*     } */
/*     /1* printf("TEST i: %d\n", i); *1/ */
/*     return i; */
/* } */

pipes_t* parse_pipes(char* in) {
    int cmd_n = 0, i=0;
    char *cmds;
    pipes_t* pipe_s; // Struct
    char* dup = strndup(in, MAXLIST);
    
    for(char* c = dup; *c; c++) // Counts amount of pipes
        if (*c == '|') ++cmd_n;
    ++cmd_n;

    pipe_s = calloc(sizeof(pipes_t) + cmd_n * sizeof(cmd_t*), 1);
    pipe_s->cmd_n = cmd_n;

    while((cmds = strsep(&dup ,"|"))) // Parses the pipes
        pipe_s->cmds[i++] = parse_args(cmds);

    return pipe_s;
}

// -- Print tests -- {{{
void print_command(cmd_t* cmd_s) {
    char** arg = cmd_s->args;
    int i = 0;
   
    fprintf(stderr, "progname: %s\n", cmd_s->main_cmd);
   
    for (i = 0, arg = cmd_s->args; *arg; ++arg, ++i)
      fprintf(stderr, " args[%d]: %s\n", i, *arg);
}

void print_pipeline(pipes_t* pipe_s) {
    cmd_t** cmd = pipe_s->cmds;
    int i = 0;

    fprintf(stderr, "n_cmds: %d\n", pipe_s->cmd_n);

    for (i = 0; i < pipe_s->cmd_n; ++i) {
      fprintf(stderr, "cmds[%d]:\n", i);
      print_command(cmd[i]);
    }
}
// }}}
