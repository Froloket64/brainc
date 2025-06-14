#ifndef COMPILER_H_
#define COMPILER_H_

#include "instr.h"
#include "vector.h"
#include <stddef.h>

enum CompResult
{
    Ok,
    LoopNotStarted,
    LoopNotFinished,
};

enum CompMode
{
    Normal,
    /** @brief Produces a raw result without any subroutine setup (only `main`).
     *
     * Useful for testing or debugging emitted code.
     */
    NoHeader,
};

enum CompResult compile(char *input, vec_t *opcode, enum CompMode mode);

#endif // COMPILER_H_
