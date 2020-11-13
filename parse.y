%{
#include "shadosh.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void yyerror(char *s);
int yylex();

// Declarations
Wordlist *head;
%}

%token EXIT_CMD CR END

// YACC: Definitions
%union {
    Wordlist *wl;
    char *w;
}

%token <w> WORD
%type <wl> line end cmd exit word

%start shadosh
%%

// Describes expected inputs
shadosh    : line end       { head = $1; YYACCEPT; }
           | error end      { yyerrok; head = NULL; YYABORT; } ;

end        : END            { YYABORT; }
           | CR             { YYABORT; };

line       : cmd ;

word       : WORD                   { $$ = new_node(ndWord, $1); };

cmd        : /* empty */
           | exit
           ;

exit       : EXIT_CMD CR            {printf("TEST\n"); exit(0); }
%%

int main () { // int argc, char* argv[]) {
    init_sh();
    while(1) {
        yyparse();
    }
    return EXIT_STATUS;
}

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }

