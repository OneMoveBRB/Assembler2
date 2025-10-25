#include "write_file.h"

#include <stdio.h>

size_t WriteFile(const int* buffer, size_t buffer_size, size_t StartIP, const char* file_name) {
    FILE* fp = fopen(file_name, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Opening %s [%s]: fp_from = NULL\n", file_name, __func__);
        return 0;
    }

    if (fwrite(&StartIP, sizeof(size_t), 1, fp) != 1) {
        fprintf(stderr, "Writing StartIP FAILED\n");
        return 0;
    }

    printf("Wrote StartIP\n");

    if (fwrite(&buffer_size, sizeof(size_t), 1, fp) != 1) {
        fprintf(stderr, "Writing OutputBuffer_size FAILED\n");
        return 0;
    }

    printf("Wrote OutputBuffer_size\n");

    size_t num_of_wrote_elements = fwrite(buffer, sizeof(int), buffer_size, fp);
    printf("Wrote OutputBuffer: %zu elements out of %zu\n", num_of_wrote_elements,  buffer_size);

    fclose(fp);

    return num_of_wrote_elements;
}
