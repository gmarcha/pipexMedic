#include "main.h"

volatile sig_atomic_t testDone = 0;

int main(int ac, char *av[]) {

    if (ac < 2) {
        ferror("input", EAGAIN);
    }
    const char *inputSubsetTest = av[1];

    size_t inputSpecificTest = 0;
    if (ac > 2) {
        inputSpecificTest = atoi(av[2]);
    }

    unlink("./tester.log");
    setSignal();

    const t_test basicTests[] = {
        { ARGS("grep Hello", "wc -l"), DEFAULT_ENV, "Hello World!\n" },
        { ARGS("grep Hello", "wc -l"), DEFAULT_ENV, "Hello World!\nHello World!\nHello World!\nHello World!\nHello World!\n" },
        { ARGS("grep Hello", "ls -la src/"), DEFAULT_ENV, "Hello World!\n" },
        { ARGS("ls -la src/", "wc -l"), DEFAULT_ENV, "Hello World!\n" },
        { ARGS("grep Hello", "awk '{count++} END {print count}'"), DEFAULT_ENV, "Hello World!\nHello World!\n" },
        { ARGS("grep Hello", "awk \"{count++} END {print count}\""), DEFAULT_ENV, "Hello World!\nHello World!\n" },
        { ARGS("grep Hello", "awk '\"{count++} END {print count}\"'"), DEFAULT_ENV, "Hello World!\nHello World!\n" },
        { ARGS("grep Hello", "awk \"'{count++} END {print count}'\""), DEFAULT_ENV, "Hello World!\nHello World!\n" }
    };

    const t_test errorTests[] = {
        { ARGS("grep Hello", "wc -l"), DEFAULT_ENV, NULL },
        { ARGS("grep Hello", "wc -l"), NULL_ENV, "Hello World!\n" },
        { ARGS("fizzBuzz", "ls -la src/"), DEFAULT_ENV, "Hello World!\n" },
        { ARGS("ls -la src/", "buzzFizz"), DEFAULT_ENV, "Hello World!\n" },
        { ARGS("fizzBuzz", "wc -l"), NULL_ENV, "Hello World!\n" },
        { ARGS("grep Hello", "buzzFizz"), NULL_ENV, "Hello World!\n" }
    };

    const t_test concurrencyTests[] = {
        { ARGS("sleep 3", "sleep 3"), DEFAULT_ENV, "Hello World!\n" }
    };

    if (
        strcmp(inputSubsetTest, "basic") == 0 ||
        strcmp(inputSubsetTest, "mandatory") == 0 ||
        strcmp(inputSubsetTest, "m") == 0 ||
        strcmp(inputSubsetTest, "all") == 0 ||
        strcmp(inputSubsetTest, "a") == 0
    ) {

        printf(FG_YELLOW"basic tests\n"RESET);
        for (size_t i = 0; i < 8; i++) {
            if (inputSpecificTest == 0 || inputSpecificTest == i + 1) {
                runTest("BASIC", i + 1, basicTests[i].commandList, basicTests[i].envp, basicTests[i].inputFileContent);
            }
        }
    }

    if (
        strcmp(inputSubsetTest, "error") == 0 ||
        strcmp(inputSubsetTest, "mandatory") == 0 ||
        strcmp(inputSubsetTest, "m") == 0 ||
        strcmp(inputSubsetTest, "all") == 0 ||
        strcmp(inputSubsetTest, "a") == 0
    ) {

        printf(FG_YELLOW"\nerror handling\n"RESET);
        for (size_t i = 0; i < 6; i++) {
            if (inputSpecificTest == 0 || inputSpecificTest == i + 1) {
                runTest("ERROR", i + 1, errorTests[i].commandList, errorTests[i].envp, errorTests[i].inputFileContent);
            }
        }
    }

    if (
        strcmp(inputSubsetTest, "concurrency") == 0 ||
        strcmp(inputSubsetTest, "mandatory") == 0 ||
        strcmp(inputSubsetTest, "m") == 0 ||
        strcmp(inputSubsetTest, "all") == 0 ||
        strcmp(inputSubsetTest, "a") == 0
    ) {

        printf(FG_YELLOW"\nconcurrency launching for pipeline\n"RESET);
        for (size_t i = 0; i < 1; i++) {
            if (inputSpecificTest == 0 || inputSpecificTest == i + 1) {
                runTest("CONCURRENCY", i + 1, concurrencyTests[i].commandList, concurrencyTests[i].envp, concurrencyTests[i].inputFileContent);
            }
        }
    }

    printf("\n");
    return (0);
}
