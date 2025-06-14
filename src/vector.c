#include "vector.h"
#include <assert.h>

vec_t vec_new(void)
{
    vec_t vec;
    opcode_t *buf = (opcode_t *)malloc(sizeof(opcode_t) * INIT_CAPACITY);

    vec.buf = buf;
    vec.len = 0;
    vec.capacity = INIT_CAPACITY;

    return vec;
}

vec_t vec_with_capacity(size_t capacity)
{
    vec_t vec;
    opcode_t *buf = (opcode_t *)malloc(sizeof(opcode_t) * capacity);

    vec.buf = buf;
    vec.len = 0;
    vec.capacity = INIT_CAPACITY;

    return vec;
}

void vec_push(vec_t *vec, opcode_t x)
{
    assert(vec != NULL);

    if (vec->len == 0)
    {
        vec->buf = realloc(vec->buf, sizeof(opcode_t) * INIT_CAPACITY);
        vec->capacity = INIT_CAPACITY;
    }
    else if (vec->len == vec->capacity)
    {
        vec->buf = realloc(vec->buf, sizeof(opcode_t) * vec->capacity * 2);
        vec->capacity *= 2;
    }

    vec->buf[vec->len] = x;
    vec->len += 1;
}

void vec_pop(vec_t *vec, opcode_t *x)
{
    assert(vec != NULL);
    assert(vec->len > 0);

    *x = vec_last(vec);
    vec->len -= 1;
}

opcode_t vec_last(vec_t *vec)
{
    assert(vec != NULL);
    assert(vec->len > 0);

    return vec->buf[vec->len - 1];
}

opcode_t vec_first(vec_t *vec)
{
    assert(vec != NULL);
    assert(vec->len > 0);

    return vec->buf[0];
}
