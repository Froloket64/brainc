#include "test_compiler.h"
#include "../src/compiler.h"
#include "../src/memory_map.h"
#include <assert.h>
#include <stdbool.h>

bool arr_equal(const opcode_t *lhs, const opcode_t *rhs, size_t count);

void test_compiler(void)
{
    char *input;
    size_t pc;
    vec_t opcode = vec_new();
    opcode_t expected[20];
    enum CompResult result;

    input = "+-><";
    pc = 0;
    vec_clear(&opcode);
    result = compile(input, &opcode, NoHeader);

    INSTR_1(expected, pc, INC_REL_X, 0x00);
    INSTR_1(expected, pc, DEC_REL_X, 0x00);
    INSTR_0(expected, pc, INX);
    INSTR_0(expected, pc, DEX);

    assert(result == Ok);
    assert(opcode.len == pc);
    assert(arr_equal(opcode.buf, expected, pc));

    input = ".";
    pc = 0;
    vec_clear(&opcode);
    result = compile(input, &opcode, NoHeader);

    INSTR_1(expected, pc, LDA_REL_X, MEM_START_LO);
    // HACK?
    INSTR_2(expected, pc, STA_ABS_REL_Y, LO(DISPLAY_START), HI(DISPLAY_START));
    INSTR_0(expected, pc, INY);

    assert(result == Ok);
    assert(opcode.len == pc);
    assert(arr_equal(opcode.buf, expected, pc));

    input = ",";
    pc = 0;
    vec_clear(&opcode);
    result = compile(input, &opcode, NoHeader);

    INSTR_2(expected, pc, JSR, 0x2C, PROGRAM_START_HI);

    assert(result == Ok);
    assert(opcode.len == pc);
    assert(arr_equal(opcode.buf, expected, pc));

    input = "[]";
    pc = 0;
    vec_clear(&opcode);
    result = compile(input, &opcode, NoHeader);

    INSTR_1(expected, pc, LDA_REL_X, MEM_START_LO);
    INSTR_1(expected, pc, BEQ, 0x04);
    INSTR_1(expected, pc, LDA_REL_X, MEM_START_LO);
    INSTR_1(expected, pc, BNE, -0x04);

    assert(result == Ok);
    assert(opcode.len == pc);
    assert(arr_equal(opcode.buf, expected, pc));
}

/** @brief Compares the first `count` elements of two arrays.
 *
 * @return `true` if all equal, `false` otherwise. */
bool arr_equal(const opcode_t *lhs, const opcode_t *rhs, size_t count)
{
    while (--count)
        if (lhs[count] != rhs[count])
            return false;

    return true;
}
