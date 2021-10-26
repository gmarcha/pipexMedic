#include "main.h"

void setRedirection(const char* output, const char* error) {

    const int fdOutput = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    const int fdError = open(error, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fdOutput == -1 || fdError == -1) {
        ferror("open", errno);
    }

    if (dup2(fdOutput, STDOUT_FILENO) == -1 || dup2(fdError, STDERR_FILENO) == -1) {
        ferror("dup2", errno);
    }
}

void executeCommand(const char *commandPath, char *const commandArgs[], char *const envp[], const char* output, const char* error) {

    const pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    } else if (pid == (pid_t)0) {
        setRedirection(output, error);
        const int execStatus = execve(commandPath, commandArgs, envp);
        if (execStatus == -1) {
            ferror("execve", errno);
        }
    }
}

int getExitStatus() {

    int     waitStatus;
    int     exitStatus;

    wait(&waitStatus);
    if (WIFEXITED(waitStatus)) {
        exitStatus = WEXITSTATUS(waitStatus);
    } else if (WIFSIGNALED(waitStatus)) {
        exitStatus = WTERMSIG(waitStatus);
    } else {
        ferror("wait", EAGAIN);
    }
    return (exitStatus);
}

bool getFilePermission() {

    struct stat outputPipexStat;
    struct stat outputBashStat;

    if (stat("./outputPipex", &outputPipexStat) == -1) {
        ferror("stat", errno);
    }
    if (stat("./outputBash", &outputBashStat) == -1) {
        ferror("stat", errno);
    }

    return (outputPipexStat.st_mode == outputBashStat.st_mode);
}

void test(const char *testSubset, const size_t testIndex, char *const commandList[], char *const envp[], const char* inputFileContent) {

    const size_t nbCommand = strslen(commandList);

    if (inputFileContent != NULL) {
        FILE* fsInputFile = fopen("./input", "w");

        if (fsInputFile == NULL) {
            ferror("fopen", errno);
        }
        fwrite(inputFileContent, strlen(inputFileContent), 1, fsInputFile);
        fclose(fsInputFile);
    }

    char *pipexArgs[nbCommand + 4];

    pipexArgs[0] = "./pipex";
    pipexArgs[1] = "input";
    for (size_t i = 0; i < nbCommand; i++) {
        pipexArgs[i + 2] = commandList[i];
    }
    pipexArgs[nbCommand + 2] = "outputPipex";
    pipexArgs[nbCommand + 3] = NULL;

    char bashCommand[256] = {0};

    strcat(bashCommand, "< input ");
    for (size_t i = 0; i < nbCommand; i++) {
        strcat(bashCommand, commandList[i]);
        if (i < nbCommand - 1) {
            strcat(bashCommand, " | ");
        }
    }
    strcat(bashCommand, " > outputBash");

    char*   bashArgs[] = { "bash", "-c", bashCommand, NULL };

    const time_t pipexStartTime = time(NULL);
    executeCommand("../pipex", pipexArgs, envp, "./tmp/outputPipex.tmp", "./tmp/errorPipex.tmp");
    const int pipexStatus = getExitStatus();
    const time_t pipexTime = time(NULL) - pipexStartTime;

    const time_t bashStartTime = time(NULL);
    executeCommand("/usr/bin/bash", bashArgs, envp, "./tmp/outputBash.tmp", "./tmp/errorBash.tmp");
    const int bashStatus = getExitStatus();
    const time_t bashTime = time(NULL) - bashStartTime;

    system("sed -i 's/bash/pipex/g' tmp/errorBash.tmp");

    bashCommand[strlen(bashCommand) - 3 - 1] = '\0';

    FILE* fsOutFilePipex = fopen("./outputPipex", "r");
    FILE* fsOutFileBash = fopen("./outputBash", "r");
    FILE* fsStdOutPipex = fopen("./tmp/outputPipex.tmp", "r");
    FILE* fsStdOutBash = fopen("./tmp/outputBash.tmp", "r");
    FILE* fsStdErrPipex = fopen("./tmp/errorPipex.tmp", "r");
    FILE* fsStdErrBash = fopen("./tmp/errorBash.tmp", "r");

    const bool outFilePipexExists = (fsOutFilePipex == NULL) ? false : true;
    const bool outFileBashExists = (fsOutFileBash == NULL) ? false : true;

    if (fsStdOutPipex == NULL || fsStdOutBash == NULL || fsStdErrPipex == NULL || fsStdErrBash == NULL) {
        ferror("fopen", errno);
    }

    char outFilePipex[4096 + 1] = {0};
    char outFileBash[4096 + 1] = {0};
    char stdOutPipex[4096 + 1] = {0};
    char stdOutBash[4096 + 1] = {0};
    char stdErrPipex[4096 + 1] = {0};
    char stdErrBash[4096 + 1] = {0};

    if (outFilePipexExists == true) {
        fread(outFilePipex, 1, 4096, fsOutFilePipex);
        fclose(fsOutFilePipex);
    }
    if (outFileBashExists == true) {
        fread(outFileBash, 1, 4096, fsOutFileBash);
        fclose(fsOutFileBash);
    }
    fread(stdOutPipex, 1, 4096, fsStdOutPipex);
    fclose(fsStdOutPipex);
    fread(stdOutBash, 1, 4096, fsStdOutBash);
    fclose(fsStdOutBash);
    fread(stdErrPipex, 1, 4096, fsStdErrPipex);
    fclose(fsStdErrPipex);
    fread(stdErrBash, 1, 4096, fsStdErrBash);
    fclose(fsStdErrBash);

    const time_t timeDifference = pipexTime - bashTime;

    const bool areOutfileMatching = (!outFilePipexExists || !outFileBashExists) ? (outFilePipexExists != outFileBashExists) ? false : true : strcmp(outFilePipex, outFileBash) == 0;
    const bool areStdOutMatching = strcmp(stdOutPipex, stdOutBash) == 0;
    const bool areStdErrMatching = strcmp(stdErrPipex, stdErrBash) == 0;
    const bool areExitCodeMatching = pipexStatus == bashStatus;
    const bool isPipexUsingConcurrency = timeDifference < 2;
    const bool areFilePermissionsMatching = (!outFilePipexExists || !outFileBashExists) ? false : getFilePermission();

    if (!areOutfileMatching || !areStdOutMatching || !areStdErrMatching || !areExitCodeMatching || !isPipexUsingConcurrency || !areFilePermissionsMatching) {

        FILE* fsPipexLog = fopen("tester.log", "a");

        if (fsPipexLog == NULL) {
            ferror("fopen", errno);
        }

        char testOutput[8192 + 1];

        snprintf(
            testOutput, 8192,
            "============================================================================\n\n\n"
            "      %s TEST\n"
            "      NUMBER %lu\n"
            "      %s%s\n\n\n"
            " - Bash output file:%s"
            "%s"
            "%s"
            " - Pipex output file:%s"
            "%s"
            "%s"
            " - Bash standard stream:%s"
            "%s"
            "%s"
            " - Pipex standard stream:%s"
            "%s"
            "%s"
            "%s"
            " - Bash exit code value: %d\n\n"
            " - Pipex exit code value: %d\n\n"
            "%s"
            "%s"
            "\n",
            testSubset,
            testIndex,
            bashCommand, !envp[0] ? " (No environment)" : "",
            outFileBashExists ? outFileBash[0] ? "\n" : " Empty\n\n" : " None\n\n",
            outFileBashExists ? outFileBash[0] ? outFileBash : "" : "",
            outFileBashExists ? outFileBash[0] ? "\\EOF\n\n" : "" : "",
            outFilePipexExists ? outFilePipex[0] ? "\n" : " Empty\n\n" : " None\n\n",
            outFilePipexExists ? outFilePipex[0] ? outFilePipex : "" : "",
            outFilePipexExists ? outFilePipex[0] ? "\\EOF\n\n" : "" : "",
            stdErrBash[0] ? "\n" : " None\n\n",
            stdErrBash[0] ? stdErrBash : "",
            stdErrBash[0] ? "\\EOF\n\n" : "",
            stdErrPipex[0] ? "\n" : (stdOutPipex[0] && !areStdOutMatching) ? "\n" : " None\n\n",
            stdErrPipex[0] ? stdErrPipex : (stdOutPipex[0] && !areStdOutMatching) ? stdOutPipex : "",
            stdErrPipex[0] ? "\\EOF\n\n" : (stdOutPipex[0] && !areStdOutMatching) ? "\\EOF\n\n" : "",
            stdErrPipex[0] ? "" : (stdOutPipex[0] && !areStdOutMatching) ? "(Your pipex' errors are written on standard output!)\n\n" : "",
            bashStatus,
            pipexStatus,
            isPipexUsingConcurrency ? "" : "(Your pipex is not launching commands in parallel!)\n\n",
            (!outFilePipexExists || !outFileBashExists) ? "" : areFilePermissionsMatching ? "File permissions of output files match\n\n" : "File permissions of output files mismatch\n\n"
        );
        fwrite(testOutput, strlen(testOutput), 1, fsPipexLog);
        fclose(fsPipexLog);
        printf(FG_RED"%lu. KO%s%s%s%s%s%s  "RESET, testIndex,
            !areOutfileMatching ? " OUTPUT" : "",
            !areStdOutMatching ? " STDOUT" : "",
            !areStdErrMatching ? " STDERR" : "",
            !areExitCodeMatching ? " EXIT_CODE" : "",
            !isPipexUsingConcurrency ? " NO_CONCURRENCY" : "",
            !areFilePermissionsMatching ? " FILE_PERMISSIONS" : ""
        );
        fflush(NULL);
    }

    else {
        printf(FG_GREEN"%lu. OK  "RESET, testIndex);
        fflush(NULL);
    }
}
