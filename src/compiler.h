#ifndef COMPILER_H_
#define COMPILER_H_

#include <stddef.h>
#include "instr.h"

enum CompResult {
    Ok,
    LoopNotStarted,
    LoopNotFinished,
};

enum CompResult compile(char *input, opcode_t *opcode, size_t *opcode_n, size_t opcode_max_n);

#endif // COMPILER_H_
