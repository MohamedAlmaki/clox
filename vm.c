#include <stdio.h> 
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "vm.h"
#include "value.h"
#include "debug.h"
#include "memory.h"

Vm vm; 

void resetStack() {
    vm.stackTop = vm.stack.values;
}

void initStack() {
    vm.stack.capacity = 8;
    vm.stack.count = 0; 
    vm.stack.values = GROW_ARRAY(vm.stack.values, Value, 0, vm.stack.capacity); 
    resetStack(); 
}

void freeStack() {
    FREE_ARRAY(Value, vm.stack.values, vm.stack.capacity); 
}

void initVm() {
    initStack(); 
}

void freeVm() {

}

void push(Value value) {
    int oldCapacity = vm.stack.capacity; 
    if(oldCapacity < vm.stack.count + 2){ 
        int capacity =  GROW_CAPACITY(oldCapacity); 
        vm.stack.capacity = capacity; 
        GROW_ARRAY(vm.stack.values, Value, oldCapacity, capacity); 
    }
   
    // *(vm.stackTop) = value; 
    vm.stack.values[vm.stack.count] = value;
    printf("*****");
    vm.stackTop = vm.stack.values + vm.stack.count; 
    ++vm.stackTop; 
    ++vm.stack.count; 
}

Value pop() {
    --vm.stackTop; 
    --vm.stack.count; 
    return *vm.stackTop; 
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    #define BINARY_OP(op)\
        do{ \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
        } while(false)

    for(;;){
        #ifdef DEBUG_TRACE_EXECUTION
            printf("                "); 
            for(Value* slot = vm.stack.values; slot < vm.stackTop; slot++){
                printf("[ ");  
                printValue(*slot); 
                printf(" ]"); 
            }
            printf("\n"); 
            disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code)); 
        #endif 

        uint8_t instruction; 
        switch (instruction = READ_BYTE())
        {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT(); 
                push(constant);  
                break; 
            }
            case OP_ADD:      BINARY_OP(+); break; 
            case OP_SUBTRACT: BINARY_OP(-); break; 
            case OP_MULTIPLY: BINARY_OP(*); break; 
            case OP_DIVIDE:   BINARY_OP(/); break; 
            case OP_NEGATE: push(-pop()); break; 
            case OP_RETURN:{
                printValue(pop()); 
                printf("\n"); 
                return INTERPRET_OK; 
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
    compile(source); 
    return INTERPRET_OK; 
}
