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

%token EXIT_CMD END

// YACC: Definitions
%union {
    Wordlist *wl;
    char *w, c;
}

%token <w> WORD
%type <wl> end cmd word

%start shadosh
%%

// Describes expected inputs
shadosh    : cmd end        { head = NULL; YYACCEPT; }
           | error end      { yyerrok; head = NULL; YYABORT; }
           ;

end        : END            { YYABORT; }
           | '\n'           { YYABORT; }
           ;

word       : WORD                   { $$ = append(&head, $1); print(&head); }
           | word WORD              { $$ = append(&head, $2); print(&head); }
           ;

cmd        : /* empty */ { $$ = NULL; }
           | word
           | EXIT_CMD               { printf("TEST\n"); exit(0); }
           ;
%%

int main () { // int argc, char* argv[]) {
    FILE *hist = NULL;
    char **line, *str, *hf = strcat(home, hist_file);
    int len = 0;

    init_sh();
    while(1) {
        yyparse();
        line = arrayify(&head);
        clean_list(&head);

        while (line[++len] != NULL);
        printf("LEN: %d\n",len);
        str = stringify(line, len);

        hist = fopen(hf,"a");
        if(hist == NULL)
            printf("Failed to write to history file: %s\n", hf);
        else {
            fputs(str,hist);
            fclose(hist);
        }

        exec_cmd(line);
    }
    return EXIT_STATUS;
}

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }
