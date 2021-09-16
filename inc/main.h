#ifndef MAIN_H
# define MAIN_H

# define _GNU_SOURCE

# include "test.h"
# include "color.h"

volatile sig_atomic_t testDone;

///  ///////////////////////////////  ///
///  ///          UTILS          ///  ///
///  ///////////////////////////////  ///

size_t strslen(char *const *strs);
void setSignal(void);

#endif // MAIN_H
