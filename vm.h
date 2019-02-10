#ifndef clox_vm_h
#define clox_vm_h 

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct 
{
    Chunk* chunk; 
    ValueArray stack; 
    Value* stackTop; 
    uint8_t* ip; 
} Vm;

typedef enum
{
    INTERPRET_OK, 
    INTERPRET_COMPILE_ERROR, 
    INTERPRET_RUNTIME_ERROR
} InterpretResult;


void initVm();
void freeVm(); 
InterpretResult interpret(Chunk* chunk);
void initStack(); 
void freeStack(); 
void push(Value value); 
Value pop();  

#endif

