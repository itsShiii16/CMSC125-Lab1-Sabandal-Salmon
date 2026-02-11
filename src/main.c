#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>                 // to have access to chdir(), fork(), execvp(), getcwd()
#include <sys/types.h>              // for types of pid
#include <sys/wait.h>               // to have access to waitpid()

#include "../include/parser.h"      // Make sure this is included

#define MAX_LINE 1024

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

    // for child process
    if (pid == 0) {
        char *argv[MAX_ARGS];                                               // build argv array for execvp
        argv[0] = cmd.command;
        int i = 0;
        
        while (cmd.args[i] != NULL) {
            argv[i + 1] = cmd.args[i];
            i++;
        }

        argv[i + 1] = NULL;
        execvp(cmd.command, argv);                                          // execute program
        perror("execvp failed");                                            // if execvp fails
        exit(1);
    }

    // for parent process
    else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Process exited with status %d\n", WEXITSTATUS(status));
        }
    }
}


int main(void) {
    char line[MAX_LINE];

    while (1) {
        // Print shell prompt
        printf("mysh > ");
        fflush(stdout);

        // Read input
        if (fgets(line, sizeof(line), stdin) == NULL) {
            // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }

        // Trim newline character from input
        trim_newline(line);

        // Ignore empty input
        if (strlen(line) == 0) {
            continue;
        }

        // Parse the input line
        Command cmd = parse_input(line);

        // for built-in commands
        if (handle_builtin(cmd)) {
            continue;
        }

        // for external commands
        execute_external(cmd);


        // Print the parsed command (for testing)
        // print_command(cmd);
    }

    return 0;
}
