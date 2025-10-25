#ifndef WRITE_FILE_H
#define WRITE_FILE_H

#include "asm_settings.h"

size_t WriteFile(const int* buffer, size_t buffer_size, size_t StartIP, const char* file_name);

#endif
