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
// TOK_REDIR "< > <> >| << >> <& &>"
// TOK_CTRL "& && ( ) ; ;; \n | ||"
#define TOK_CTRL "&();\n"

// -- Initialize Memory -- {{{
pipes_t* init_pipes_t(int cmd_n) {
    pipes_t* pipe_s = (pipes_t*)malloc(sizeof(pipe_s));
    pipe_s->cmd_n = cmd_n;
    pipe_s->cmds = (cmd_t**)calloc(cmd_n, cmd_n * sizeof(cmd_t*));
    for(int j=0;j<sizeof(pipe_s->cmds); j++)
        pipe_s->cmds[j]=(cmd_t*)malloc(sizeof(cmd_t));

    return pipe_s;
}

ctrl_t* init_ctrl_t(int cmd_n) {
    ctrl_t* ctrl_s = (ctrl_t*)malloc(sizeof(ctrl_s));
    ctrl_s->cmd_n = cmd_n;
    ctrl_s->ccmds = (cmd_t**)calloc(cmd_n, cmd_n * sizeof(cmd_t*));
    for(int j=0;j<sizeof(ctrl_s->ccmds); j++)
        ctrl_s->ccmds[j]=(cmd_t*)malloc(sizeof(cmd_t));

    return ctrl_s;
}
//}}}
// -- Tokenizing -- {{{
char* split_space(char** dup) {
    char *tok;
    while ((tok = strsep(dup, TOK_SEP)) && !*tok);

    return tok;
}

char* split_ctrl(char** dup) {
    char *tok;
    while ((tok = strsep(dup, TOK_CTRL)) && !*tok);
    printf("TOK: %s\n", tok);

    return tok;
}
//}}}
// -- Parse Arguments -- {{{
cmd_t* parse_args(char* in) {
    int i = 0;
    char *tok, *dup = strndup(in, MAXLIST);

    cmd_t* cmd_s = calloc(sizeof(cmd_t) + MAXLIST * sizeof(char*), 1);

    while((tok = split_space(&dup)))
        cmd_s->args[i++] = tok;

    cmd_s->main_cmd = cmd_s->args[0];
    cmd_s->redir[0] = cmd_s->redir[1] = -1;

    if(builtin_handler(cmd_s->args))
        cmd_s->builtin = 1;

    return cmd_s;
}
//}}}
// -- Parse Pipes -- {{{
pipes_t* parse_pipes(char* in) {
    int cmd_n=0, i=0;
    char *cmds;
    char* dup = strndup(in, MAXLIST);
    
    for(char* c = dup; *c; c++) // Counts amount of pipes
        if (*c == '|') ++cmd_n;
    ++cmd_n;
    
    pipes_t* pipe_s = init_pipes_t(cmd_n); // init struct

    while((cmds = strsep(&dup ,"|"))) // Parses the pipes
        pipe_s->cmds[i++] = parse_args(cmds);

    return pipe_s;
}
//}}}
// -- Parse Control -- {{{
ctrl_t* parse_multi(char* in) {
    int i=0, cmd_n=0;
    char *cmds, *dup = strndup(in, MAXLIST);

    for(char* c = dup; *c; c++) // Counts amount of pipes
        if (*c == '&' || *c == '(' || *c == ')' || *c == ';') ++cmd_n;
    ++cmd_n;
    
    ctrl_t* ctrl_s = init_ctrl_t(cmd_n);

    while((cmds = strsep(&dup ,";"))) // Parses the semi
        ctrl_s->ccmds[i++] = parse_args(cmds);

    return ctrl_s;
}
// }}}
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
//----------------------------------------------------------------------------------------
