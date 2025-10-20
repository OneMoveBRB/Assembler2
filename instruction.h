#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "asm_settings.h"

struct Instruction {
    const char* word;
    size_t id;
};

enum InstructionType {
    IN      = 1,
    PUSH    = 2,
    POP     = 3,
    PUSHR   = 4,
    POPR    = 5,
    ADD     = 6,
    SUB     = 7,
    MUL     = 8,
    OUT     = 9,
    HLT     = 10,
    JA      = 13,
    JAE     = 14,
    JB      = 11,
    JBE     = 12,
    JE      = 15,
    JNE     = 16,
    JMP     = 17
};

enum RegsType {
    RAX     = 1,
    RBX     = 2,
    RCX     = 3
};


AssemblerErr_t WordInit(Stack_t* string, char** word);
AssemblerErr_t WordDestroy(Stack_t* Labels);

#endif
