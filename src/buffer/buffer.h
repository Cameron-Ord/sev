#ifndef BUFFER_H
#define BUFFER_H
#include "../typedefs.h"
#include <stdbool.h>

struct buf_inst {
    i32 *buffer;
    bool valid;
    size_t size;
    u32 cursor;
};

struct buf_init_ret {
    struct buf_inst *buf;
    i32 err;
};

struct buf_init_ret buf_init(u32 rows, u32 cols);
#endif // BUFFER_H