#ifndef READ_FILE_H
#define READ_FILE_H

#include "asm_settings.h"

int64_t GetFileSize(const char* file_name);
size_t ReadFile(char* buffer, size_t file_size, const char* file_name);

#endif
