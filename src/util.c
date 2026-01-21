#include "util.h"
#include <stdlib.h>
#include <SDL3/SDL_render.h>
#include <utf8proc.h>

void sev_free(void *ptr){
    if(!ptr) return;
    free(ptr);
    ptr = NULL;
}

void sdl_texture_free(SDL_Texture *ptr){
    if(!ptr) return;
    SDL_DestroyTexture(ptr);
    ptr = NULL;
}

u32 log2u32(u32 val){
    if(val == 0){
        return UINT32_MAX;
    }

    if(val == 1){
        return 0;
    }

    u32 ret = 0;
    while(val > 1){
        val >>= 1;
        ret++;
    }

    return ret;
}

ptrdiff_t make_codepoint(const char *utf8str, int32_t *dst){
  if(!utf8str || !dst)
    return -1;

  const utf8proc_uint8_t *str8 = (const utf8proc_uint8_t *)utf8str;
  return utf8proc_iterate(str8, -1, dst);
}

ptrdiff_t make_utf8str(const int32_t codepoint, char *dst){
  if(!dst)
    return -1;

  utf8proc_uint8_t *str8 = (utf8proc_uint8_t *)dst;
  return utf8proc_encode_char(codepoint, str8);
}
