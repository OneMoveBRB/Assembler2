#include <stdio.h>

#include "asm.h"

int main() {
    Assembler ASM;
    const char* input_file_name = "instructions.asm";

    AssemblerInit(&ASM, input_file_name);

    Translation(&ASM);

    AssemblerDestroy(&ASM);
    
    return 0;
}