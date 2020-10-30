// -- Header: parsing.c -- //
#ifndef PARSING_H
#define PARSING_H

#include "types.h"

cmd_t* parse_args(char* in);
pipes_t* parse_pipes(char* in);
ctrl_t* parse_multi(char* in);
void print_pipeline(pipes_t* pipe);

#endif
