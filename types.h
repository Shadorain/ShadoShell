// -- Header :: types.c -- //
#ifndef TYPES_H
#define TYPES_H
  // IO redirections; redir[i] should be used as fd i in the child.  A value of -1 indicates no redirect

typedef struct {
  char* main_cmd; // First arg
  int redir[2], builtin;
  char* args[]; 
} cmd_t;


typedef struct {
  int cmd_n, multicmd_n; // cmd, multi count
  cmd_t** m_cmds;
  cmd_t** cmds;
  /* cmd_t* cmds[]; */
} pipes_t;

#endif
