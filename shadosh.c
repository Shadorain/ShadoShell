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

pid_t fork_cmd(cmd_t* cmd_s, int pipe_n, int (*piped)[2]) {
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
            exec_cmd(cmd_s, pipe_n, piped);
            perror("Command not found.");
            return 0;
        }
    }
    return 0;
}                                                                         
//}}}
// -- Main -- {{{
int main () { // int argc, char* argv[]) {
    char* piped[MAXLIST], *cmds[MAXLIST], *parsedCmds[MAXLIST];
    char in[MAXLIST], *args[MAXLIST];

    init_sh();
    while (prompt(PROMPT,in)) {
        pipes_t* pipe_s = parse_pipes(in); // Parses by '|' -> bool
        int pipe_n = pipe_s->cmd_n - 1;
        int (*piped)[2] = calloc(sizeof(int[2]), pipe_n);

        print_pipeline(pipe_s);

        for (int i = 1; i < pipe_s->cmd_n; ++i) {
            pipe(piped[i-1]);
            pipe_s->cmds[i]->redir[STDIN_FILENO] = piped[i-1][0];
            pipe_s->cmds[i-1]->redir[STDOUT_FILENO] = piped[i-1][1];
        }

        for (int i = 0; i < pipe_s->cmd_n; ++i) {
            fork_cmd(pipe_s->cmds[i], pipe_n, piped);
        }

        close_pipes(pipe_n, piped);

        /* Wait for all the children to terminate. Rule 0: not checking status. */
        for (int i = 0; i < pipe_s->cmd_n; ++i) {
            wait(NULL);
        }
        
        free(pipe_s);
    }

    return EXIT_STATUS;
}
// }}}
//----------------------------------------------------------------------------------------
