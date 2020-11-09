%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "shadosh.h"

void yyerror(char *s);
int yylex();

// Declarations
Node *tree;
%}
%token AND ELSE END IF WHILE
%token OR //PIPE
%token EXIT_CMD CR

%left AND OR '\n'
%left PIPE

// YACC: Definitions
%union {
    struct Node *node_s;
    struct Pipe *pipe_s;
    struct Word *word;
    char *keyword, *c;
    int n;
}

%token <word> WORD
%token <pipe_s> PIPE
%type <node_s> line sa_cmd san_cmd body end cmd exit basic basic_elem

%start shadosh
%%

// Describes expected inputs
shadosh    : line end       { tree = $1; YYACCEPT; }
           | error end      { yyerrok; tree = NULL; YYABORT; } ;

end        : END            { YYABORT; }
           | CR             { YYABORT; };

sa_cmd     : cmd '&'
           | cmd ';' //if NULL then  mk new node     else 
           { $$ = ($1 != NULL ? new_node(ndCompound,$1): $1); };

san_cmd    : sa_cmd
           | cmd '\n'
           { $$ = $1; YYABORT; };

line       : cmd 
           | san_cmd body
           { printf("TESTES\n"); $$ = ($1 != NULL ? new_node(ndBody,$1,$2) : $2); };

body       : cmd 
           | san_cmd body
           { $$ = ($1 == NULL ? $2 : $2 == NULL ? $1 : new_node(ndBody,$1,$2)); };

basic_elem : WORD                   { $$ = new_node(ndWord, $1->w, $1->m, $1->q); };

basic      : basic_elem             { $$ = new_node(ndBasic, $1, NULL); }
           | basic basic_elem       { $$ = new_node(ndBasic, $2, $1); }

cmd        : /* empty */    %prec WHILE      { $$ = NULL; }
           | basic
           | cmd PIPE nlop cmd      { $$ = new_node(ndPipe,$2->l,$2->r,$1,$4); }
           | exit
           ;

exit       : EXIT_CMD CR            {printf("TEST\n"); exit(0); }

nlop       : /* empty */
           | nlop CR;

/* paren      : '(' body ')'   { $$ = $2; }; */

/* iftail     : cmd else               { $$ = $2 != NULL ? new_node(ndElse, $1, $2) : $1;}; */

/* else       :            %prec ELSE  { $$ = NULL; } */
/*            | ELSE nlop cmd          { $$ = $3; }; */

/* keyword    : IF         { $$ = "if"; } */
/*            | WHILE      { $$ = "while"; } */
/*            | NOT        { $$ = "not"; } */
/*            | ELSE       { $$ = "else"; } */
/*            | BANG       { $$ = "!"; } */
/*            | '='        { $$ = "="; } */
/*            ; */

           /* | IF paren nlop iftail   { $$ = new_node(ndIf,$2,$4); } */
           /* | cmd OR nlop cmd        { $$ = new_node(ndOR,$1,$4); } */
           /* | cmd AND nlop cmd       { $$ = new_node(ndAND,$1,$4); } */
%%

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }
