#include "io.h"

#include <stdio.h>

#include "read_file.h"
#include "write_file.h"

AssemblerErr_t Read_File_2_Buffer(Assembler* ASM, const char* file_name, const size_t file_size) {
    size_t num_of_read_elements = ReadFile(ASM->InputBuffer.data, file_size, file_name);
    if (num_of_read_elements < ASM->InputBuffer.capacity - 1) {
        fprintf(stderr, "Not all possible bytes were read!\n");
        return ASM_READING_ERROR;
    }

    ASM->InputBuffer.data[ASM->InputBuffer.capacity - 1] = 0;
    ASM->InputBuffer.size = ASM->InputBuffer.capacity;

    return ASM_OK;
}

AssemblerErr_t Write_Buffer_2_File(Assembler* ASM, const char* file_name) {
    size_t num_of_wrote_elements = WriteFile((const int*)ASM->OutPutBuffer->data,
                                             ASM->OutPutBuffer->meta.size, file_name);
    
    if (num_of_wrote_elements < ASM->OutPutBuffer->meta.size) {
        fprintf(stderr, "Not all possible bytes were written!\n");
        return ASM_WRITING_ERROR;
    }

    return ASM_OK;
}
