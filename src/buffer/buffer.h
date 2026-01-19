#ifndef BUFFER_H
#define BUFFER_H
#include "../typedefs.h"

typedef struct {
    i32 *buffer;
    size_t size;
    u32 cursor;
} buf_inst;

buf_inst *buf_alloc(u32 rows, u32 cols, size_t bsize);

#endif // BUFFER_H