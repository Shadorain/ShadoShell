%{
#include "shadosh.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void yyerror(char *s);
int yylex();

// Declarations
Wordlist *head,*buf;
int symbol = 0;
%}

%token EXIT_CMD DEL_HIST END
%left ';' '&'

// YACC: Definitions
%union {
    Wordlist *wl;
    char *w, c;
}

%token <w> WORD
%type <wl> end cmd word sa_cmd

%start shadosh
%%

// Describes expected inputs
shadosh    : cmd end        { head = NULL; YYACCEPT; }
           | error end      { yyerrok; head = NULL; YYABORT; }
           ;

end        : END            { YYABORT; }
           | '\n'           { YYABORT; }
           ;

sa_cmd     : word '&' { char **line = arrayify(&head); exec_cmd(line); buf = head; head = NULL; }
           | word ';' { char **line = arrayify(&head); exec_cmd(line); buf = head; head = NULL; }
           | sa_cmd word
           ;

word       : WORD                   { $$ = append(&head, $1); }//print(&head); }
           | word WORD              { $$ = append(&head, $2); }//print(&head); }
           ;

cmd        : /* empty */            { $$ = NULL; }
           | word
           | sa_cmd
           | DEL_HIST               { if(del_hist()==0) printf("Successfully cleared history!\n"); }
           | EXIT_CMD               { exit(0); }
           ;
%%
int main () { // int argc, char* argv[]) {
    char **line;

    init_sh();
    while(1) {
        yyparse();
        line = arrayify(&head);
        clean_list(&head);

        int status = exec_cmd(line);
        if(status == 0)
            hist_write(line);
        free(line);
    }
    return EXIT_STATUS;
}

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }
