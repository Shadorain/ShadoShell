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
%token EXIT_CMD ARGS

%left AND OR '\n'
%left PIPE

// YACC: Definitions
%union {
    struct Node *node;
    struct Pipe *pipe;
    char *keyword, *c;
    int n;
}

%token <pipe> PIPE
%type <node> line paren sa_cmd san_cmd body end cmd iftail else

%start shadosh
%%

// Describes expected inputs
shadosh    : line end       { tree = $1; YYACCEPT; }
           | error end      { yyerrok; tree = NULL; YYABORT; } ;

end        : END            { YYABORT; }
           | '\n'           { YYABORT; } ;

sa_cmd     : cmd '&'
           | cmd ';' //if NULL then  mk new node     else 
           { $$ = ($1 != NULL ? new_node(ndCompound,$1): $1); };

san_cmd    : sa_cmd
           | cmd '\n'
           { $$ = $1; YYABORT; };

line       : cmd 
           | san_cmd body
           { $$ = ($1 != NULL ? new_node(ndBody,$1,$2) : $2); } ;

body       : cmd 
           | san_cmd body
           { $$ = ($1 == NULL ? $2 : $2 == NULL ? $1 : new_node(ndBody,$1,$2)); };

paren      : '(' body ')'   { $$ = $2; };


cmd        : /* empty */    %prec WHILE      { $$ = NULL; }
           | ARGS
           | IF paren nlop iftail   { $$ = new_node(ndIf,$2,$4); }
           | cmd PIPE nlop cmd      { $$ = new_node(ndPipe,$2->l,$2->r,$1,$4); }
           | cmd OR nlop cmd        { $$ = new_node(ndOR,$1,$4); }
           | cmd AND nlop cmd       { $$ = new_node(ndAND,$1,$4); }
           ;

iftail     : cmd else               { $$ = $2 != NULL ? new_node(ndElse, $1, $2) : $1;};

else       :            %prec ELSE  { $$ = NULL; }
           | ELSE nlop cmd          { $$ = $3; };

nlop       :        
           | nlop '\n';

/* keyword    : IF         { $$ = "if"; } */
/*            | WHILE      { $$ = "while"; } */
/*            | NOT        { $$ = "not"; } */
/*            | ELSE       { $$ = "else"; } */
/*            | BANG       { $$ = "!"; } */
/*            | '='        { $$ = "="; } */
/*            ; */

%%

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }
