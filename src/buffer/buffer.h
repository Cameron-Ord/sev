#ifndef BUFFER_H
#define BUFFER_H
#include "../typedefs.h"

typedef struct {
    i32 *buffer;
    size_t size;
    i32 cursor;
} buf_inst;

#endif // BUFFER_H