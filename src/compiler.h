#ifndef COMPILER_H_
#define COMPILER_H_

#include <stddef.h>
#include "instr.h"

enum CompResult {
    Ok,
    LoopNotStarted,
    LoopNotFinished,
};

enum CompMode {
    Normal,
    /** @brief Produces a raw result without any subroutine setup (only `main`).
     *
     * Useful for testing or debugging emitted code.
     */
    NoHeader,
};

enum CompResult compile(char *input, opcode_t *opcode, size_t *opcode_n, size_t opcode_max_n, enum CompMode mode);

#endif // COMPILER_H_
