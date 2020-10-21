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

pipes_t* init_pipes_t(int cmd_n, int multicmd_n) {
    pipes_t* pipe_s = (pipes_t*)malloc(sizeof(pipe_s));
    pipe_s = calloc(sizeof(pipes_t) + multicmd_n + cmd_n * (2*sizeof(cmd_t*)), 1);
    pipe_s->cmd_n = cmd_n;
    pipe_s->multicmd_n = multicmd_n;
    /* pipe_s->m_cmds = (pipes_t**)malloc(sizeof(pipes_t*) * cmd_n * multicmd_n); */
    
    /* pipe_s->m_cmds = //calloc(sizeof(pipes_t) + cmd_n + multicmd_n * sizeof(cmd_t*), 1); // Alloc m_cmds */
    /* for(int j=0;j<sizeof(pipe_s->m_cmds); j++) */
    pipe_s->m_cmds = (cmd_t**)calloc(multicmd_n, multicmd_n * sizeof(cmd_t*));
    for(int j=0;j<sizeof(pipe_s->m_cmds); j++)
        pipe_s->m_cmds[j]=(cmd_t*)malloc(sizeof(cmd_t));

    return pipe_s;
}

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

pipes_t* parse_pipes(char* in) {
    int cmd_n = 0, multicmd_n = 0, i=0;
    char *cmds, *m_cmds;
    /* pipes_t* pipe_s; // Struct */
    char* dup = strndup(in, MAXLIST);
    
    for(char* c = dup; *c; c++) // Counts amount of pipes
        if (*c == '|') ++cmd_n;
        else if (*c == ';') ++multicmd_n;
    ++cmd_n;
    ++multicmd_n;
    
    pipes_t* pipe_s = init_pipes_t(cmd_n, multicmd_n); // init struct

    /* if (cmd_n > 1 || multicmd_n > 1) { */
    if (cmd_n >= 1)
        while((cmds = strsep(&dup ,"|"))) // Parses the pipes
            pipe_s->cmds[i++] = parse_args(cmds);
    if (multicmd_n > 1)
        while((m_cmds = strsep(&dup ,";"))) // Parses the semi
            pipe_s->m_cmds[i++] = parse_args(m_cmds);
    /* } else */
    /*     pipe_s->cmds[i] = parse_args(cmds); */

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
