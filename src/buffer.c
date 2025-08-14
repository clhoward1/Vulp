#include <stdlib.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include "buffer.h"


gapBuffer* initBuffer() {

    gapBuffer *buffer = malloc(sizeof(*buffer));

    buffer->gapLeft = 0;
    buffer->gapRight = GAP_SIZE - 1;
    buffer->totalSize = GAP_SIZE;
    buffer->gapSize = GAP_SIZE;
    buffer->data = (char*)calloc(buffer->totalSize, 1);

    return buffer;
}

void shiftLeft(int index, gapBuffer *buffer) {

    while (index < buffer->gapLeft && index >= 0) {
        
        /*
        printf("\ngapRight: %d", buffer->gapRight);
        printf("\ngapLeft: %d", buffer->gapLeft);
        printf("\n strlen: %d", strlen(buffer->data));
        */
        
        buffer->gapLeft--;
        
        buffer->data[buffer->gapRight] = buffer->data[buffer->gapLeft];
        
        buffer->gapRight--;
        
        /*
        printf("\n");
        for (int i = 0; i < strlen(buffer->data); i++) {
            printf("%c", buffer->data[i]);
        }

        printf("\nIndex location: %d", index);
        printf("\nCursor location: %d", buffer->gapLeft);
        */
        
    } 
}


void shiftRight(int index, gapBuffer *buffer) {

    while (index > buffer->gapLeft && buffer->gapRight + 1 < buffer->totalSize) { // this is wrong but idk how
        
        buffer->data[buffer->gapLeft] = buffer->data[buffer->gapRight + 1];
        
        buffer->gapLeft++;
        buffer->gapRight++;
        
        /*
        printf("\n");
        for (int i = 0; i < strlen(buffer->data); i++) {
            printf("%c", buffer->data[i]);
        }
        printf("\nIndex location: %d", index);
        printf("\nCursor location: %d", buffer->gapLeft);
        */
    } 
}

void grow(int newSize, gapBuffer *buffer) {
    size_t back = buffer->totalSize - buffer->gapLeft;
    size_t oldSize = buffer->totalSize;
    printf("\n Add Gap: %d", GAP_SIZE);

    buffer->data = realloc(buffer->data, newSize);

    if (!buffer->data) {
        printf("\nFailed to realloc buffer somehow");
        return;
    }

    memmove(buffer->data + buffer->gapLeft + GAP_SIZE, buffer->data + buffer->gapLeft, back);
    memset(buffer->data + buffer->gapLeft, 0, GAP_SIZE);

    /*
    printf("\ngrown thingy\n");
    for (int i = 0; i < buffer->totalSize; i++) {
        printf("%c", buffer->data[i]);
    }
    */
    
    buffer->gapRight += GAP_SIZE;
    buffer->gapSize += GAP_SIZE;
    buffer->totalSize = newSize;
    
    /*
    printf("\nNew Buffer totalSize: %d", buffer->totalSize);
    printf("\nGap Left: %d", buffer->gapLeft);
    printf("\nGap Right: %d", buffer->gapRight);
    */
}

void moveCursor(int index, gapBuffer *buffer) {
    
    if (index < buffer->gapLeft) {
        shiftLeft(index, buffer);
    } else {
        shiftRight(index, buffer);
    }
}

void insert(char *newChar, int index, gapBuffer *buffer) {
    
    if (index != buffer->gapLeft) {
        moveCursor(index, buffer);
    }

    buffer->data[buffer->gapLeft] = (char)*newChar;
    buffer->gapLeft++;
    buffer->gapSize--;
    
    // Grow gap if empty
    if (!buffer->gapSize) {
        grow(buffer->totalSize + GAP_SIZE, buffer);
    }

    /*
    printf("\nGap Size: %d", buffer->gapSize);
    printf("\nData: %c", buffer->data[buffer->gapLeft - 1]);
    printf("\nData Len: %d", strlen(buffer->data));
    printf("\nTotal Len: %d", buffer->totalSize);
    */
}

void backspace(gapBuffer *buffer) {

    buffer->data[buffer->gapLeft] = '\0';
    buffer->gapLeft--;
    buffer->gapSize++;
    
}
