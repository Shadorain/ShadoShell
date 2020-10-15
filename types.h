// -- Header :: types.c -- //
#ifndef UTILS_H
#define UTILS_H

typedef struct {
  char* cmd; // First arg
  // IO redirections; redirect[i] should be used as fd i in the child.  A value of -1 indicates no redirect
  int redirect[2];
  char* args[]; // args; must be NULL-terminated
} cmd_struct;


typedef struct {
  int n_cmds; // num of cmds
  cmd_struct* cmds[];
} pipeline_struct;

#endif
