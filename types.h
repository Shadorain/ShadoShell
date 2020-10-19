// -- Header :: types.c -- //
#ifndef TYPES_H
#define TYPES_H

typedef struct {
  char* main_cmd; // First arg
  // IO redirections; redir[i] should be used as fd i in the child.  A value of -1 indicates no redirect
  int redir[2], builtin;
  char* args[]; // args; must be NULL-terminated
} cmd_t;


typedef struct {
  int cmd_n, multicmd_n; // cmd count
  cmd_t* cmds[];
} pipes_t;

#endif
