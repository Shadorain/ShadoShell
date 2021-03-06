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
// -- Parsing -- {{{
int get_exec_flag(char* in, char** args, char** pipe, char** cmds) {
    char* piped[2];
    int pipeCheck = 0, multiCmd = 0;

    pipeCheck = parse_pipes(in, piped);
    multiCmd = parse_semi(in, cmds);

    if (multiCmd > 0)
        for (int i = 0; i < multiCmd; i++) {
            parse_args(cmds[i], cmds);
            printf("CMD #%d: %s\n", i, cmds[i]);
        }
    
    if (pipeCheck) {
        parse_args(piped[0], args);
        parse_args(piped[1], pipe);
    } else
        parse_args(in, args);

    if (builtin_handler(args))
        return 0;
    else
        return 1 + pipeCheck;
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
        /* if (flag == 3) */
        /*     exec_semi(args, cmds); */
    }
    return EXIT_STATUS;
}
// }}}
//----------------------------------------------------------------------------------------
