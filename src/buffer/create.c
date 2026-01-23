#include "buffer.h"
#include "../util.h"
#include "../errdef.h"
#include <stdlib.h>

struct buf_init_ret buf_init(u32 rows, u32 cols){
    struct mem_ret result = { NULL, UNSET };

    result = sev_calloc(1, sizeof(struct buf_inst));
    if(result.err != OK) {
        return (struct buf_init_ret){ NULL, result.err };
    }
    struct buf_inst *buf = (struct buf_inst *)result.allocated;

    buf->size = rows * cols;
    buf->valid = false;
    //i32 *tmp 
    result = sev_calloc(rows * cols, rows * cols * sizeof(i32));
    if(result.err != OK){
        sev_free(buf);
        return (struct buf_init_ret){ NULL, result.err };
    }
    i32 *tmp = (i32 *)result.allocated;

    buf->buffer = tmp;
    buf->valid = true;
    
    return (struct buf_init_ret){ NULL, OK };
}
