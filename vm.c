#include <stdio.h> 
#include "common.h"
#include "vm.h"
#include "value.h"
#include "debug.h"
#include "memory.h"

Vm vm; 

void resetStack() {
    vm.stackTop = vm.stack->values;
}

void initStack() {
    vm.stack.capacity = 0;
    vm.stack.count = 0; 
    vm.stack->values = NULL; 
}

void freeStack() {
    FREE_ARRAY(Value, vm.stack->values, vm.stack.capacity);
    initStack(); 
}

void initVm() {
    resetStack(); 
}

void freeVm() {

}

void push(Value value) {
    if(vm.stack.capacity > vm.stack.counter + 1){
        int capacity =  GROW_CAPACITY(vm.count); 
        vm.stack.capacity = capacity; 
        GROW_ARRAY(vm.stack->values, Value, vm.stack.count, capacity); 
    }
    *vm.stackTop = value; 
    ++vm.stackTop; 
    ++vm.stack.count; 
}

Value pop() {
    --vm.stackTop; 
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
            for(Value* slot = vm.stack; slot < vm.stackTop; slot++){
                printf("[ ");  
                printValue(*slot); 
                printf(" ]"); 
            }
            printf("\n"); 
            disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code)); 
        #endif 

        uint8_t instruction; 
        switch ( instruction = READ_BYTE())
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
    vm.chunk = chunk; 
    vm.ip = vm.chunk->code; 
    return run(); 
}
