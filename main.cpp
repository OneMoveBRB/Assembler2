#include <stdio.h>

#include "asm.h"

int main() {
    Assembler ASM;
    const char* input_file_name  = "instructions.asm";
    const char* output_file_name = "bytecode.bin";

    AssemblerInit(&ASM, input_file_name);

    if ( Read_File_2_Buffer(&ASM, input_file_name, ASM.InputBuffer.capacity - 1) != ASM_OK )
        return 1;

    // Используется qsort и бин поиск
    Translation(&ASM);

    Write_Buffer_2_File(&ASM, output_file_name);

    AssemblerDestroy(&ASM);
    
    return 0;
}
