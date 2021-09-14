#include "main.h"

int main(void) {

    test("Hello World!\n", ARGS("grep Hello", "wc -l"), DEFAULT_ENV);
    return (0);
}
