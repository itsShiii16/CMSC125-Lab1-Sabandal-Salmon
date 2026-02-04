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

---

## Usage Instructions

---

## Implemented Features

---

## Known Limitations / Bugs

---

## Design Decisions & Architecture Overview


### Core Architecture

---

### Command Data Structure

The shell uses a shared structure between parsing and execution:

    typedef struct {
        char *command;
        char *args[256];

        char *input_file;
        char *output_file;

        bool append;
        bool background;
    } Command;

This ensures parsing and execution remain decoupled and maintainable.

---

## Testing Strategy

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
