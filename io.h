#ifndef IO_H
#define IO_H

#include "asm_settings.h"

AssemblerErr_t Read_File_2_Buffer(Assembler* ASM, const char* file_name, const size_t file_size);
AssemblerErr_t Write_Buffer_2_File(Assembler* ASM, const char* file_name);

#endif
