#include <stdlib.h> 
#include <stdio.h>

#include "chunk.h"
#include "common.h"
#include "memory.h"

void initChunk(Chunk* chunk){
    chunk->capacity = 0;  
    chunk->count = 0; 
    initValueArray(&chunk->constants); 
    chunk->lines = NULL;
    chunk->code = NULL; 
}

void freeChunk(Chunk* chunk){
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity); 
    FREE_ARRAY(int, chunk->lines, chunk->capacity); 
    freeValueArray(&chunk->constants); 
    initChunk(chunk); 
}

void writeChunk(Chunk* chunk, uint8_t byte, int line){

    int oldCapacity = chunk->capacity; 

    if(oldCapacity < chunk->count + 1){
        chunk->capacity = GROW_CAPACITY(oldCapacity); 
        chunk->code = GROW_ARRAY(chunk->code, uint8_t, oldCapacity, chunk->capacity); 
        chunk->lines = GROW_ARRAY(chunk->lines, int, oldCapacity, chunk->capacity); 
    }

    chunk->code[chunk->count] = byte; 
    chunk->lines[chunk->count] = line; 

    ++chunk->count; 
}

void writeConstant(Chunk* chunk, Value value, int line){
    int constant = addConstant(chunk, value); 
    if(constant < 256){
        writeChunk(chunk, OP_CONSTANT, line); 
        writeChunk(chunk, constant, line); 
    } else if (constant < 16777216  && constant>256 ){
        writeChunk(chunk, OP_CONSTANT_LONG, line); 
        writeChunk(chunk, constant - 256, line); 
    } else {
        printf("The constant pool of this chunk reached its limit."); 
    }
}

int addConstant(Chunk* chunk, Value value){
    writeValueArray(&chunk->constants, value); 
    return chunk->constants.count-1; 
}