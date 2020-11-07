%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();
%}
%%

"exit"				   return EXIT_CMD;
[0-9]+                 yylval.n = atoi(yytext);printf("YYTEXT [0]: %c\n",yytext[0]);
[-+=;]           	   printf("TEST\n"); return yytext[0];
[-a-zA-Z0-9]+          yylval.c = strdup(yytext);printf("YYTEXT: %s\n",yytext);
[ \t]+                 ; // ignore all whitespace
\n                     return CR; // ignore newline
.                      printf("unexpected character\n"); // All else

%%

int yywrap (void) {return 1;}
/* .                      { ECHO; yyerror ("unexpected character"); } */
