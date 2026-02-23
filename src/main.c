#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/interpreter.h"

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
