#include <stdlib.h>
#include "cursor.h"


cursor* initCursor() {

    cursor *cursor = malloc(sizeof(*cursor));

    cursor->line = 0;
    cursor->offset = 0;

    return cursor;
}