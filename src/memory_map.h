/**
 * @file memory_map.h
 * @brief Describes the memory map used in the emulator.
 */

#ifndef MEMORY_MAP_H_
#define MEMORY_MAP_H_

/** @brief Start of the display memory (used for printing to the screen).
 *
 * NOTE: If your emulator has a display that allows drawing pixels, instead of
 * characters, printing ('.') might not produce expected results.
 */
#define DISPLAY_START 0x7000

/** @brief MOS 6551 ACIA Key Register. */
#define ACIA_KEY     0x8800
/** @brief MOS 6551 ACIA Status Register. */
#define ACIA_STATUS  0x8801
/** @brief MOS 6551 ACIA Control Register. */
#define ACIA_CONTROL 0x8803
/** @brief MOS 6551 ACIA Command Register. */
#define ACIA_COMMAND 0x8802

/** @brief IRQ vector.
 *
 * Used to store the keyboard input (interrupt) handler.
 */
#define IRQ_VECTOR 0xFFFE

/** @brief Mode passed to the Control ACIA register at setup. */
#define ACIA_CONTROL_MODE 0b00011110
/** @brief Mode passed to the Command ACIA register at setup. */
#define ACIA_COMMAND_MODE 0b00001001
/** @brief Rx Full (Receiver Full) mask.
 *
 * Used for checking if a key was pressed. Set by the ACIA. */
#define ACIA_RX_FULL_MASK 0b00010000

/* #define PROGRAM_START_LO  */
/** @brief Program code starting address.
 *
 * Indicated where the program will be stored in memory.
 * Must be set to a correct value, otherwise the emitted code will not work as expected. */
#define PROGRAM_START_HI 0x03
// TODO
/* #define MEM_START_HI 0x00 */
/** @brief Usable memory starting address. */
#define MEM_START_LO 0x00
/** @brief Always null address.
 *
 * Might be used internally later (for example, for tape out of bounds detection). */
#define NULL_ADDR 0x00
/** @brief Address to store the key received after input. */
#define KEY_ADDR 0xff

#endif // MEMORY_MAP_H_
