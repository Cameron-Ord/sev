#ifndef IO_H
#define IO_H
#include <stdbool.h>
#include "../typedefs.h"

struct read_ret {
    char *data;
    size_t pread;
    i32 err;
};

struct file_ret {
    char *data;
    size_t size;
    i32 err;
};

struct file_ret read_text_file(const char *const path);
bool write_text_file(const char *const path, const i32 *const buffer, const size_t bsize);

#endif