// -- Header :: exec.c -- //
#ifndef EXEC_H
#define EXEC_H
#include "types.h"
#include "shadosh.h"

int exec_cmd(cmd_t* cmd, int pipe_n, int (*piped)[2]);
/* pid_t fork_cmd(cmd_t* cmd_s, int pipe_n, int (*piped)[2]); */
/* void exec_piped(char** args, char** piped); */
/* void exec_semi(char** args, char** cmds); */

#endif
