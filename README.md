# README.md — CMSC 125 Lab 1: Unix Shell

## CMSC 125 Laboratory Manual 1
### Lab 1 — Simplified Unix Shell Implementation

---

## Group Members

- Antonio Gabriel Salmon
- Sherwin Paul Sabandal

---

## Project Overview

This project implements a simplified Unix shell called `mysh`, designed to demonstrate key operating system concepts such as:

- Process creation and execution
- Foreground and background job handling
- Input/output redirection
- Built-in command execution
- Command parsing and tokenization

The shell is written in POSIX-compliant C and follows modular design principles required in CMSC 125 Lab 1.

---

## Compilation Instructions

```bash
gcc src/main.c src/parser.c -Iinclude -o mysh
```

---

## Usage Instructions

After compilation, run the shell using:
```bash
./mysh
```
You should see the prompt:
```bash
mysh >
```

You can then enter commands interactively.
Example
```bash
mysh > ls
mysh > echo Hello World
mysh > pwd
mysh > cd ..
mysh > exit
```

---

## Implemented Features

### 1. Command Parsing
The shell reads user input and tokenizes commands and arguments using a custom parser.

Example:
```bash
ls -l
```

Parsed as:
- Command: ls
- Arguments: -l


### 2. Built-in Commands (Executed in Parent Process)
The following built-in commands are handled directly without creating a child process:

- exit - Terminates the shell session.
```bash
mysh > exit
```

- cd - Changes the current working directory of the shell.
```bash
mysh > cd foldername
```

NOTE: If no directory is provided, it defaults to the HOME directory.

- pwd - Prints the current working directory.

```bash
mysh > pwd
```
### 3. Foreground External Command Execution
External commands are executed using process creation and synchronization:

- fork() creates a child process
- execvp() runs the specified program
- waitpid() makes the parent shell wait until the child finishes

Example:
```bash
mysh > ls
mysh > sleep 2
mysh > echo Testing
```

---

## Known Limitations / Bugs
- Background execution using & is parsed but not yet implemented.
- Input/output redirection (<, >, >>) is included in the parser structure but not yet functional.

---

## Design Decisions & Architecture Overview

### Core Architecture

The shell follows a modular structure:
- main.c handles the shell loop and execution logic
- parser.c handles tokenization and command parsing
- parser.h defines shared structures and prototypes

This separation ensures that parsing and execution remain independent.

---

### Command Data Structure

The shell uses a shared structure between parsing and execution:
```bash
    typedef struct {
        char *command;
        char *args[256];

        char *input_file;
        char *output_file;

        bool append;
        bool background;
    } Command;
```

This ensures parsing and execution remain decoupled and maintainable.

---

## Testing Strategy

The shell was tested using common Unix commands:

- ls, echo, sleep
- built-ins: pwd, cd, exit

Example test sequence:
```bash
mysh > pwd
mysh > cd ..
mysh > pwd
mysh > ls
mysh > exit
```

---

## Screenshots (Functionality Demo)

---

## Repository Structure

---

## Lessons Learned

---

## Submission Notes

---

# End of README

