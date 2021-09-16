#include "main.h"

size_t strslen(char *const *strs) {

    size_t len;
    for (len = 0; strs[len] != NULL; len++);
    return (len);
}

void syncSignal(int sig) {

    (void)sig;
    testDone = 1;
}

void setSignal(void) {

    struct sigaction act;
    sigset_t blockmask;

    sigfillset(&blockmask);
    act.sa_handler = syncSignal;
    act.sa_mask = blockmask;
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
}
