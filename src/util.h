#ifndef UTIL_H
#define UTIL_H
#include "typedefs.h"
#include <stddef.h>
#include <stdint.h>
typedef struct SDL_Texture SDL_Texture;

struct mem_ret {
  void *allocated;
  i32 err;
};

struct codepoint_fill_ret {
  ptrdiff_t consumed;
  ptrdiff_t jpos;
  ptrdiff_t ipos;
  i32 err;
};

struct codepoint_ret {
  i32 *buffer;
  size_t size;
  i32 err;
};

struct codepoint_fill_ret codepoint_buffer_fill(const char *const bytestr,
                                                i32 *const buffer,
                                                const ptrdiff_t pread);
struct codepoint_ret bytestr_to_codepoints(char *const bytestr,
                                           const size_t len);

struct mem_ret sev_malloc(size_t size);
struct mem_ret sev_calloc(size_t nmemb, size_t size);
struct mem_ret sev_realloc(void **original, size_t size);

void sev_free(void *ptr);
void sdl_texture_free(SDL_Texture *ptr);
u32 log2u32(u32 val);

ptrdiff_t make_utf8str(const i32 codepoint, char *dst);
ptrdiff_t make_codepoint(const char *utf8str, i32 *dst, ptrdiff_t len);
#endif // UTIL_H
