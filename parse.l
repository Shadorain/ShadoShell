%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();

/* YYSTYPE *y = &yylval; */
%}
%option yylineno
%%

"exit"				   return EXIT_CMD;
"#"[ -~]+              ; // POSIX style comments
"//"[ -~]+             ; // C style comments
"--"[ -~]+             ; // Haskell style comments
[!@#$%^&*()_+-={}[]|:;'`~<>,.?\/]*    { yylval.w = (char*)calloc(strlen(yytext)-1, sizeof(char*))
                                      ; yylval.w = strndup(yytext, sizeof(yytext))
                                      ; printf("Word: %s\n",yylval.w)
                                      /* ; y->w = strndup(yytext, sizeof(yylex)) */
                                      ; return WORD; }
[^ \t\r\n\v\f]+       { char* text = strdup(yytext)
                      ; printf("Text: %s\n",text)
                      ;  }

[ \t]+                 ; // ignore all whitespace
\n                     return CR; // ignore newline
.                      printf("unexpected character\n"); // All else

%%

int yywrap (void) { return 1; }
//strings -> ^"([^"\\]*(?:\\.[^"\\]*)*)"$
