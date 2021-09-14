#include "main.h"

size_t strslen(char **strs) {

    int len;
    for (len = 0; strs[len] != NULL; len++);
    return (len);
}

void test(char *inputFileContent, char *commandList[], char *envp[]) {

    int fd_output;
    int fd_error;

    FILE*   fsInputFile;

    if ((fsInputFile = fopen("infile", "w")) == NULL) {
        ferror("fopen", errno);
    }
    fwrite(inputFileContent, strlen(inputFileContent), 1, fsInputFile);
    fclose(fsInputFile);

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {

        size_t  nbCommand = strslen(commandList);
        char*   pipexArgs[nbCommand + 4];

        pipexArgs[0] = "../pipex";
        pipexArgs[1] = "infile";
        for (size_t i = 0; i < nbCommand; i++) {
            pipexArgs[i + 2] = commandList[i];
        }
        pipexArgs[nbCommand + 2] = "outfile";
        pipexArgs[nbCommand + 3] = NULL;

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

        // FILE    *fs_log;
        int     waitStatus;
        int     pipexStatus;

        wait(&waitStatus);
        if (WIFEXITED(waitStatus)) {
            pipexStatus = WEXITSTATUS(waitStatus);
        } else if (WIFSIGNALED(waitStatus)) {
            pipexStatus = WTERMSIG(waitStatus);
        } else {
            ferror("wait", EAGAIN);
        }


    }
}
