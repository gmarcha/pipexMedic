#include "main.h"

size_t strslen(char **strs) {

    int len;
    for (len = 0; strs[len] != NULL; len++);
    return (len);
}

void test(char *inputFileContent, char *commandList[], char *envp[]) {

    // FILE    *fsPipexLog;
    int     waitStatus;
    int     pipexStatus;
    int     bashStatus;

    int     fdOutput;
    int     fdError;

    size_t  nbCommand = strslen(commandList);

    FILE*   fsInputFile;

    if ((fsInputFile = fopen("input", "w")) == NULL) {
        ferror("fopen", errno);
    }
    fwrite(inputFileContent, strlen(inputFileContent), 1, fsInputFile);
    fclose(fsInputFile);

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {

        if (
            (fdOutput = open("./tmp/outputPipex.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ||
            (fdError = open("./tmp/errorPipex.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
        ) {
            ferror("open", errno);
        }

        if (dup2(fdOutput, STDOUT_FILENO) == -1 || dup2(fdError, STDERR_FILENO) == -1) {
            ferror("dup2", errno);
        }

        char*   pipexArgs[nbCommand + 4];

        pipexArgs[0] = "../pipex";
        pipexArgs[1] = "input";
        for (size_t i = 0; i < nbCommand; i++) {
            pipexArgs[i + 2] = commandList[i];
        }
        pipexArgs[nbCommand + 2] = "outputPipex";
        pipexArgs[nbCommand + 3] = NULL;

        int execStatus = execve("./pipex", pipexArgs, envp);
        if (execStatus == -1) {
            ferror("execve", errno);
        }
    }

    wait(&waitStatus);
    if (WIFEXITED(waitStatus)) {
        pipexStatus = WEXITSTATUS(waitStatus);
    } else if (WIFSIGNALED(waitStatus)) {
        pipexStatus = WTERMSIG(waitStatus);
    } else {
        ferror("wait", EAGAIN);
    }

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {

        if ((fdError = open("./tmp/errorBash.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            ferror("open", errno);
        }

        if (dup2(fdError, STDERR_FILENO) == -1) {
            ferror("dup2", errno);
        }

        char    bashCommand[256] = {0};

        strcat(bashCommand, "< input ");
        for (size_t i = 0; i < nbCommand; i++) {
            strcat(bashCommand, commandList[i]);
            if (i < nbCommand - 1) {
                strcat(bashCommand, " | ");
            } else {
                strcat(bashCommand, " ");
            }
        }
        strcat(bashCommand, " > outputBash");

        char*   bashArgs[4];

        bashArgs[0] = "bash";
        bashArgs[1] = "-c";
        bashArgs[2] = bashCommand;
        bashArgs[3] = NULL;

        int execStatus = execve("bash", bashArgs, envp);
        if (execStatus == -1) {
            ferror("execve", errno);
        }
    }

    wait(&waitStatus);
    if (WIFEXITED(waitStatus)) {
        bashStatus = WEXITSTATUS(waitStatus);
    } else if (WIFSIGNALED(waitStatus)) {
        bashStatus = WTERMSIG(waitStatus);
    } else {
        ferror("wait", EAGAIN);
    }
}
