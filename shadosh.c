//-----------------------------------------------------------------------------------
//   _________.__                .___         _________.__           .__  .__
//  /   _____/|  |__ _____     __| _/____    /   _____/|  |__   ____ |  | |  |
//  \_____  \ |  |  \\__  \   / __ |/  _ \   \_____  \ |  |  \_/ __ \|  | |  |
//  /        \|   Y  \/ __ \_/ /_/ (  <_> )  /        \|   Y  \  ___/|  |_|  |__
// /_______  /|___|  (____  /\____ |\____/  /_______  /|___|  /\___  >____/____/
//         \/      \/     \/      \/                \/      \/     \/
//----------------------------------------------------------------------------------------
// -- Includes -- {{{
// --- Libs --- //
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// --- Files --- //
/* #include "parse.h" */
/* #include "parsing.h" */
/* #include "exec.h" */
/* #include "types.h" */
// }}}
// -- Defines -- {{{
#define EXIT_STATUS 0
#define MAXCHAR 1024 // Max chars
#define MAXLIST 1024 // Max cmds
#define PROMPT "❱ " // Prompt string
#define PROMPT_DIR 1 // Show directory on prompt or not
#define clear() printf("\033[H\033[J") // Clear shell via esc codes

extern int yylex();
extern int yylineno;
extern char* yytext;
// }}}
// -- Init Shell -- {{{
void init_sh () {
    clear();
    char* curr_user = getenv("USER");
    printf("       _________.__                .___         _________.__           .__  .__           \n");
    printf("      /   _____/|  |__ _____     __| _/____    /   _____/|  |__   ____ |  | |  |          \n");
    printf("      \\_____  \\ |  |  \\\\__  \\   / __ |/  _ \\   \\_____  \\ |  |  \\_/ __ \\|  | |  |\n");
    printf("      /        \\|   Y  \\/ __ \\_/ /_/ (  <_> )  /        \\|   Y  \\  ___/|  |_|  |__   \n");
    printf("     /_______  /|___|  (____  /\\____ |\\____/  /_______  /|___|  /\\___  >____/____/     \n");
    printf("             \\/      \\/     \\/      \\/                \\/      \\/     \\/            \n");
    printf(" ---------------------------------------------------------------------------------------- \n");
    printf("USER -- %s\n", curr_user);
    /* sleep(1); */
    // clear();
}
// }}}
// -- Prompt -- {{{
void show_dir() {
    char cwd[512];

    getcwd(cwd, sizeof(cwd));
    printf("\n  %s\n", cwd);
}

ssize_t prompt(const char* prompt, char* in){//, size_t *in_len) {
    char* buf;
    if (PROMPT_DIR==1) show_dir();

    /* fputs(prompt, stderr); */
    buf = readline(prompt);
    if(strlen(buf) != 0) {
        add_history(buf);
        strcpy(in,buf);
    }

    return 1; //getline(in, in_len, stdin);
}

// }}}
// -- Main -- {{{
int main () { // int argc, char* argv[]) {
    int ntoken, vtoken; // name token, value token
    char in[MAXLIST];
    
    init_sh();
    ntoken = yylex();
    while (ntoken) {
        /* if(yylex() != COLON) { */
        /*     printf("Syntax error in line %d, Expected a ':' but found %s\n",yylineno,yytext); */
        /*     return 1; */
        /* } */
        vtoken = yylex();
        switch (ntoken) {
            /* case BLAH: */
        }
    }

    return EXIT_STATUS;
}
// }}}
//----------------------------------------------------------------------------------------
