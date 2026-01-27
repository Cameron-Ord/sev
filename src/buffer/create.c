#include "../errdef.h"
#include "../util.h"
#include "buffer.h"
#include <stdlib.h>

struct buf_init_ret buf_init(size_t size, i32 *memory) {
  struct mem_ret result = {NULL, UNSET};
  const i32 preallocated = memory != NULL;

  result = sev_calloc(1, sizeof(struct buf_inst));
  if (result.err != OK) {
    return (struct buf_init_ret){NULL, result.err};
  }
  struct buf_inst *buf = (struct buf_inst *)result.allocated;

  buf->size = size;
  buf->valid = false;
  // i32 *tmp

  switch (preallocated) {
  case 1: {
    buf->data = memory;
  } break;
  case 0: {
    result = sev_calloc(1, sizeof(i32));
    if (result.err != OK) {
      sev_free(buf);
      return (struct buf_init_ret){NULL, result.err};
    }
    buf->data = (i32 *)result.allocated;
  } break;
  }

  buf->valid = true;
  return (struct buf_init_ret){buf, OK};
}
