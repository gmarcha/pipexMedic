#include "main.h"

int main(int ac, char *av[], char *envp[]) {

    test("Hello World!", (char *[]){ "grep Hello", "wc -l", NULL });
    return (0);
}
