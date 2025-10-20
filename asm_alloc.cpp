#include "asm_alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ReadFile.h"


AssemblerErr_t AssemblerInit(Assembler* ASM, const char* file_name) {
    assert( ASM != NULL );
    assert( file_name != NULL );

    int64_t file_size = GetFileSize(file_name);
    if (file_size == -1) {
        fprintf(stderr, "Getting file size of %s failed!\n", file_name);
        return ASM_READING_ERROR;
    }

    TempBufferInit(&ASM->InputBuffer, (size_t)file_size + 1);

    size_t buffer_size = ReadFile(ASM->InputBuffer.data, (size_t)file_size, file_name);
    if (buffer_size < ASM->InputBuffer.capacity - 1) {
        fprintf(stderr, "Not all possible bytes were read!\n");
        return ASM_READING_ERROR;
    }

    ASM->InputBuffer.data[ASM->InputBuffer.capacity - 1] = 0;
    ASM->InputBuffer.size = ASM->InputBuffer.capacity;

    StackInit(&ASM->OutPutBuffer, sizeof(int), FIRST_SIZE, "OutPutBuffer");

    return ASM_OK;
}

AssemblerErr_t AssemblerDestroy(Assembler* ASM) {
    assert( ASM != NULL );

    TempBufferDestroy(&ASM->InputBuffer);

    StackDestroy(ASM->OutPutBuffer);

    return ASM_OK;
}

AssemblerErr_t TempBufferInit(TempBuffer_t* buffer, size_t capacity) {
    assert( buffer != NULL );
    assert( 1 <= capacity && capacity <= FIRST_SIZE*1024 );

    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->data = (char*)calloc(capacity, sizeof(char));

    if (buffer->data == NULL) {
        fprintf(stderr, "Memory allocation error [%s]: buffer.data = NULL\n", __func__);
        return ASM_OVERFLOW;
    }
    return ASM_OK;
}

AssemblerErr_t TempBufferDestroy(TempBuffer_t* buffer) {
    assert( buffer != NULL );

    buffer->size = 0;
    buffer->capacity = 0;

    free(buffer->data);

    return ASM_OK;
}

AssemblerErr_t TempBufferPush(TempBuffer_t* buffer, char value) {
    assert( buffer != NULL );

    const size_t exp_multiplier = 2;

    if (buffer->size + 1 == buffer->capacity) {
        buffer->capacity *= exp_multiplier;
        buffer->data = (char*)realloc(buffer->data, buffer->capacity * sizeof(char));

        if (buffer->data == NULL) {
            fprintf(stderr, "Memory allocation error [%s]: buffer.data = NULL\n", __func__);
            return ASM_OVERFLOW;
        }

        memset(buffer->data + buffer->size, '\0', buffer->capacity - buffer->size);
    }

    buffer->data[buffer->size++] = value;

    return ASM_OK;
}

AssemblerErr_t TempBufferRelease(TempBuffer_t* buffer) {
    assert( buffer != NULL );

    buffer->data = (char*)realloc(buffer->data, (buffer->size + 1) * sizeof(char));
    buffer->data[buffer->size] = '\0';

    return ASM_OK;
}