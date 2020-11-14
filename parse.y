%{
#include "shadosh.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void yyerror(char *s);
int yylex();

/* void del_node(Wordlist *wl) { */
/*     Wordlist *temp = wl; */
/*     temp->word=temp->next->word; */
/*     temp->next=temp->next->next; */
/* } */

Wordlist *append(Wordlist** head_ref, char *w) {
    Wordlist *new_node = (Wordlist*)malloc(sizeof(Wordlist)); /* 1. allocate node */
    Wordlist *last = *head_ref;  /* used in step 5 */

    new_node->word  = w; /* 2. put in the data  */
    new_node->next = NULL; /* 3. This new node is going to be the last node, so make next of it as NULL*/

    if (*head_ref == NULL) { /* 4. If the Linked List is empty, then make the new node as head */
       *head_ref = new_node;
       return new_node;
    }  

    while (last->next != NULL) /* 5. Else traverse till the last node */
        last = last->next;

    last->next = new_node; /* 6. Change the next of last node */
    return last->next;    
}

void print(Wordlist **head_ref) {
    Wordlist *temp = *head_ref;
    printf("List is: ");
    while(temp != NULL) {
        printf(" %s",temp->word);
        temp = temp->next;
    }
    printf("\n");
}

void clean_list(Wordlist **head_ref) {
    Wordlist *current = *head_ref;
    Wordlist *next; 
    
    while (current != NULL)  { 
        next = current->next; 
        free(current); 
        current = next; 
    } 
     
    *head_ref = NULL; /* deref head_ref to affect the real head back in the caller. */
}

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
    init_sh();
    while(1) {
        yyparse();
        clean_list(&head);
    }
    return EXIT_STATUS;
}

void yyerror (char *s) { fprintf (stderr, "%s\n", s); }

