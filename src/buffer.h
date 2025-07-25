#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#define GAP_SIZE 10

typedef struct {
    char buffer[1024];
    size_t gapLeft;
    size_t gapRight;
    size_t size;
} gapBuffer;


gapBuffer* initBuffer();

//Grows the gap when it gets full
void grow(int newSize, int index, gapBuffer *buffer);

void shiftLeft(int index, gapBuffer *buffer);

void shiftRight(int index, gapBuffer *buffer);

void moveCursor(int index, gapBuffer *buffer);

void insert(char *newChar, int index, gapBuffer *buffer);

#endif