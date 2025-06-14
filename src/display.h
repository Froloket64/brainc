#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "compiler.h"
#include "instr.h"

#define ERR_UNKNOWN_MNEMONIC 1

/** @brief Writes the formatted code to `out`. */
int fmt_code(opcode_t opcodes[], size_t opcodes_n, char *out);
/** @brief Writes the formatted instruction to `out`. */
size_t fmt_instr(opcode_t *opcodes, char *out);

/** @brief Converts an opcode to a mnemonic string.
 *
 * If return value is `NULL`, `instr` indicates an unknown instruction.
 */
char *to_mnemonic_str(opcode_t instr);

#endif // DISPLAY_H_
