// -- Header :: exec.c -- //
#ifndef EXEC_H
#define EXEC_H

void exec_piped(char** args, char** piped);
void exec_args(char** args);
void exec_semi(char** args, char** cmds);

#endif
