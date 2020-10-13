// -- Exec Library -- //
// --- Header: exec.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "exec.h"

#define MAXLIST 100 // Max cmds

void exec_semi(char** args, char** cmds) { // Exec sys cmd
    pid_t p1, p2 = fork(); // Fork child

    if (p1 == -1) {
        printf("Failed forking child...\n");
        return;
    } else if (p1 == 0) {
        if (execvp(args[0], args) < 0)
            printf("Couldn't execute command...\n");
        exit(0);
    } else {
        wait(NULL); // waits for child to terminate
        return;
    }
}

void exec_args(char** args) { // Exec sys cmd
    pid_t pid = fork(); // Fork child

    if (pid == -1) {
        printf("Failed forking child...\n");
        return;
    } else if (pid == 0) {
        if (execvp(args[0], args) < 0)
            printf("Couldn't execute command...\n");
        exit(0);
    } else {
        wait(NULL); // waits for child to terminate
        return;
    }
}

void exec_piped(char** args, char** piped) { // piped sys cmd is exec
    int pipefd[2]; // 0:r ; 1:w
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("Couldn't init pipe.\n");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("Couldn't fork.\n");
        return;
    }

    if (p1 == 0) { // Child 1 exec ; needs to read at read end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (execvp(args[0], args) < 0) {
            printf("Couldn't exec cmd 1\n");
            exit(0);
        }
    } else {
        p2 = fork(); // Exec parent
        if (p2 < 0) {
            printf("Couldn't fork.\n");
            return;
        }
        if (p2 == 0) { // Child 2 exec; read end
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(piped[0], piped) < 0) {
                printf("Couldn't exec child 2.\n");
                exit(0);
            }
        } else {
            wait(NULL); // waiting for children to die, parent exec
            wait(NULL);
        }
    }
}
