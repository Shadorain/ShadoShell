// -- Header :: types.c -- //
#ifndef TYPES_H
#define TYPES_H
  // IO redirections; redir[i] should be used as fd i in the child.  A value of -1 indicates no redirect

typedef struct {
  char* main_cmd; // First arg
  int redir[2], builtin;
  char *args[]; 
} cmd_t;

typedef struct {
  int cmd_n;
  cmd_t** cmds;
} pipes_t;

typedef struct {
  int cmd_n;
  cmd_t **ccmds;//, **acmds, **pocmds, **pccmds;
} ctrl_t;

#endif
