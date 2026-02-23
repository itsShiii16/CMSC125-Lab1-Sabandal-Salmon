#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "../include/interpreter.h"      // Make sure this is included

int handle_builtin(Command cmd) {
    // for exit
    if (strcmp(cmd.command, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // for cd
    if (strcmp(cmd.command, "cd") == 0) {
        if (cmd.args[0] == NULL) {                                          // If no directory provided, go to HOME
            char *home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
            chdir(home);
        }
        else {
            if (chdir(cmd.args[0]) != 0) {
                perror("cd failed");
            }
        }
        return 1;                                                           // handled
    }

    // for pwd
    if (strcmp(cmd.command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd failed");
        }
        return 1;                                                           // handled
    }
    
    return 0;                                                               // not a builtin
}

void execute_external(Command cmd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    // CHILD PROCESS
    if (pid == 0) {

        // 1) INPUT REDIRECTION: <
        if (cmd.input_file != NULL) {
            int in_fd = open(cmd.input_file, O_RDONLY);
            if (in_fd < 0) {
                perror("open input failed");
                exit(1);
            }
            if (dup2(in_fd, STDIN_FILENO) < 0) {
                perror("dup2 input failed");
                close(in_fd);
                exit(1);
            }
            close(in_fd);
        }

        // 2) OUTPUT REDIRECTION: > or >>
        if (cmd.output_file != NULL) {
            int flags = O_WRONLY | O_CREAT;

            if (cmd.append) {
                flags |= O_APPEND;   // >>
            } else {
                flags |= O_TRUNC;    // >
            }

            int out_fd = open(cmd.output_file, flags, 0644);
            if (out_fd < 0) {
                perror("open output failed");
                exit(1);
            }
            if (dup2(out_fd, STDOUT_FILENO) < 0) {
                perror("dup2 output failed");
                close(out_fd);
                exit(1);
            }
            close(out_fd);
        }

        // build argv array for execvp
        char *argv[MAX_ARGS];
        argv[0] = cmd.command;

        int i = 0;
        while (cmd.args[i] != NULL && i < MAX_ARGS - 2) {
            argv[i + 1] = cmd.args[i];
            i++;
        }
        argv[i + 1] = NULL;

        execvp(cmd.command, argv);
        perror("execvp failed");
        exit(1);
    }

    // PARENT PROCESS
    else {
        // 3) BACKGROUND EXECUTION: &
        if (cmd.background) {
            printf("[Background pid %d]\n", pid);
            return; // do not wait
        }

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Process exited with status %d\n", WEXITSTATUS(status));
        }
    }
}