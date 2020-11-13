// -- Exec Library -- //
// --- Header: exec.h
#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

#include "shadosh.h"

extern int exec_cmd(Args args) {
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return -1; 
    } else if (pid == 0) { 
        if (execvp(args.args[0], args.args) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return 0; 
    } 
    /* int fd = -1; */
    /* if ((fd = nd->redir[0]) != -1) { */
    /*     dup2(fd, STDIN_FILENO); */
    /* } */
    /* if ((fd = nd->redir[1]) != -1) { */
    /*     dup2(fd, STDOUT_FILENO); */
    /* } */
    return 1; //execvp(args.args[0], args.args);
}

/* int exec_fork(cmd_t* cmd, int pipe_n, int (*piped)[2]) { */
/*     int fd = -1; */
/*     if ((fd = cmd->redir[0]) != -1) { */
/*         dup2(fd, STDIN_FILENO); */
/*     } */
/*     if ((fd = cmd->redir[1]) != -1) { */
/*         dup2(fd, STDOUT_FILENO); */
/*     } */
/*     close_pipes(pipe_n, piped); */

/*     return execvp(cmd->main_cmd, cmd->args); */
/* } */
