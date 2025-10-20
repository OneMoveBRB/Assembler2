#include "translation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "asm_alloc.h"
#include "instruction.h"
#include "utils.h"

Instruction Commands[] = {
    {"IN"   ,	IN   },
    {"PUSH" ,	PUSH },
    {"POP"  ,	POP  },
    {"PUSHR",	PUSHR},
    {"POPR" ,	POPR },
    {"ADD"  ,	ADD  },
    {"SUB"  ,	SUB  },
    {"MUL"  ,	MUL  },
    {"OUT"  ,	OUT  },
    {"HLT"  ,	HLT  },
    {"RAX"  ,   RAX  },
    {"RBX"  ,   RBX  },
    {"RCX"  ,   RCX  },
    {"JA"   ,   JA   },
    {"JAE"  ,   JAE  },
    {"JB"   ,   JB   },
    {"JBE"  ,   JBE  },
    {"JE"   ,   JE   },
    {"JNE"  ,   JNE  },
    {"JMP"  ,   JMP  }
};

size_t Commands_size = sizeof(Commands)/sizeof(Commands[0]);

AssemblerErr_t FirstIteration(Assembler* ASM, Stack_t* Labels) {
    char* arr = ASM->InputBuffer.data;

    size_t num_of_commands = 0;

    for (size_t i = 0; arr[i] != '\0'; ) {
        Skip_Spaces(ASM, &i);

        if (arr[i] == ':') {
            size_t start = i;
            ++i; Skip_Spaces(ASM, &i);

            char* word = GetWord(ASM, &i);
            if (word == NULL)
                return ASM_GETWORD_FAILED;

            Instruction label = {word, num_of_commands};
            StackPush(Labels, &label);

            for (size_t j = start; j < i; j++) {
                arr[j] = ' ';
            }
        }

        Skip_Spaces(ASM, &i);

        if (SkipWord(ASM, &i))
            ++num_of_commands;

    }
    return ASM_OK;
}

AssemblerErr_t SecondIteration(Assembler* ASM, Stack_t* Labels) {
    char* input_buffer = ASM->InputBuffer.data;
    
    size_t asm_line = 1;

    for (size_t i = 0; input_buffer[i] != '\0'; ) {
        Skip_Spaces(ASM, &i, &asm_line);

        size_t command_type = 0;
        StackPushCommand(ASM, &i, Commands, Commands_size, &command_type);

        Skip_Spaces(ASM, &i, &asm_line);

        if (command_type == PUSH) {
            char* word = GetWord(ASM, &i);
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
        } else if (command_type == PUSHR || command_type == POPR) {
            StackPushCommand(ASM, &i, Commands, Commands_size, NULL);
        } //else if (command_type == JA)

        Skip_Spaces(ASM, &i, &asm_line);
    }

    return ASM_OK;
}

AssemblerErr_t Translation(Assembler* ASM) {
    qsort(Commands, Commands_size, sizeof(Instruction), CommandsCompare);

    Stack_t* Labels;
    StackInit(&Labels, sizeof(Instruction), FIRST_SIZE, "Labels");

    FirstIteration(ASM, Labels);

    SecondIteration(ASM, Labels);

    for (int i=0; i<ASM->OutPutBuffer->meta.size; i++) {
        printf("%d ", *(int*)move_ptr(ASM->OutPutBuffer->data, i, sizeof(int)));
    } printf("\n");

    WordDestroy(Labels);
    StackDestroy(Labels);

    return ASM_OK;
}
