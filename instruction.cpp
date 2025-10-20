#include "instruction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


AssemblerErr_t WordInit(Stack_t* string, char** word) {
    *word = (char*)calloc(string->meta.size + 1, sizeof(char));
    if (*word == NULL) {
        fprintf(stderr, "Memory allocation error [%s]: *word = NULL\n", __func__);
        return ASM_GETWORD_FAILED;
    }

    memcpy(*word, string->data, string->meta.size);
    (*word)[string->meta.size] = '\0';

    return ASM_OK;
}

AssemblerErr_t WordDestroy(Stack_t* Labels) {
    for (size_t i = 0; i < Labels->meta.size; i++) {
        free((void*)(((Instruction*)((char*)Labels->data + i * Labels->meta.element_size)) -> word));
    }

    return ASM_OK;
}