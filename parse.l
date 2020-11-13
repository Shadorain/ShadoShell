%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();

YYSTYPE *y = &yylval;
int cp_line = 0;
Args args;
%}
%option yylineno
%%

^.+                    { if ( !cp_line ) { args.main = malloc(sizeof(yytext)); args.main = strdup(yytext); cp_line = 1; args = init_args(args); } REJECT;}
"exit"				   return EXIT_CMD;
[0-9]+                 yylval.n = atoi(yytext);printf("YYVAL.n: %d\n",yylval.n);
[-+=;]           	   return yytext[0];
"|"                    return PIPE;
[-a-zA-Z0-9]+          y->word.w = malloc(sizeof(yytext));y->word.w = strdup(yytext);return WORD;
"#"[ -~]+              ; // POSIX style comments
"//"[ -~]+             ; // C style comments
"--"[ -~]+             ; // Haskell style comments
[ \t]+                 ; // ignore all whitespace
\n                     cp_line = 0; return CR; // ignore newline
.                      printf("unexpected character\n"); // All else

%%

int yywrap (void) { return 1; }
/* .                      { ECHO; yyerror ("unexpected character"); } */
