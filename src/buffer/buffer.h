#ifndef BUFFER_H
#define BUFFER_H
#include "../typedefs.h"
#include <stdbool.h>
#include <stddef.h>

struct buf_inst {
  i32 *data;
  bool valid;
  size_t size;
  u32 cursor;
};

struct buf_init_ret {
  struct buf_inst *buf;
  i32 err;
};

struct buf_init_ret buf_init(size_t size, i32 *memory);
#endif // BUFFER_H
