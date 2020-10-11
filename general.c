//-----------------------------------------------------------------------------------
//   _________.__                .___         _________.__           .__  .__
//  /   _____/|  |__ _____     __| _/____    /   _____/|  |__   ____ |  | |  |
//  \_____  \ |  |  \\__  \   / __ |/  _ \   \_____  \ |  |  \_/ __ \|  | |  |
//  /        \|   Y  \/ __ \_/ /_/ (  <_> )  /        \|   Y  \  ___/|  |_|  |__
// /_______  /|___|  (____  /\____ |\____/  /_______  /|___|  /\___  >____/____/
//         \/      \/     \/      \/                \/      \/     \/
//----------------------------------------------------------------------------------------
// -- Includes -- {{{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
// }}}
// -- Defines -- {{{
#define EXIT_STATUS 0
#define MAXCHAR 1000 // Max chars
#define MAXLIST 100 // Max cmds
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
// -- Help Menu -- {{{
void usage () {
    printf("Run 'man shadosh' for full documentation.\n");
    printf("Usage: ");
    exit(0);
}
// }}}
// -- Prompt -- {{{
int prompt (char* in) {
    char cwd[512];
    char* buf;

    getcwd(cwd, sizeof(cwd));
    printf("\n  %s", cwd);

    buf = readline("\n❱ ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(in, buf);
        return 0;
    } else
        return 1;
}
// }}}
// -- Command Handling -- {{{
int builtin_handler(char** args) {
    int count = 4, checkArgs = 0;
    char* cmdArr[count];
    char* user;

    cmdArr[0] = "cd";
    cmdArr[1] = "exit";
    cmdArr[2] = "help";
    cmdArr[3] = "test";

    for (int i = 0; i < count; i++)
        if (strcmp(args[0], cmdArr[i]) == 0) {
            checkArgs = i + 1;
            break;
        }
    
    switch (checkArgs) {
        case 1:
            chdir(args[1]);
            return 1;
        case 2:
            exit(0);
        case 3:
            usage();
            /* help_menu(); */
            return 1;
        case 4:
            user = getenv("USER");
            printf("\n Test %s Test.", user);
            return 1;
        default:
            break;
    }

    return 0;
}
// }}}
// -- Exec -- {{{
void exec_semi(char** args, char** cmds) { // Exec sys cmd
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
// }}}
// -- Parsing -- {{{
void parse_args(char* in, char** args) {
    for (int i = 0; i < MAXLIST; i++) {
        args[i] = strsep(&in, " ");
        if(args[i] == NULL)
            break;
        if(strlen(args[i]) == 0)
            i--;
    }
}

int parse_pipes(char* in, char** piped) {
    for (int i = 0; i < 2; i++) {
        piped[i] = strsep(&in ,"|");
        if (piped[i] == NULL)
            break;
    }

    if (piped[1] == NULL)
        return 0;
    else
        return 1;
}

int parse_semi(char* in, char** cmds) {
    for (int i = 0; i < 2; i++) {
        cmds[i] = strsep(&in ,";");
        if (cmds[i] == NULL)
            break;
    }

    if (cmds[1] == NULL)
        return 0;
    else
        return 1;
}

int get_exec_flag(char* in, char** args, char** pipe, char** cmds) {
    char* piped[2];
    int pipeCheck = 0, multiCmd = 0;

    pipeCheck = parse_pipes(in, piped);
    multiCmd = parse_semi(in, cmds);

    if (pipeCheck) {
        parse_args(piped[0], args);
        parse_args(piped[1], pipe);
    } else if (multiCmd) {
        parse_semi(cmds[0], cmds);
        parse_semi(cmds[1], cmds);
    } else
        parse_args(in, args);

    if (builtin_handler(args))
        return 0;
    else
        return 1 + pipeCheck + multiCmd;
}
//}}}
// -- Main -- {{{
int main () { // int argc, char* argv[]) {
    char inStr[MAXCHAR], *args[MAXLIST];
    char* piped[MAXLIST], *cmds[MAXLIST];
    int flag = 0;

    init_sh();
    while (1) {
        if(prompt(inStr))
            continue;
        flag = get_exec_flag(inStr, args, piped, cmds); // 0:builtin, 1:simple, 2:pipe
        if (flag == 1)
            exec_args(args);
        if (flag == 2)
            exec_piped(args, piped);
        if (flag == 3)
            exec_semi(args, cmds);
    }
    return EXIT_STATUS;
}
// }}}
//----------------------------------------------------------------------------------------
