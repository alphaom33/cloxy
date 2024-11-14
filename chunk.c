#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->lineCapacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  if (chunk->lines == NULL) {
    chunk->lineCount = 0; 
    chunk->lineCapacity = GROW_CAPACITY(0);
    chunk->lines = NULL;
    chunk->lines = GROW_ARRAY(int, chunk->lines, 0, chunk->lineCapacity);
    chunk->lines[1] = line;
  }

  if (chunk->lines[chunk->lineCount + 1] != line) {
    if (chunk->lineCapacity < chunk->lineCount + 2) {
      int oldCapacity = chunk->lineCapacity;
      chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
      chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->lineCapacity);
    }
    
    chunk->lineCount += 2;
    chunk->lines[chunk->lineCount + 1] = line;
  }
  
  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->lineCount]++;
  chunk->count++;

  // printf("%d\n", line);
  // for (int i = 0; i < chunk->lineCount; i += 2) {
  //   printf("%d: %d\n", chunk->lines[i + 1], chunk->lines[i]);
  // }
  // printf("\n\n");
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}