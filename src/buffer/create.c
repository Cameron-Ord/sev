#include "buffer.h"
#include <stdlib.h>

buf_inst *buf_alloc(u32 rows, u32 cols){
    buf_inst *buf = calloc(1, sizeof(buf_inst));
    if(!buf) {
        return NULL;
    }

    buf->size = rows * cols;
    buf->valid = false;

    i32 *tmp = calloc(rows * cols, rows * cols * sizeof(i32));
    if(!tmp){
        return buf;
    }

    buf->buffer = tmp;
    buf->valid = true;
    return buf;
}
