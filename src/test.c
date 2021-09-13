#include "main.h"



void test(char *inputFileContent, char *commandList[], char *envp[]) {

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {



        int execStatus = execve("./pipex", pipexArgs, envp);
        if (execStatus == -1) {
            ferror("execve", errno);
        }
    }

    else {

    }
}
