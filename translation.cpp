#include "translation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "asm_alloc.h"
#include "instruction.h"
#include "utils.h"

static AssemblerErr_t FirstIteration(Assembler* ASM, Stack_t* Labels);
static AssemblerErr_t SecondIteration(Assembler* ASM, Stack_t* Labels);

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


AssemblerErr_t Translation(Assembler* ASM) {
    qsort(Commands, Commands_size, sizeof(Instruction), InstructionsCompare);

    Stack_t* Labels;
    StackInit(&Labels, sizeof(Instruction), FIRST_SIZE, "Labels");

    FirstIteration(ASM, Labels);

    qsort(Labels->data, Labels->meta.size, Labels->meta.element_size, InstructionsCompare);

    SecondIteration(ASM, Labels);

    WordDestroy(Labels);
    StackDestroy(Labels);

    return ASM_OK;
}

static AssemblerErr_t FirstIteration(Assembler* ASM, Stack_t* Labels) {
    char* arr = ASM->InputBuffer.data;

    size_t num_of_commands = 0;

    for (size_t i = 0; arr[i] != '\0'; ) {
        SkipSpaces(ASM, &i);

        if (arr[i] == ':') {
            size_t start = i;
            ++i; SkipSpaces(ASM, &i);

            char* word = GetWord(ASM, &i);
            if (word == NULL)
                return ASM_GETWORD_FAILED;

            Instruction label = {word, num_of_commands};
            StackPush(Labels, &label);

            for (size_t j = start; j < i; j++) {
                arr[j] = ' ';
            }
        }

        SkipSpaces(ASM, &i);

        if (SkipWord(ASM, &i))
            ++num_of_commands;

    }
    return ASM_OK;
}

static AssemblerErr_t SecondIteration(Assembler* ASM, Stack_t* Labels) {
    char* input_buffer = ASM->InputBuffer.data;
    
    size_t asm_line = 1;

    for (size_t i = 0; input_buffer[i] != '\0'; ) {
        SkipSpaces(ASM, &i, &asm_line);

        size_t command_type = 0;
        StackPushCommand(ASM, &i, Commands, Commands_size, &command_type);

        SkipSpaces(ASM, &i, &asm_line);

        if (command_type == PUSH) {

            StackPushInt(ASM, &i);

        } else if (command_type == PUSHR || command_type == POPR) {

            StackPushCommand(ASM, &i, Commands, Commands_size, NULL);

        } else if (command_type == JA || command_type == JAE ||
                   command_type == JB || command_type == JBE ||
                   command_type == JE || command_type == JNE ||
                   command_type == JMP) {

            StackPushLabel(ASM, &i, Labels);

        }

        SkipSpaces(ASM, &i, &asm_line);
    }

    return ASM_OK;
}
