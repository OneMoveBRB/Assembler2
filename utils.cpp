#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "instruction.h"


void SkipSpaces(Assembler* ASM, size_t* i, size_t* asm_line) {
    const char* arr = ASM->InputBuffer.data;
    const size_t size = ASM->InputBuffer.size;

    for (; isspace(arr[*i]) && *i < size; (*i)++) {
        if (arr[*i] == '\n' && asm_line != NULL)
            *asm_line += 1;
    };

    if (arr[*i] == ';') {
        for (; arr[*i] != '\n' && arr[*i] != '\0' && *i < size; (*i)++);
    }

    for (; isspace(arr[*i]) && *i < size; (*i)++) {
        if (arr[*i] == '\n' && asm_line != NULL)
            *asm_line += 1;
    };
}

int SkipWord(Assembler* ASM, size_t* i) {
    const char* arr = ASM->InputBuffer.data;

    size_t start = *i;
        for (; !isspace(arr[*i]) && arr[*i] != '\0'; (*i)++);

    return (*i > start);
}

char* GetWord(Assembler* ASM, size_t* i) {
    const char* arr = ASM->InputBuffer.data;

    Stack_t* string;
    StackInit(&string, sizeof(char), FIRST_SIZE, "string");

    for (; !isspace(arr[*i]) && arr[*i] != '\0'; (*i)++) {
        if (StackPush(string, arr + (*i)) != STACK_OK) {
            StackDestroy(string);
            // DUMP(ASM_GETWORD_FAILED)
            return NULL;
        }
    }

    char* word = NULL;
    if (WordInit(string, &word) != ASM_OK) {
        // DUMP(ASM_GETWORD_FAILED)
        return NULL;
    }

    StackDestroy(string);

    return word;
}

AssemblerErr_t StackPushCommand(Assembler* ASM, size_t* i, Instruction* Commands, size_t Commands_size, size_t* command_type) {
    char* word = GetWord(ASM, i);
    if (word == NULL)
        return ASM_GETWORD_FAILED;

    size_t command_index = BinSearch(Commands, Commands_size, sizeof(Instruction), word, InstrStrCompare);
    if ( command_index == Commands_size ) {
        fprintf(stderr, "STR[%s] Invalid command at line \n", word); // asm_line
        free(word);
        return ASM_INVALID_COMMAND; // DUMP(ASM_INVALID_COMMAND)
    }

    free(word);
    
    if (command_type != NULL) {
        *command_type = Commands[command_index].id;
        StackPush(ASM->OutPutBuffer, command_type);
    } else {
        size_t regs_type = Commands[command_index].id;
        StackPush(ASM->OutPutBuffer, &regs_type);
    }
    
    return ASM_OK;
}

AssemblerErr_t StackPushLabel(Assembler* ASM, size_t* i, Stack_t* Labels) {
    char* word = GetWord(ASM, i);
    if (word == NULL)
        return ASM_GETWORD_FAILED;

    size_t command_index = BinSearch(Labels->data, Labels->meta.size, sizeof(Instruction), word, InstrStrCompare);
    if ( command_index == Labels->meta.size ) {
        fprintf(stderr, "STR[%s] Invalid command at line \n", word); // asm_line
        free(word);
        return ASM_INVALID_COMMAND; // DUMP(ASM_INVALID_COMMAND)
    }

    free(word);

    size_t Label_type = ((Instruction*)MovePtr(Labels->data, command_index, sizeof(Instruction)))->id;
    StackPush(ASM->OutPutBuffer, &Label_type);
    
    return ASM_OK;
}

AssemblerErr_t StackPushInt(Assembler* ASM, size_t* i) {
    char* word = GetWord(ASM, i);
    if (word == NULL)
        return ASM_GETWORD_FAILED;
    
    int int_from_word = atoi(word);
    if (int_from_word == 0 && strcmp(word, "0") != 0) {
        fprintf(stderr, "Invalid number was given with PUSH\n");
        free(word);
        return ASM_INVALID_NUMBER; // DUMP(ASM_INVALID_NUMBER)
    }
    StackPush(ASM->OutPutBuffer, &int_from_word);

    free(word);

    return ASM_OK;
}

size_t BinSearch(void* arr, size_t num, size_t element_size, void* value, int (*cmp)(const void*, const void*)) {
    size_t left = 0;
    size_t right = num-1;

    while ( left <= right ) {
        size_t mid = left + (right - left)/2;
        if (cmp(MovePtr(arr, mid, element_size), value) == 0) {
            return mid;
        } else if (cmp(MovePtr(arr, mid, element_size), value) < 0) {
            left = mid + 1;
        } else {
            if (right == 0)
                return num;
            right = mid - 1;
        }
    }

    return num;
}

void* MovePtr(void* arr, size_t offset, size_t element_size) {
    return (void*)((size_t)arr + offset * element_size);
}

int InstrStrCompare(const void* a, const void* b) {
    const char* str1 = ((const Instruction*) a) -> word;
    const char* str2 = (const char*) b;

    return strcmp(str1, str2);
}

int InstructionsCompare(const void* a, const void* b) {
    Instruction obj1 = *(const Instruction*)a;
    Instruction obj2 = *(const Instruction*)b;

    return (strcmp(obj1.word, obj2.word));
}
