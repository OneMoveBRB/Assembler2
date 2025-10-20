#ifndef ASM_ALLOC_H
#define ASM_ALLOC_H

#include "asm_settings.h"

AssemblerErr_t AssemblerInit(Assembler* ASM, const char* file_name);
AssemblerErr_t AssemblerDestroy(Assembler* ASM);

AssemblerErr_t TempBufferInit(TempBuffer_t* buffer, size_t capacity);
AssemblerErr_t TempBufferDestroy(TempBuffer_t* buffer);
AssemblerErr_t TempBufferPush(TempBuffer_t* buffer, char value);
AssemblerErr_t TempBufferRelease(TempBuffer_t* buffer);

#endif
