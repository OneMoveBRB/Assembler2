#include "read_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
 
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
 
int64_t GetFileSize(const char* file_name){
	int64_t file_size = 0;
	struct stat fileStatbuff;
	if ((stat(file_name, &fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		file_size = -1;
	} else {
		file_size = fileStatbuff.st_size;
	}
	return file_size;
}


size_t ReadFile(char* buffer, size_t file_size, const char* file_name) {
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "Opening %s [%s]: fp_from = NULL\n", file_name, __func__);
        return 0;
    }

    size_t num_of_read_elements = fread(buffer, sizeof(char), file_size, fp);
    printf("Read %zu elements out of %zu\n", num_of_read_elements,  file_size);

    buffer[num_of_read_elements] = 0;

    fclose(fp);

    return num_of_read_elements;
}
