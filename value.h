#ifndef clox_value_h
#define clox_value_h

typedef double Value ; 

typedef struct {
    int capacity; 
    int count; 
    Value* values; 
} ValueArray; 

void initValueArray(ValueArray *array); 
void writeValueArray(ValueArray* , Value value); 
void freeValueArray(ValueArray* array); 
void printValue(Value value); 

#endif