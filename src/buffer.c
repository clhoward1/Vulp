#include <stdlib.h>
#include <SDL3/SDL.h>
#include "buffer.h"


gapBuffer* initBuffer() {

    gapBuffer *buffer = malloc(sizeof(*buffer));

    buffer->gapLeft = 0;
    buffer->gapRight = GAP_SIZE - 1;
    buffer->size = GAP_SIZE;
    memset(buffer->buffer, 0, sizeof(buffer->buffer));

    return buffer;
}

void shiftLeft(int index, gapBuffer *buffer) {

    while (index < buffer->gapLeft) {
        buffer->gapLeft--;
        buffer->gapRight--;
        buffer->buffer[buffer->gapRight + 1] = buffer->buffer[buffer->gapLeft];
        buffer->buffer[buffer->gapLeft] = '\0';
    } 
}

void shiftRight(int index, gapBuffer *buffer) {

    while (index > buffer->gapLeft) {
        buffer->gapLeft++;
        buffer->gapRight++;
        buffer->buffer[buffer->gapLeft - 1] = buffer->buffer[buffer->gapRight];
        buffer->buffer[buffer->gapRight] = '\0';
    } 
}

void grow(int newSize, int index, gapBuffer *buffer) {

    char a[buffer->size];

    // Copy chars from buffer to temp string
    for (int i = index; i < buffer->size; i++) {
        a[i - index] = buffer->buffer[i];
    }

    // Insert new gap from index
    for (int i = 0; i < newSize; i++) {
        buffer->buffer[i + index] = '\0';
    }

    for (int i = 0; i < index + newSize; i++) {
        buffer->buffer[index + newSize + i] = a[i];
    }

    buffer->size += newSize;
    buffer->gapRight += newSize;

}

void moveCursor(int index, gapBuffer *buffer) {
    
    if (index < buffer->gapLeft) {
        shiftLeft(index, buffer);
    } else {
        shiftRight(index, buffer);
    }
}

void insert(char *newChar, int index, gapBuffer *buffer) {

    // Move cursor to index if not matching
    if (index != buffer->gapLeft) {
        moveCursor(index, buffer);
    }

    // Grow gap if empty
    if (buffer->gapLeft == buffer->gapRight) {
        grow(GAP_SIZE, index, buffer);
    }

    buffer->buffer[buffer->gapLeft] = *newChar;

    buffer->gapLeft++;

}
