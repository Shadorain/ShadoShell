%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();
%}
%%
"exit"				   { exit(0); }
[ \t\n]                ;
[-+=;]           	   { printf("TEST\n"); return yytext[0]; }

%%

int yywrap (void) {return 1;}
/* .                      { ECHO; yyerror ("unexpected character"); } */
/* [a-zA-Z]			   { yylval.id = yytext[0]; return identifier; } */
/* [0-9]+                 { yylval.num = atoi(yytext); return number; } */
