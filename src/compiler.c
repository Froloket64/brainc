#include "compiler.h"
#include "vector.h"
#include <stdint.h>
#include "memory_map.h"

#ifdef DEBUG
#include <stdio.h>
#endif

// TODO? Switch over to a vector, or other means of extensible `opcode` storage.
enum CompResult compile(char *input, opcode_t *opcode, size_t *opcode_n, size_t opcode_max_n)
{
    vec_t labels = vec_new();
    uint16_t pc = 0;
    #ifdef DEBUG
    uint16_t init, irq_handler, irq_exit, wait_for_input, main;
    #endif
    uint16_t loop_start;
    enum CompResult rc = Ok;

    // init:
    #ifdef DEBUG
    init = pc;
    #endif
    INSTR_0(opcode, pc, SEI);

    // ACIA Setup
    INSTR_1(opcode, pc, LDA_IMM, ACIA_CONTROL_MODE);
    INSTR_2(opcode, pc, STA_ABS, LO(ACIA_CONTROL), HI(ACIA_CONTROL));
    INSTR_1(opcode, pc, LDA_IMM, ACIA_COMMAND_MODE);
    INSTR_2(opcode, pc, STA_ABS, LO(ACIA_COMMAND), HI(ACIA_COMMAND));

    // IRQ Setup
    INSTR_1(opcode, pc, LDA_IMM, 0x1D);
    INSTR_2(opcode, pc, STA_ABS, LO(IRQ_VECTOR), HI(IRQ_VECTOR));
    INSTR_1(opcode, pc, LDA_IMM, PROGRAM_START_HI);
    INSTR_2(opcode, pc, STA_ABS, LO(IRQ_VECTOR) + 1, HI(IRQ_VECTOR));

    INSTR_1(opcode, pc, LDX_IMM, 0);
    INSTR_1(opcode, pc, LDY_IMM, 0);

    INSTR_0(opcode, pc, CLI);
    // jmp main
    INSTR_2(opcode, pc, JMP_ABS, 0x37, PROGRAM_START_HI);

    // irq_handler:
    #ifdef DEBUG
    irq_handler = pc;
    #endif
    INSTR_0(opcode, pc, PHA);
    INSTR_2(opcode, pc, LDA_ABS, LO(ACIA_STATUS), HI(ACIA_STATUS));
    INSTR_1(opcode, pc, AND_IMM, ACIA_RX_FULL_MASK);
    INSTR_1(opcode, pc, BEQ, 0x05);
    INSTR_2(opcode, pc, LDA_ABS, LO(ACIA_KEY), HI(ACIA_KEY));
    INSTR_1(opcode, pc, STA_ZERO, KEY_ADDR);

    // irq_exit:
    #ifdef DEBUG
    irq_exit = pc;
    #endif
    INSTR_0(opcode, pc, PLA);
    INSTR_0(opcode, pc, RTI);

    // wait_for_input:
    #ifdef DEBUG
    wait_for_input = pc;
    #endif
    INSTR_1(opcode, pc, LDA_ZERO, KEY_ADDR);
    INSTR_1(opcode, pc, BEQ, -0x04);

    INSTR_1(opcode, pc, STA_REL_X, 0);

    INSTR_1(opcode, pc, LDA_IMM, 0);
    INSTR_1(opcode, pc, STA_ZERO, KEY_ADDR);
    INSTR_0(opcode, pc, RTS);

    // main:
    #ifdef DEBUG
    main = pc;
    #endif

    #ifdef DEBUG
    printf("%02X, %02X, %02X, %02X, %02X\n", init, irq_handler, irq_exit, wait_for_input, main);
    #endif

    while (*input)
    {
        switch (*input++)
        {
            case '+':
                INSTR_1(opcode, pc, INC_REL_X, MEM_START_LO);

                break;
            case '-':
                INSTR_1(opcode, pc, DEC_REL_X, MEM_START_LO);

                break;
            case '>':
                INSTR_0(opcode, pc, INX);

                break;
            case '<':
                INSTR_0(opcode, pc, DEX);

                break;
            case '.':
                INSTR_1(opcode, pc, LDA_REL_X, MEM_START_LO);
                // HACK?
                INSTR_2(opcode, pc, STA_ABS_REL_Y, LO(DISPLAY_START), HI(DISPLAY_START));
                INSTR_0(opcode, pc, INY);

                break;
            case ',':
                // jsr wait_for_input
                INSTR_2(opcode, pc, JSR, 0x2C, PROGRAM_START_HI);

                break;
            case '[':
                INSTR_1(opcode, pc, LDA_REL_X, MEM_START_LO);
                INSTR_0(opcode, pc, BEQ);

                // Replaced later by `]`
                vec_push(&labels, pc);
                pc++;

                break;
            case ']':
                INSTR_1(opcode, pc, LDA_REL_X, MEM_START_LO);
                INSTR_0(opcode, pc, BNE);

                if (labels.len == 0)
                {
                    rc = LoopNotStarted;
                    goto exit;
                }

                vec_pop(&labels, (opcode_t *) &loop_start);

                opcode[pc] = (opcode_t) (loop_start - pc);
                pc++;
                opcode[loop_start] = (opcode_t) (pc - loop_start - 1);

                break;
            // DEBUG
            case '?':
                opcode[pc++] = NOP;

                break;
            default:
                // Skip
                break;
        }
    }

    // DEBUG
    /* INSTR_1(opcode, pc, LDA_IMM, 0x00); */
    /* INSTR_2(opcode, pc, STA_ABS, LO(IRQ_VECTOR), HI(IRQ_VECTOR)); */
    /* INSTR_1(opcode, pc, LDA_IMM, 0x20); */
    /* INSTR_2(opcode, pc, STA_ABS, LO(IRQ_VECTOR) + 1, HI(IRQ_VECTOR)); */

    if (labels.len > 0)
        rc = LoopNotFinished;

exit:
    *opcode_n = pc;

    free(labels.buf);

    return rc;
}
