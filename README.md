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
gcc -Wall -Wextra -std=c11 -Iinclude src/main.c src/parser.c src/interpreter.c -o mysh
```

 ### Flag Explanation

- `-Wall` enables common compiler warnings
- `-Wextra` enables additional warnings
- `-std=c11` enforces C11 standard compliance
- `-Iinclude` specifies header file directory
- `-o mysh` outputs the executable file

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
The shell reads user input and tokenizes commands and arguments using `strtok()`.

#### It identifies:
- Command name
- Arguments
- Input redirection (<)
- Output redirection (>, >>)
- Background operator (&)

Example:
```bash
ls -l
```

Parsed as:
- Command: `ls`
- Arguments: `-l`


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

### 4. Background Job Execution (&)
Commands followed by `&` run in the background.

Example:
```bash
mysh > sleep 5 &
```

Output:
```bash
[1] Started background job: sleep (PID: 12345)
```

The shell does not wait for background processes.

#### Background Job Reaping
- `waitpid(..., WNOHANG)` is used
- Prevents zombie processes
- Completed jobs are automatically detected
- Completion message is printed:

```bash
[1] Done: sleep (PID: 12345) exit=0
```

### 5. Input Redirection (<)
Redirects input from a file.

Example:
```bash
mysh > sort < input.txt
```

#### Implementation details:
- `open()` with `O_RDONLY`
- `dup2()` to redirect to `STDIN_FILENO`

### 6. Output Redirection (> and >>)
#### Overwrite Mode (>)

Example:
```bash
mysh > echo Hello > output.txt
```

- `Uses O_WRONLY | O_CREAT | O_TRUNC`

#### Append Mode (>>)

Example:
```bash
mysh > echo World >> output.txt
```

- `Uses O_WRONLY | O_CREAT | O_TRUNC`

### 7. Zombie Process Prevention
The shell regularly calls:
```bash
reap_background_jobs();
```

This uses:
```bash
waitpid(-1, &status, WNOHANG);
```

#### This ensures:
- No zombie processes remain
- Background jobs are tracked and cleaned up properly

---

## Known Limitations / Bugs
- No support for pipes (|)
- No support for command chaining (&&, ||)
- No advanced job control (fg, bg, jobs)
- No signal handling customization (Ctrl+C behavior is default)

---

## Design Decisions & Architecture Overview

### Modular Structure
The shell follows separation of concerns:
#### main.c
- Shell loop
- Prompt display
- Calls parser
- Calls built-in or external execution
#### parser.c
- Tokenizes input
- Identifies redirection and background operators
- Populates Command struct
#### interpreter.c
- Built-in command handling
- Fork/exec logic
- Redirection handling
- Background job tracking
- Zombie cleanup

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
Tested using:
- ls
- echo
- sleep
- cat
- sort
- pwd
- cd
- exit
- Redirection tests
- Background execution tests

Example full test:
### Interactive Command Loop
```bash
mysh > pwd
mysh > cd /tmp
mysh > pwd
mysh> ls -la
mysh> echo "Hello, World!"
mysh> nonexistent_command
mysh> exit
```

### I/O Redirection
```bash
mysh> ls -la > output.txt
mysh> cat output.txt
mysh> wc -l < output.txt
mysh> echo "appending text" >> output.txt
mysh> sort < unsorted.txt > sorted.txt
```

### Background Execution
```bash
mysh> sleep 10 &
mysh> echo "Shell still responsive"
mysh> sleep 20 &
mysh> ls -la
mysh>
```

---

## Screenshots (Functionality Demo)

---

## Repository Structure
```bash
CMSC125-Lab1-Sabandal-Salmon/
│
├── .vscode/
│   └── c_cpp_properties.json        # VSCode C/C++ configuration
│
├── include/
│   ├── interpreter.h                # Interpreter function declarations
│   └── parser.h                     # Parser structures and prototypes
│
├── src/
│   ├── interpreter.c                # Built-ins, execution, redirection, background jobs
│   ├── main.c                       # Shell loop and program entry point
│   └── parser.c                     # Command parsing logic
│
├── design.md                        # Design explanation and architecture notes
├── Makefile                         # Build automation file
├── mysh                             # Compiled Linux executable
├── mysh.exe                         # Compiled Windows executable
└── README.md                        # Project documentation
```

---

## Lessons Learned
- Proper process management using `fork()` and `execvp()`
- Importance of handling zombie processes
- How file descriptors and `dup2()` enable redirection
- Why built-in commands must run in the parent process
- Practical understanding of Unix process control

---

## Submission Notes
- POSIX-compliant
- Compiles using gcc with warnings enabled
- Background jobs fully implemented
- Input/output redirection fully implemented
- Modular and readable structure

---

# End of README
