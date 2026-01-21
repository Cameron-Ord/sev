#ifndef IO_H
#define IO_H
#include <stdbool.h>
#include "../typedefs.h"


i32 *read_text_file(const char *const path);
bool write_text_file(const char *const path, const i32 *const buffer, const size_t bsize);

#endif