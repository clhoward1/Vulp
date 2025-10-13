#ifndef CURSOR_H
#define CURSOR_H

#include <stddef.h>

typedef struct {

    size_t line;
    size_t offset;

} cursor;

cursor* initCursor();

#endif