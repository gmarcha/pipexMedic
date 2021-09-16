# Pipex Medic
A complete C program to test Pipex, a 42 project to learn fork, redirection, and pipe in C programming. \
Features: output files, standard streams, exit codes, parallelism, and file permissions comparison.

# How to use it ?
- Clone the repository in your Pipex directory, go in the tester directory, and run `bash test.sh` with a test subset. \
  For example, `bash test.sh basic`. You can have a subset list if you launch the script without argument.
- Warning: If you want to pass error handling tests, you should use a macro in your pipex code to define the program name, \
  and set it like your pipex was bash. \
  You should define it like this for your own tests or to submit the project (`#define PROGRAM_NAME pipex`), \
  but you have to set it to BASH to pass the tester (`#define PROGRAM_NAME bash`).

![output](/assets/output2.png)
