#include "main.h"



void test(char *inputFileContent, char *commandList[], char *envp[]) {

    int     fd_output;
    int     fd_error;
    FILE    *fs_return;

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {

        char **pipexArgs = createPipexArgs(commandList);

        if (
            (fd_output = open("./tmp/output.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ||
            (fd_error = open("./tmp/error.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
        ) {
            ferror("open", errno);
        }

        if (dup2(fd_output, STDOUT_FILENO) == -1 || dup2(fd_error, STDERR_FILENO) == -1) {
            ferror("dup2", errno);
        }

        int execStatus = execve("./pipex", pipexArgs, envp);
        if (execStatus == -1) {
            ferror("execve", errno);
        }
    }

    else {

        if ((fs_return = fopen("./tmp/return.tmp", "w")) == NULL) {

        }
    }
}
