#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(void) {
    char line[MAX_LINE];

    while (1) {
        /* Print shell prompt */
        printf("mysh > ");
        fflush(stdout);

        /* Read input */
        if (fgets(line, sizeof(line), stdin) == NULL) {
            /* Handle Ctrl+D (EOF) */
            printf("\n");
            break;
        }

        /* Remove trailing newline */
        line[strcspn(line, "\n")] = '\0';

        /* Ignore empty input */
        if (strlen(line) == 0) {
            continue;
        }

        /* Temporary behavior (Week 1 foundation only) */
        printf("You entered: %s\n", line);
    }

    return 0;
}
