#ifndef BUFFER_H
#define BUFFER_H

#define GAP_SIZE 10

typedef struct {
    char *buffer[50];
    size_t gapSize;
    size_t gapLeft;
    size_t gapRight;
    size_t size;
} gapBuffer;


void init();

size_t checkGapSize();

//Grows the gap when it gets full
void grow(int newSize, int index);

void shiftLeft(int index);

void shiftRight(int index);

void moveCursor(int index);

void insert(char newChar, int index);

#endif