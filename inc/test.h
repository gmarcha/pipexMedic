#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <time.h>
# include <pthread.h>
# include <signal.h>
# include <stdint.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>

# define PROGRAM_NAME                   "pipexMedic"
# define TIMEOUT                        10
# define DEFAULT_PATH                   "PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin"
# define DEFAULT_ENV                    (char *const []){ DEFAULT_PATH, NULL }
# define NULL_ENV                       (char *const []){ NULL }

# define ARGS(...)                      (char *const []){ __VA_ARGS__, NULL }
# define ferror(funName, errNum)        fprintf(stderr, "%s: %s failed: %s\n", PROGRAM_NAME, funName, strerror(errNum)); exit(1);

typedef struct s_test {

    char *const*    commandList;
    char *const*    envp;
    const char*     inputFileContent;
}                   t_test;

void    test(const char *testSubset, const size_t testIndex, char *const commandList[], char *const envp[], const char* inputFileContent);
void    runTest(const char *testSubset, const size_t testIndex, char *const commandList[], char *const envp[], const char* inputFileContent);

#endif // TEST_H
