// -- Header: parsing.c -- //
#ifndef PARSING_H
#define PARSING_H

#include "types.h"

cmd_t* parse_args(char* in);
pipes_t* parse_pipes(char* in);
pipes_t* parse_multi(char* in, pipes_t* pipe_s);
void print_pipeline(pipes_t* pipe);

#endif
