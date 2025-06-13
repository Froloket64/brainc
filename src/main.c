#include "compiler.h"
#include "display.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// HACK
#define MAX_OPCODE_LEN 512

char *fread_to_str(FILE *f);

int main(int argc, char **argv)
{
    char *input;
    size_t input_len;
    size_t opcodes_n = 0;
    // HACK
    opcode_t *opcodes;
    char *out;
    size_t i = 0;
    bool verbose = false;
    long input_size;
    FILE *src, *dest;

    if (argc != 3 && argc != 4)
    {
        printf("Usage: ./brainc IN OUT [-v]");
        return 1;
    }

    if (argc == 4 && strcmp(argv[3], "-v") == 0)
        verbose = true;

    src = fopen(argv[1], "rb");
    input = fread_to_str(src);
    fclose(src);

    input_len = strlen(input);

    // TODO? OPTIM: Cut down on the memory, boy
    opcodes = malloc(sizeof(opcode_t) * 8 * input_len + 20);

    enum CompResult result = compile(input, opcodes, &opcodes_n, MAX_OPCODE_LEN);

    if (result != Ok)
    {
        printf("Compilation failed");
        return 1;
    }

    dest = fopen(argv[2], "wb");
    fwrite(opcodes, sizeof(opcode_t), opcodes_n, dest);
    fclose(dest);

    if (verbose)
    {
        out = malloc(1000 * 1000 * 4);
        fmt_code(opcodes, opcodes_n, out);

        printf("asm:\n%s\n", out);

        free(out);
    }

    free(input);
    free(opcodes);

    return 0;
}

char *fread_to_str(FILE *f)
{
    char *buf;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = malloc(file_size + 1);
    fread(buf, 1, file_size, f);

    buf[file_size] = 0;

    return buf;
}
