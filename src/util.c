#include "util.h"
#include "errdef.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_render.h>
#include <utf8proc.h>

mem_ret sev_realloc(void **original, u32 size){
    if(!original || !*original || size < 1) return (mem_ret){ NULL, BAD_PARAM };

    void *allocated = realloc(*original, size);
    if(!allocated){
        printf("Memory allocation failed!\n");
        if(errno == ENOMEM){
            printf("%s\n", strerror(errno));
            return (mem_ret){ NULL, NO_MEM };
        }
        return (mem_ret){ NULL, UNSPECIFIED };
    }
    return (mem_ret){ allocated, OK };
}

mem_ret sev_calloc(u32 nmemb, u32 size){
    if(nmemb < 1 || size < 1) return (mem_ret){ NULL, BAD_PARAM };

    void *allocated = calloc(nmemb, size);
    if(!allocated){
        printf("Memory allocation failed!\n");
        if(errno == ENOMEM){
            printf("%s\n", strerror(errno));
            return (mem_ret){ NULL, NO_MEM };
        }
        return (mem_ret){ NULL, UNSPECIFIED };
    }
    return (mem_ret){ allocated, OK};
}

mem_ret sev_malloc(u32 size){
    if(size < 1) return (mem_ret){ NULL, BAD_PARAM };

    void *allocated = malloc(size);
    if(!allocated){
        printf("Memory allocation failed!\n");
        if(errno == ENOMEM){
            printf("%s\n", strerror(errno));
            return (mem_ret){ NULL, NO_MEM };
        }
        return (mem_ret){ NULL, UNSPECIFIED };
    }
    return (mem_ret){ allocated, OK};
}

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
