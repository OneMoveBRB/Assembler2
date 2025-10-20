#ifndef ASM_SETTINGS_H
#define ASM_SETTINGS_H

#include <stddef.h>
#include <stdint.h>

#include "../stack/stack.h"

const int FIRST_SIZE = 8;

struct TempBuffer_t {
    char* data;
    size_t size;
    size_t capacity;
};

struct Assembler {
    TempBuffer_t InputBuffer;
    Stack_t* OutPutBuffer;
};

enum AssemblerErr_t {
    ASM_OK = 0,
    ASM_OVERFLOW,
    ASM_READING_ERROR,
    ASM_GETWORD_FAILED,
    ASM_INVALID_COMMAND,
    ASM_INVALID_NUMBER
};

#endif
