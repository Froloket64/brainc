#include "compiler.h"
#include "memory_map.h"
#include "vector.h"
#include <stdint.h>

#ifdef DEBUG
#include <stdio.h>
#endif

enum CompResult compile(char *input, vec_t *opcode, enum CompMode mode)
{
    vec_t labels = vec_new();
#ifdef DEBUG
    uint16_t init, irq_handler, irq_exit, wait_for_input, main;
#endif
    uint16_t loop_start = 0;
    enum CompResult rc = Ok;

    if (mode != NoHeader)
    {
// init:
#ifdef DEBUG
        init = opcode->len;
#endif
        VINSTR_0(opcode, SEI);

        // ACIA Setup
        VINSTR_1(opcode, LDA_IMM, ACIA_CONTROL_MODE);
        VINSTR_2(opcode, STA_ABS, LO(ACIA_CONTROL), HI(ACIA_CONTROL));
        VINSTR_1(opcode, LDA_IMM, ACIA_COMMAND_MODE);
        VINSTR_2(opcode, STA_ABS, LO(ACIA_COMMAND), HI(ACIA_COMMAND));

        // IRQ Setup
        VINSTR_1(opcode, LDA_IMM, 0x1D);
        VINSTR_2(opcode, STA_ABS, LO(IRQ_VECTOR), HI(IRQ_VECTOR));
        VINSTR_1(opcode, LDA_IMM, PROGRAM_START_HI);
        VINSTR_2(opcode, STA_ABS, LO(IRQ_VECTOR) + 1, HI(IRQ_VECTOR));

        VINSTR_1(opcode, LDX_IMM, 0);
        VINSTR_1(opcode, LDY_IMM, 0);

        VINSTR_0(opcode, CLI);
        // jmp main
        VINSTR_2(opcode, JMP_ABS, 0x37, PROGRAM_START_HI);

// irq_handler:
#ifdef DEBUG
        irq_handler = opcode->len;
#endif
        VINSTR_0(opcode, PHA);
        VINSTR_2(opcode, LDA_ABS, LO(ACIA_STATUS), HI(ACIA_STATUS));
        VINSTR_1(opcode, AND_IMM, ACIA_RX_FULL_MASK);
        VINSTR_1(opcode, BEQ, 0x05);
        VINSTR_2(opcode, LDA_ABS, LO(ACIA_KEY), HI(ACIA_KEY));
        VINSTR_1(opcode, STA_ZERO, KEY_ADDR);

// irq_exit:
#ifdef DEBUG
        irq_exit = opcode->len;
#endif
        VINSTR_0(opcode, PLA);
        VINSTR_0(opcode, RTI);

// wait_for_input:
#ifdef DEBUG
        wait_for_input = opcode->len;
#endif
        VINSTR_1(opcode, LDA_ZERO, KEY_ADDR);
        VINSTR_1(opcode, BEQ, -0x04);

        VINSTR_1(opcode, STA_REL_X, 0);

        VINSTR_1(opcode, LDA_IMM, 0);
        VINSTR_1(opcode, STA_ZERO, KEY_ADDR);
        VINSTR_0(opcode, RTS);

#ifdef DEBUG
        printf("%02X, %02X, %02X, %02X, ", init, irq_handler, irq_exit, wait_for_input);
#endif
    }

// main:
#ifdef DEBUG
    main = opcode->len;

    printf("%02X\n", main);
#endif

    while (*input)
    {
        switch (*input++)
        {
        case '+':
            VINSTR_1(opcode, INC_REL_X, MEM_START_LO);

            break;
        case '-':
            VINSTR_1(opcode, DEC_REL_X, MEM_START_LO);

            break;
        case '>':
            VINSTR_0(opcode, INX);

            break;
        case '<':
            VINSTR_0(opcode, DEX);

            break;
        case '.':
            VINSTR_1(opcode, LDA_REL_X, MEM_START_LO);
            // HACK?
            VINSTR_2(opcode, STA_ABS_REL_Y, LO(DISPLAY_START), HI(DISPLAY_START));
            VINSTR_0(opcode, INY);

            break;
        case ',':
            // jsr wait_for_input
            VINSTR_2(opcode, JSR, 0x2C, PROGRAM_START_HI);

            break;
        case '[':
            VINSTR_1(opcode, LDA_REL_X, MEM_START_LO);
            VINSTR_0(opcode, BEQ);

            // Replaced later by `]`
            vec_push(&labels, opcode->len);
            VINSTR_0(opcode, 0);

            break;
        case ']':
            VINSTR_1(opcode, LDA_REL_X, MEM_START_LO);
            VINSTR_0(opcode, BNE);

            if (labels.len == 0)
            {
                rc = LoopNotStarted;
                goto exit;
            }

            vec_pop(&labels, (opcode_t *)&loop_start);

            VINSTR_0(opcode, (opcode_t)(loop_start - opcode->len));
            opcode->buf[loop_start] = (opcode_t)(opcode->len - loop_start - 1);

            break;
        // DEBUG
        case '?':
            VINSTR_0(opcode, NOP);

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
    free(labels.buf);

    return rc;
}
