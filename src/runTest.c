#include "main.h"

void   runTest(const char *testSubset, const size_t testIndex, char *const commandList[], char *const envp[], const char* inputFileContent) {

    pid_t pid = fork();

    if (pid == (pid_t)-1) {
        ferror("fork", errno);
    }

    else if (pid == (pid_t)0) {

        test(testSubset, testIndex, commandList, envp, inputFileContent);
        kill(getppid(), SIGUSR1);
        exit(0);
    }

    const uint64_t startTime = time(NULL);
    while (testDone == 0) {
        const uint64_t currentTime = time(NULL);
        if (currentTime - startTime > TIMEOUT) {
            printf(FG_RED"%lu. TIMEOUT (NO TEST PERFORMED)"RESET, testIndex);
            kill(pid, SIGKILL);
            break;
        }
    }

    testDone = 0;
    unlink("./input");
    unlink("./outputPipex");
    unlink("./outputBash");
}
