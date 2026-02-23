#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

int handle_builtin(Command cmd);
void execute_external(Command cmd);

#endif