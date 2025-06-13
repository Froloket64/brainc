#include "display.h"
#include <string.h>
#include <stdio.h>

/* #define NEXT(PTR) (*(*PTR)++) */
#define INSTR_MAX_SIZE (3 + 1 + 9)

void write_zero_page_addr(opcode_t addr, char **out);
void write_abs_addr(opcode_t lo, opcode_t hi, char **out);
void write_rel_x_addr(opcode_t addr, char **out);
void write_rel_y_addr(opcode_t addr, char **out);
void write_abs_rel_y_addr(opcode_t lo, opcode_t hi, char **out);
void write_imm(opcode_t x, char **out);

// TODO: Errors
int fmt_code(opcode_t opcodes[], size_t opcodes_n, char *out)
{
    size_t i = 0;
    char instr_s[INSTR_MAX_SIZE] = { 0 };

    while (i < opcodes_n)
    {
        strcpy(instr_s, "\t");
        i += fmt_instr(opcodes + i, instr_s + 1);
        strcat(out, instr_s);
        strcat(out, "\n");
    }

    return 0;
}

char *to_mnemonic_str(opcode_t instr)
{
    switch (instr)
    {
        case LDA_REL_X:
        case LDA_ABS:
        case LDA_IMM:
        case LDA_ZERO:
            return "lda";
            break;
        case LDX_IMM:
            return "ldx";
            break;
        case LDY_IMM:
            return "ldy";
            break;
        case STA_ABS_REL_Y:
        case STA_REL_X:
        case STA_ABS:
        case STA_ZERO:
            return "sta";
            break;
        case DEC_REL_X:
            return "dec";
            break;
        case INC_REL_X:
            return "inc";
            break;
        case INX:
            return "inx";
            break;
        case DEX:
            return "dex";
            break;
        case INY:
            return "iny";
            break;
        case DEY:
            return "dey";
            break;
        case AND_IMM:
            return "and";
            break;
        case CMP:
            return "cmp";
            break;
        case BEQ:
            return "beq";
            break;
        case BNE:
            return "bne";
            break;
        case JMP_ABS:
            return "jmp";
            break;
        case SEI:
            return "sei";
            break;
        case CLI:
            return "cli";
            break;
        case PHA:
            return "pha";
            break;
        case PLA:
            return "pla";
            break;
        case JSR:
            return "jsr";
            break;
        case RTI:
            return "rti";
            break;
        case RTS:
            return "rts";
            break;
        case NOP:
            return "nop";
            break;
        default:
            return NULL;
    }
}

size_t fmt_instr(opcode_t *opcodes, char *out)
{
    opcode_t *ptr = opcodes;

    // TODO: Error
    char *mnemonic = to_mnemonic_str(*ptr);

    if (!mnemonic)
        return 0;

    strcpy(out, mnemonic);
    out += 3;

    switch (*ptr++)
    {
        case INX:
        case DEX:
        case INY:
        case DEY:
        case SEI:
        case CLI:
        case PHA:
        case PLA:
        case RTI:
        case RTS:
        case NOP: // DEBUG
            break;

        case LDA_IMM:
        case LDX_IMM:
        case LDY_IMM:
        case AND_IMM:
            *(out++) = ' ';
            write_imm(*ptr++, &out);

            break;
        case JMP_ABS:
        case LDA_ABS:
        case STA_ABS:
        case JSR:
            *(out++) = ' ';
            // HACK?
            ptr += 2;
            write_abs_addr(*(ptr - 2), *(ptr - 1), &out);

            break;
        case LDA_REL_X:
        case STA_REL_X:
        case DEC_REL_X:
        case INC_REL_X:
            *(out++) = ' ';
            write_rel_x_addr(*ptr++, &out);

            break;
        case STA_ABS_REL_Y:
            *(out++) = ' ';
            // HACK?
            ptr += 2;
            write_abs_rel_y_addr(*(ptr - 2), *(ptr - 1), &out);

            break;
        case CMP:
            *(out++) = ' ';
            write_imm(*ptr++, &out);

            break;
        case BEQ:
        case BNE:
        case LDA_ZERO:
        case STA_ZERO:
            *(out++) = ' ';
            // HACK?
            write_zero_page_addr(*ptr++, &out);

            break;
        default:
            // Unknown instruction
            return 0;
    }

    return ptr - opcodes;
}

void write_zero_page_addr(opcode_t addr, char **out)
{
    sprintf(*out, "$%02x", addr);
    *out += 3;
}

void write_abs_addr(opcode_t lo, opcode_t hi, char **out)
{
    sprintf(*out, "$%02x%02x", hi, lo);
    *out += 5;
}

void write_rel_x_addr(opcode_t addr, char **out)
{
    sprintf(*out, "$%02x,x", addr);
    *out += 5;
}

void write_rel_y_addr(opcode_t addr, char **out)
{
    sprintf(*out, "$%02x,y", addr);
    *out += 5;
}

void write_abs_rel_y_addr(opcode_t lo, opcode_t hi, char **out)
{
    sprintf(*out, "$%02x%02x,y", hi, lo);
    *out += 7;
}

void write_imm(opcode_t x, char **out)
{
    sprintf(*out, "#$%02x", x);
    *out += 4;
}
