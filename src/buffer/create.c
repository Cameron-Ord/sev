#include "buffer.h"
#include <stdlib.h>

i32 *buf_alloc(u32 rows, u32 cols, size_t bsize){
    i32 *tmp = calloc(rows * cols, bsize);
    if(!tmp){
        return NULL;
    }
    return tmp;
}
