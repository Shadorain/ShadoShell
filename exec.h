// -- Header :: exec.c -- //
#ifndef EXEC_H
#define EXEC_H
#include "types.h"
#include "shadosh.h"

int exec_cmd(cmd_t* cmd, int pipe_n, int (*piped)[2]);

#endif
