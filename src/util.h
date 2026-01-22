#ifndef UTIL_H
#define UTIL_H
#include "typedefs.h"
typedef struct SDL_Texture SDL_Texture;

struct mem_ret {
    void* allocated;
    u32 err;
};
typedef struct mem_ret mem_ret;

mem_ret sev_malloc(u32 size);
mem_ret sev_calloc(u32 nmemb, u32 size);
mem_ret sev_realloc(void **original, u32 size);

void sev_free(void *ptr);
void sdl_texture_free(SDL_Texture *ptr);
u32 log2u32(u32 val);

ptrdiff_t make_utf8str(const i32 codepoint, char *dst);
ptrdiff_t make_codepoint(const char *utf8str, i32 *dst);
#endif // UTIL_H