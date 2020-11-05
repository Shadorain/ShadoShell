%{
#include "y.tab.h"
void yyerror(char *s);
int yylex();
%}

%%

"exit"           { return EXIT_CMD; }
[ \t\n]          ;

%%

int yywrap(void) { return 1; }
