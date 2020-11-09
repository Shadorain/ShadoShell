%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();
YYSTYPE *y = &yylval;
%}
%%

"exit"				   return EXIT_CMD;
[0-9]+                 yylval.n = atoi(yytext);printf("YYVAL.n: %d\n",yylval.n);
[-+=;]           	   return yytext[0];
"|"                    return PIPE;
[-a-zA-Z0-9]+          y->word.w = malloc(sizeof(yytext)); y->word.w = strdup(yytext);printf("YYTEXT: %s\n",yytext);return WORD;
"#"[ -~]+              ; // POSIX style comments
"//"[ -~]+             ; // C style comments
"--"[ -~]+             ; // Haskell style comments
[ \t]+                 ; // ignore all whitespace
\n                     return CR; // ignore newline
.                      printf("unexpected character\n"); // All else

%%

int yywrap (void) {return 1;}
/* .                      { ECHO; yyerror ("unexpected character"); } */
