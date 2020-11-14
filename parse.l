%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();

/* YYSTYPE *y = &yylval; */
%}
%option yylineno
%%

"exit"				  return EXIT_CMD;
"#"[ -~]+             ;
"//"[ -~]+            ;
"--"[ -~]+            ; 
[!@#$%^&*()_+-={}[]|:;`~<>,.?\/]*    { yylval.c = yytext[0]; printf("WORD\n"); return yytext[0]; }
[^ \t\r\n\v\f]+       { yylval.w = strdup(yytext)
                      ; printf("Text: %s\n", yylval.w)
                      ; return WORD; }
[ \t\n]*              ;
.                     printf("unexpected character\n");

%%

int yywrap (void) { return 1; }
//strings -> ^"([^"\\]*(?:\\.[^"\\]*)*)"$
