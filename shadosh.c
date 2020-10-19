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
#include "builtins.h"
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
    sleep(1);
    // clear();
}
// }}}
// -- Prompt -- {{{
void show_dir() {
    char cwd[512];

    getcwd(cwd, sizeof(cwd));
    printf("\n  %s\n", cwd);
}

ssize_t prompt(const char* prompt, char** in, size_t *in_len) {
    char* buf;

    if (PROMPT_DIR==1) show_dir();

    fputs(prompt, stderr);
    return getline(in, in_len, stdin);
}

// }}}
// -- Exec Flag -- {{{
int get_exec_flag(char* in, char** args, char** pipe, char** cmds, char** parsedCmds) {
    char* piped[2];
    int pipeCheck = 0;
    
    /* multiCmd = parse_semi(in); // Parses by ';' -> num of multi_cmds */
    pipes_t* pipes = parse_pipes(in); // Parses by '|' -> bool

    if (builtin_handler(args))
        return 0;
    else
        return 1 + pipeCheck;
}

void close_ALL_the_pipes(int n_pipes, int (*piped)[2]) {                
    for (int i = 0; i < n_pipes; ++i) {                                   
      close(piped[i][0]);                                                 
      close(piped[i][1]);                                                 
    }                                                                     
}                                                                       

int exec_with_redir(cmd_t* cmd, int pipe_n, int (*piped)[2]) {
    int fd = -1;
    if ((fd = cmd->redir[0]) != -1) {
      dup2(fd, STDIN_FILENO);
    }
    if ((fd = cmd->redir[1]) != -1) {
      dup2(fd, STDOUT_FILENO);
    }
    close_ALL_the_pipes(pipe_n, piped);
    return execvp(cmd->main_cmd, cmd->args);
}

pid_t run_with_redir(cmd_t* cmd_s, int pipe_n, int (*piped)[2]) {
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
      exec_with_redir(cmd_s, pipe_n, piped);                             
      perror("OH DEAR");                                                    
      return 0;                                                             
    }                                                                       
}                                                                         

//}}}
// -- Main -- {{{
int main () { // int argc, char* argv[]) {
    char* piped[MAXLIST], *cmds[MAXLIST], *parsedCmds[MAXLIST];
    char* in = NULL, *args[MAXLIST];
    size_t in_len = 0;
    int flag = 0;

    init_sh();
    while (prompt(PROMPT, &in, &in_len) > 0) {
        add_history(in);
        pipes_t* pipe_s = parse_pipes(in); // Parses by '|' -> bool
        /* if(prompt(in)) */
        /*     continue; */

        int pipe_n = pipe_s->cmd_n - 1;
        print_pipeline(pipe_s);

        int (*piped)[2] = calloc(sizeof(int[2]), pipe_n);

        for (int i = 1; i < pipe_s->cmd_n; ++i) {                               
            pipe(piped[i-1]);                                                        
            pipe_s->cmds[i]->redir[STDIN_FILENO] = piped[i-1][0];               
            pipe_s->cmds[i-1]->redir[STDOUT_FILENO] = piped[i-1][1];            
        }                                                                          
                                                                                       
        for (int i = 0; i < pipe_s->cmd_n; ++i) {                               
            run_with_redir(pipe_s->cmds[i], pipe_n, piped);                       
        }                                                                          
                                                                                       
        close_ALL_the_pipes(pipe_n, piped);                                       
                                                                                       
        /* Wait for all the children to terminate. Rule 0: not checking status. */ 
        for (int i = 0; i < pipe_s->cmd_n; ++i) {                               
            wait(NULL);                                                              
        }

        /* flag = get_exec_flag(inStr, args, piped, cmds, parsedCmds); // 0:builtin, 1:simple, 2:pipe */
        /* if (flag == 1) */
        /*     exec_args(args); */
        /* if (flag == 2) */
        /*     exec_piped(args, piped); */
        // Checks if there is more than 1 command
        //  - If check: will parse separate cmds
        /* if (multiCmd > 1) */
        /*     for (int i = 1; i < multiCmd; i++) { */
        /*         /1* printf("MULTICMD: %d, i:%d\n",multiCmd,i); *1/ */
        /*         /1* printf("CMD #%d: %s\n", i, cmds[i]); *1/ */
        /*         /1* for (int j = 0; j < multiCmd; j++) *1/ */
        /*         /1*     printf("PARSED CMDS #%d: %s\n",j,parsedCmds[j]); *1/ */
        /*         parse_args(cmds[i], parsedCmds); */
        /*         if (builtin_handler(parsedCmds)) */
        /*             exec_args(parsedCmds); */
        /*         else */
        /*             exec_args(parsedCmds); */
        /*         if(cmds[i] == NULL) */
        /*             break; */
        /*     } */
    }
    return EXIT_STATUS;
}
// }}}
//----------------------------------------------------------------------------------------
