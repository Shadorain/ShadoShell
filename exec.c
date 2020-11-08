// -- Exec Library -- //
// --- Header: exec.h
#include <unistd.h>

#include "exec.h"

int exec_cmd(Node *nd) {
    /* int fd = -1; */
    /* if ((fd = nd->redir[0]) != -1) { */
    /*     dup2(fd, STDIN_FILENO); */
    /* } */
    /* if ((fd = nd->redir[1]) != -1) { */
    /*     dup2(fd, STDOUT_FILENO); */
    /* } */
    return 0;//execvp(nd->un[0].cmds, &nd->un->cmds);
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
