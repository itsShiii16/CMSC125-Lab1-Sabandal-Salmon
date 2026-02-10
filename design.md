## Estimated Implementation Timeline  
### 4-Week Accomplishment Period

---

## Week 1 – Planning & Foundation *(Current Week)*

**Focus:**  
Design, architecture, and environment setup.

### Accomplishments
- Analyzed Lab 1 requirements and identified core operating system concepts involved:
  - Process creation and execution
  - I/O redirection
  - Background job handling
- Set up a UNIX-like development environment capable of compiling and running POSIX-compliant C programs.
- Initialized a public GitHub repository with a clean, modular directory structure:
  - `src/`
  - `include/`
  - `tests/`
- Implemented a basic REPL (read–eval–print loop) that:
  - Displays a shell prompt
  - Accepts user input
  - Handles end-of-file (EOF) termination gracefully
- Designed the overall system architecture, including:
  - A shared `Command` data structure acting as the interface between parsing and execution
  - A clear separation between parsing logic and execution logic
- Drafted the parsing and execution strategies to be implemented in subsequent weeks.

**Status:**  *Completed (Planning Phase)*

---

## Week 2 – Core Execution & Built-in Commands

**Focus:**  
Making the shell functionally usable.

### Planned Tasks
- Implement built-in commands executed directly in the parent process:
  - `exit`
  - `cd`
  - `pwd`
- Implement foreground external command execution using:
  - `fork()` for process creation
  - `execvp()` for program execution
  - `waitpid()` for synchronization
- Integrate the parser with the executor to ensure parsed commands are correctly routed.
- Add basic error handling for invalid commands and failed system calls.
- Maintain incremental, well-documented commits throughout the week.

### Expected Outcome
- A working shell capable of executing standard UNIX commands in the foreground.

---

## Week 3 – I/O Redirection & Background Processing

**Focus:**  
Advanced shell features and resource management.

### Planned Tasks
- Implement input and output redirection:
  - Input redirection (`<`)
  - Output overwrite (`>`)
  - Output append (`>>`)
- Use `open()` and `dup2()` to correctly redirect file descriptors.
- Support combined input and output redirection within a single command.
- Implement background execution using the `&` operator.
- Add a background process tracking mechanism and reap completed processes using:
  - `waitpid(..., WNOHANG)` to prevent zombie processes.
- Create test cases covering redirection and background execution scenarios.

### Expected Outcome
- A fully featured shell meeting all functional requirements of Lab 1.

---

## Week 4 – Testing, Refinement & Defense Preparation

**Focus:**  
Stability, documentation, and evaluation readiness.

### Planned Tasks
- Conduct extensive testing for edge cases and error scenarios.
- Document known limitations, design decisions, and implementation trade-offs.
- Finalize the `README.md` with usage instructions and feature descriptions.
- Prepare for the laboratory defense by reviewing:
  - Design decisions
  - System call usage
- Apply any refactors or feedback provided by the instructor during progress reports.

### Expected Outcome
- A stable, well-documented implementation ready for laboratory defense.

---
