#ifndef INSTR_H_
#define INSTR_H_

typedef unsigned char opcode_t;

#define LDA_IMM 0xA9
#define LDA_ZERO 0xA5
#define LDA_ABS 0xAD
#define LDA_REL_X 0xB5
#define LDX_IMM 0xA2
#define LDY_IMM 0xA0

#define STA_ABS 0x8D
#define STA_ABS_REL_Y 0x99
#define STA_ZERO 0x85
#define STA_REL_X 0x95

#define DEC_REL_X 0xD6
#define INC_REL_X 0xF6

#define INX 0xE8
#define DEX 0xCA
#define INY 0xC8
#define DEY 0x88

#define AND_IMM 0x29

#define CMP 0xC9
#define BEQ 0xF0
#define BNE 0xD0
#define JMP_ABS 0x4C

#define SEI 0x78
#define CLI 0x58

#define PHA 0x48
#define PLA 0x68

#define JSR 0x20
#define RTI 0x40
#define RTS 0x60

/** @brief Used for debug. */
#define NOP 0xEA

#define INSTR_0(OPCODE, PC, MNEMONIC)                                                                                  \
    {                                                                                                                  \
        OPCODE[PC++] = MNEMONIC;                                                                                       \
    }

#define INSTR_1(OPCODE, PC, MNEMONIC, ARG1)                                                                            \
    {                                                                                                                  \
        OPCODE[PC++] = MNEMONIC;                                                                                       \
        OPCODE[PC++] = ARG1;                                                                                           \
    }

#define INSTR_2(OPCODE, PC, MNEMONIC, ARG1, ARG2)                                                                      \
    {                                                                                                                  \
        OPCODE[PC++] = MNEMONIC;                                                                                       \
        OPCODE[PC++] = ARG1;                                                                                           \
        OPCODE[PC++] = ARG2;                                                                                           \
    }

/** @brief Takes the low byte of the (uint16_t) address. */
#define LO(ADDR) (opcode_t) ADDR
/** @brief Takes the high byte of the (uint16_t) address. */
#define HI(ADDR) (opcode_t)(ADDR >> 8)

#endif // INSTR_H_
