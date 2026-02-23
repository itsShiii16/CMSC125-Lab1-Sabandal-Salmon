// parser.h

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_LINE 1024
#define MAX_ARGS 256

// Command structure to hold parsed data
typedef struct {
    char *command;        // Primary command (e.g., "ls")
    char *args[MAX_ARGS]; // Arguments array (e.g., ["-la"])
    char *input_file;     // Input file for redirection (<)
    char *output_file;    // Output file for redirection (>)
    bool append;          // Flag for append mode (>>)
    bool background;      // Flag for background process (&)
} Command;

// Function declarations
void trim_newline(char *line);
Command parse_input(char *line);
void print_command(Command cmd);

#endif // PARSER_H
