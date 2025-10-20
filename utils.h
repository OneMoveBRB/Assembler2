#ifndef UTILS_H
#define UTILS_H

#include "asm_settings.h"
#include "instruction.h"

void Skip_Spaces(Assembler* ASM, size_t* i, size_t* asm_line = NULL);
int SkipWord(Assembler* ASM, size_t* i);

char* GetWord(Assembler* ASM, size_t* i);

AssemblerErr_t StackPushCommand(Assembler* ASM, size_t* i, Instruction* Commands, size_t Commands_size, size_t* command_type);
AssemblerErr_t StackPushLabel(Assembler* ASM, size_t* i, Stack_t* Labels);
// AssemblerErr_t DefineCommand(Assembler* ASM, size_t* i, Instruction* Commands, size_t Commands_size, size_t* command_index);

size_t BinSearch(void* arr, size_t num, size_t element_size, void* value, int (*cmp)(const void*, const void*));

void* move_ptr(void* arr, size_t offset, size_t element_size);
int strcmp_(const void* a, const void* b);
int CommandsCompare(const void* a, const void* b);

#endif
