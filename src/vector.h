#ifndef VECTOR_H_
#define VECTOR_H_

#include "instr.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define INIT_CAPACITY 16

struct Vec
{
    opcode_t *buf;
    size_t len;
    size_t capacity;
};

typedef struct Vec vec_t;

vec_t vec_new(void);
vec_t vec_with_capacity(size_t capacity);

/** @brief Pushes an element to the end of the vector. */
void vec_push(vec_t *vec, opcode_t x);
/** @brief Pops an elements off the end of the vector.
 *
 * SAFETY:
 * `vec->len` must not be zero, otherwise `x` is unchanged (panic in debug mode).
 */
void vec_pop(vec_t *vec, opcode_t *x);

/** @brief Returns the last/top element in the vector.
 *
 * SAFETY:
 * If `vec->len` is zero, the behavior is undefined. Moreover, this will
 * likely lead to a panic, because of subtraction overflow.
 */
opcode_t vec_last(vec_t *vec);
/** @brief Returns the first/bottom element in the vector.
 *
 * SAFETY:
 * If `vec->len` is zero, the behavior is undefined.
 */
opcode_t vec_first(vec_t *vec);

#endif // VECTOR_H_
