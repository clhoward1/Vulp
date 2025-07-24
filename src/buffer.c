#include <stdlib.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "buffer.h"

gapBuffer *buffer;

void init() {

    buffer = malloc(sizeof(*buffer));

    buffer->gapSize = GAP_SIZE;
    buffer->gapLeft = 0;
    buffer->gapRight = GAP_SIZE - 1;
    buffer->size = GAP_SIZE;
}

size_t checkGapSize() {

    return buffer->gapRight - buffer->gapLeft; 

}

void shiftLeft(int index) {

    if (buffer->gapLeft > 0 && index >= 0) {
        buffer->gapLeft = index;
        buffer->gapRight = buffer->gapLeft + GAP_SIZE - 1;
    } else {
        SDL_Log("Cannot shift left any further");
    }
}

void shiftRight(int index) {

    if (buffer->gapLeft < sizeof(buffer->buffer)) {
        buffer->gapLeft = index;
        buffer->gapRight = buffer->gapLeft + GAP_SIZE - 1;
    } else {
        SDL_Log("Cannot shift right any further");
    }
}

void grow(int newSize, int index) {

    char a[buffer->size];

    // Copy chars from buffer to temp string
    for (int i = index; i < buffer->size; i++) {
        a[i - index] = buffer->buffer[i];
    }

    // Insert new gap from index
    for (int i = 0; i < newSize; i++) {
        buffer->buffer[i + index] = "-";
    }

    for (int i = 0; i < index + newSize; i++) {
        buffer->buffer[index + newSize + i] = a[i];
    }

    buffer->size += newSize;
    buffer->gapRight += newSize;

}

void moveCursor(int index) {
    
    if (index < buffer->gapLeft) {
        shiftLeft(index);
    } else {
        shiftRight(index);
    }
}

void insert(char newChar, int index) {

    // Move cursor to index if not matching
    if (index != buffer->gapLeft) {
        moveCursor(index);
    }

    // Grow gap if empty
    if (buffer->gapLeft == buffer->gapRight) {
        grow(GAP_SIZE, index);
    }

    buffer->buffer[buffer->gapLeft] = newChar;

    buffer->gapLeft++;

}
