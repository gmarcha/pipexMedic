#ifndef MAIN_H
# define MAIN_H

# ifdef __APPLE__
#  define _GNU_SOURCE
# endif

# include "test.h"
# include "color.h"

extern volatile sig_atomic_t testDone;

///  ///////////////////////////////  ///
///  ///          UTILS          ///  ///
///  ///////////////////////////////  ///

size_t strslen(char *const *strs);
void setSignal(void);

#endif // MAIN_H
