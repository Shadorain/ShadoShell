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
#include "parse.h"
#include "parsing.h"
#include "exec.h"
#include "types.h"
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
// -- Fork -- {{{
void close_pipes(int n_pipes, int (*piped)[2]) {                
    for (int i = 0; i < n_pipes; ++i) {                                   
        close(piped[i][0]);                                                 
        close(piped[i][1]);                                                 
    }                                                                     
}                                                                       

pid_t fork_pipe(cmd_t* cmd_s, int pipe_n, int (*piped)[2]) {
    if(cmd_s->builtin == 0) {
        pid_t ch_pid = fork();
        if (ch_pid) {  /* We are the parent. */
            switch(ch_pid) {
                case -1:
                    fprintf(stderr, "Oh dear.\n");
                    return -1;
                default:
                    return ch_pid;
            }
        } else {  // We are the child. */
            exec_fork(cmd_s, pipe_n, piped);
            perror("Command not found.");
            return 0;
        }
    }
    return 0;
}                                                                         

pid_t fork_cmd(cmd_t* cmd_s) {
    if(cmd_s->builtin == 0) {
        pid_t ch_pid = fork();
        if (ch_pid) {  /* We are the parent. */
            switch(ch_pid) {
                case -1:
                    fprintf(stderr, "Oh dear.\n");
                    return -1;
                default:
                    return ch_pid;
            }
        } else {  // We are the child. */
            exec_cmd(cmd_s);
            perror("Command not found.");
            return 0;
        }
    }
    return 0;
}                                                                         
//}}}
// -- Main -- {{{
int main () { // int argc, char* argv[]) {
    int ntoken, vtoken; // name token, value token
    char in[MAXLIST];
    
    init_sh();
    ntoken = yylex();
    while (ntoken) {
        if(yylex() != COLON) {
            printf("Syntax error in line %d, Expected a ':' but found %s\n",yylineno,yytext);
            return 1;
        }
        vtoken = yylex();
        switch (ntoken) {
            /* case BLAH: */
        }
    }

    return EXIT_STATUS;
}
    /* while (prompt(PROMPT,in)) { */
        /* pipes_t* pipe_s; */
        /* ctrl_t* ctrl_s; */
        /* int pipes=0,multi=0; */
        /* char *cmds; */
        /* char *dup = strndup(in, MAXLIST), *dup2 = strndup(in, MAXLIST); */
        
        /* for(char* c = dup; *c; c++) // Counts amount of pipes */
        /*     if (*c == '|') ++pipes; */
        /* ++pipes; */
        /* for(char* d = dup2; *d; d++) // Counts amount of pipes */
        /*     if (*d == '&' || *d == '(' || *d == ')' || *d == ';') ++multi; */
        /* ++multi; */

        /* printf("PIPES: %d, MULTI: %d\n", pipes, multi); */

        /* pipe_s = parse_pipes(in); // Parses by '|' -> bool */
        /* if (multi > 1) */
        /*     ctrl_s = parse_multi(in); // Parses by '|' -> bool */

        /* print_pipeline(pipe_s); */
        
        /* if (pipes == 1 && multi == 1) */
        /*     for (int i = 0; i < pipe_s->cmd_n; ++i){ */
        /*         fork_cmd(pipe_s->cmds[i]); */
        /*         /1* fork_pipe(pipe_s->cmds[i], pipe_n, piped); *1/ */
        /*         wait(NULL); */
        /*     } */

        /* if (pipes > 1) { */
        /*     int pipe_n = pipe_s->cmd_n - 1; */
        /*     int (*piped)[2] = calloc(2*sizeof(int), pipe_n); */
        /*     for (int i = 1; i < pipe_s->cmd_n; ++i) { */
        /*         pipe(piped[i-1]); */
        /*         pipe_s->cmds[i]->redir[STDIN_FILENO] = piped[i-1][0]; */
        /*         pipe_s->cmds[i-1]->redir[STDOUT_FILENO] = piped[i-1][1]; */
        /*     } */

        /*     for (int i = 0; i < pipe_s->cmd_n; ++i) */
        /*         fork_pipe(pipe_s->cmds[i], pipe_n, piped); */

        /*     close_pipes(pipe_n, piped); */
        /*     /1* Wait for all the children to terminate. Rule 0: not checking status. *1/ */
        /*     for (int i = 0; i < pipe_s->cmd_n; ++i) */
        /*         wait(NULL); */

        /*     free(pipe_s); */
        /* } */

        /* if (multi > 1) { */
        /*     int cmd_n = ctrl_s->cmd_n - 1; */
        /*     int (*ctrl)[2] = calloc(2*sizeof(int), cmd_n); */
        /*     for (int i = 1; i < ctrl_s->cmd_n; ++i) { */
        /*         pipe(ctrl[i-1]); */
        /*         ctrl_s->ccmds[i]->redir[STDIN_FILENO] = ctrl[i-1][0]; */
        /*         ctrl_s->ccmds[i-1]->redir[STDOUT_FILENO] = ctrl[i-1][1]; */
        /*     } */

        /*     printf("Cmd_n ctrl_t: %d\n", ctrl_s->cmd_n); */
        /*     for (int i = 0; i < ctrl_s->cmd_n; ++i) */
        /*         /1* printf("CCMD: %s\n",ctrl_s->ccmds[i]->main_cmd); *1/ */
        /*         fork_pipe(ctrl_s->ccmds[i], cmd_n, ctrl); */

        /*     close_pipes(cmd_n, ctrl); */
            
        /*     for (int i = 0; i < ctrl_s->cmd_n; ++i) */
        /*         wait(NULL); */

        /*     free(ctrl_s); */
        /* } */
// }}}
//----------------------------------------------------------------------------------------
