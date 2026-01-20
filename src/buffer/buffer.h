#ifndef BUFFER_H
#define BUFFER_H
#include "../typedefs.h"
#include <stdbool.h>

typedef struct {
    i32 *buffer;
    bool valid;
    size_t size;
    u32 cursor;
} buf_inst;

buf_inst *buf_alloc(u32 rows, u32 cols);
//
// 556840723
#endif // BUFFER_H