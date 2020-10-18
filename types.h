// -- Header :: types.c -- //
#ifndef TYPES_H
#define TYPES_H

typedef struct {
  char* cmd; // First arg
  // IO redirections; redirect[i] should be used as fd i in the child.  A value of -1 indicates no redirect
  int redir[2];
  char* args[]; // args; must be NULL-terminated
} cmd_t;


typedef struct {
  int n_cmds; // cmd count
  cmd_t* cmds[];
} pipes_t;

#endif
