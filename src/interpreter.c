// interpreter.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "../include/interpreter.h"

// ----------------------
// Background job tracking
// ----------------------
#define MAX_BG_JOBS 64

typedef struct {
    int job_id;
    pid_t pid;
    char name[128];
} BgJob;

static BgJob bg_jobs[MAX_BG_JOBS];
static int bg_count = 0;
static int next_job_id = 1;

static void add_bg_job(pid_t pid, const char *name) {
    if (bg_count >= MAX_BG_JOBS) {
        fprintf(stderr, "warning: too many background jobs; not tracking pid %d\n", pid);
        return;
    }
    bg_jobs[bg_count].job_id = next_job_id++;
    bg_jobs[bg_count].pid = pid;

    // copy command name safely (at least cmd.command)
    if (name == NULL) name = "(unknown)";
    strncpy(bg_jobs[bg_count].name, name, sizeof(bg_jobs[bg_count].name) - 1);
    bg_jobs[bg_count].name[sizeof(bg_jobs[bg_count].name) - 1] = '\0';

    printf("[%d] Started background job: %s (PID: %d)\n",
           bg_jobs[bg_count].job_id, bg_jobs[bg_count].name, bg_jobs[bg_count].pid);

    bg_count++;
}

static void remove_bg_job_by_pid(pid_t pid) {
    for (int i = 0; i < bg_count; i++) {
        if (bg_jobs[i].pid == pid) {
            bg_jobs[i] = bg_jobs[bg_count - 1];
            bg_count--;
            return;
        }
    }
}

// Call this regularly (e.g., every loop) to avoid zombies.
void reap_background_jobs(void) {
    int status;
    pid_t done;

    // Reap any finished child without blocking
    while ((done = waitpid(-1, &status, WNOHANG)) > 0) {
        // Print a "done" message if it was tracked as a bg job
        for (int i = 0; i < bg_count; i++) {
            if (bg_jobs[i].pid == done) {
                if (WIFEXITED(status)) {
                    printf("[%d] Done: %s (PID: %d) exit=%d\n",
                           bg_jobs[i].job_id, bg_jobs[i].name, (int)done, WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("[%d] Done: %s (PID: %d) signal=%d\n",
                           bg_jobs[i].job_id, bg_jobs[i].name, (int)done, WTERMSIG(status));
                } else {
                    printf("[%d] Done: %s (PID: %d)\n",
                           bg_jobs[i].job_id, bg_jobs[i].name, (int)done);
                }

                remove_bg_job_by_pid(done);
                break;
            }
        }
    }

    // If done == 0: nothing finished (normal)
    // If done < 0: could be "no children" or real error
    if (done < 0 && errno != ECHILD) {
        perror("waitpid(WNOHANG) failed");
    }
}

// ----------------------
// Built-ins
// ----------------------
int handle_builtin(Command cmd) {
    if (cmd.command == NULL) return 1; // nothing to do

    // exit
    if (strcmp(cmd.command, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // cd
    if (strcmp(cmd.command, "cd") == 0) {
        if (cmd.args[0] == NULL) {
            char *home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
            if (chdir(home) != 0) {
                perror("cd failed");
            }
        } else {
            if (chdir(cmd.args[0]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }

    // pwd
    if (strcmp(cmd.command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd failed");
        }
        return 1;
    }

    return 0; // not a builtin
}

// ----------------------
// External execution
// ----------------------
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
            int flags = O_WRONLY | O_CREAT | (cmd.append ? O_APPEND : O_TRUNC);

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
        if (cmd.background) {
            // Track it and DO NOT WAIT
            add_bg_job(pid, cmd.command);
            return;
        }

        // Foreground: wait normally
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Process exited with status %d\n", WEXITSTATUS(status));
        }
    }
}