#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>

# define PROGRAM_NAME                   "pipexMedic"
# define ferror(funName, errNum)        fprintf(stderr, "%s: %s failed: %s\n", PROGRAM_NAME, funName, strerror(errNum)); exit(1);

#endif // TEST_H
