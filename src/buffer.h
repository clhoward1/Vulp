#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define GAP_SIZE 8
#define LINE_LIMIT 1024

typedef struct {
    char *data;
    size_t gapLeft;
    size_t gapRight;
    size_t totalSize;
    size_t gapSize;
} gapBuffer;


gapBuffer* initBuffer();

//Grows the gap when it gets full
void grow(int newSize, gapBuffer *buffer);

void shiftLeft(int index, gapBuffer *buffer);

void shiftRight(int index, gapBuffer *buffer);

void moveCursor(int index, gapBuffer *buffer);

void insert(char *newChar, int index, gapBuffer *buffer);

void backspace(gapBuffer *buffer);

#endif