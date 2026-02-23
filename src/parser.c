// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/parser.h"      // Make sure this is included


// Function to trim newline from input
void trim_newline(char *line) {
    line[strcspn(line, "\n")] = '\0';
}

// Function to parse the input line into a Command struct
Command parse_input(char *line) {
    Command cmd;
    cmd.command = NULL;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.append = false;
    cmd.background = false;

    char *token = strtok(line, " ");
    int arg_count = 0;

    while (token != NULL) {
        // Check for redirection operators
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            cmd.input_file = token;
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            cmd.output_file = token;
            cmd.append = false;  // Default to overwrite
        }
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            cmd.output_file = token;
            cmd.append = true;   // Append mode
        }
        else if (strcmp(token, "&") == 0) {
            cmd.background = true;  // Background process flag
        }
        else {
            // Store the command or arguments
            if (cmd.command == NULL) {
                cmd.command = token;  // The first token is the command itself
            } else {
                cmd.args[arg_count++] = token;  // Add arguments to the array
            }
        }
        token = strtok(NULL, " ");  // Get next token
    }

    cmd.args[arg_count] = NULL;  // Null-terminate the args array
    return cmd;
}

// Function to print the parsed Command (for testing)
void print_command(Command cmd) {
    printf("Command: %s\n", cmd.command);
    printf("Arguments: ");
    for (int i = 0; cmd.args[i] != NULL; i++) {
        printf("%s ", cmd.args[i]);
    }
    printf("\n");

    if (cmd.input_file) {
        printf("Input File: %s\n", cmd.input_file);
    }
    if (cmd.output_file) {
        printf("Output File: %s\n", cmd.output_file);
        if (cmd.append) {
            printf("Append Mode: Yes\n");
        } else {
            printf("Append Mode: No\n");
        }
    }

    printf("Background: %s\n", cmd.background ? "Yes" : "No");
}
