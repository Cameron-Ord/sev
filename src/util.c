#include "util.h"
#include "errdef.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_render.h>
#include <utf8proc.h>

mem_ret sev_realloc(void **original, size_t size){
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

mem_ret sev_calloc(size_t nmemb, size_t size){
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

mem_ret sev_malloc(size_t size){
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

codepoint_ret bytestr_to_codepoints(char *const bytestr, const size_t len){
    if(!bytestr || len < 1) return (codepoint_ret){ NULL, 0, BAD_PARAM };
   
    // Allocate the codepoint buffer (used for editing and rendering)
    mem_ret alloc_result = sev_calloc(len, sizeof(i32));
    if(alloc_result.err != OK){
        sev_free(bytestr);
        return (codepoint_ret){ NULL, 0, alloc_result.err };
    }
    i32 *buffer = (i32 *)alloc_result.allocated; 

    codepoint_fill_ret result = { 0, 0, 0, UNSET };
    const ptrdiff_t pdif_pread = (ptrdiff_t)len;

    result = codepoint_buffer_fill(bytestr, buffer, pdif_pread);
    if(result.err != OK || result.jpos < 1) {
        sev_free(bytestr);
        sev_free(buffer);
        return (codepoint_ret){ NULL, len, result.err };
    }
    // Done with this
    sev_free(bytestr);

    const size_t uj = (size_t)result.jpos;
    if(result.jpos < pdif_pread){
        alloc_result = sev_realloc((void **)&buffer, uj * sizeof(i32));
        if(alloc_result.err != OK){
            sev_free(buffer);
            return (codepoint_ret){ NULL, uj, alloc_result.err };           
        }
        buffer = (i32 *)alloc_result.allocated;
    }

    return (codepoint_ret){ buffer, uj, OK };
}

codepoint_fill_ret codepoint_buffer_fill(const char *const bytestr, i32 *const buffer, const ptrdiff_t pread){
    ptrdiff_t i = 0, j = 0, last_consumed = 0;
    while(i < pread){
        const ptrdiff_t n = make_codepoint(bytestr + i, &buffer[j], pread - i);
        if(n <= 0){
            return (codepoint_fill_ret){ n, j, i, UTF8_ITERATE_BAD_RET };
        }
        i += n, j++, last_consumed = n;
    }
    return (codepoint_fill_ret){ last_consumed, j, i, OK };
}

ptrdiff_t make_codepoint(const char *utf8str, int32_t *dst, ptrdiff_t len){
  if(!utf8str || !dst)
    return -1;
  // len = the remaining length of the string
  // utf8proc_iterate calculates the end of the string by str + len 
  // since I am passing a possibly offset pointer each time it needs to be
  // slen = len - pos
  const utf8proc_uint8_t *str8 = (const utf8proc_uint8_t *)utf8str;
  return utf8proc_iterate(str8, len, dst);
}

ptrdiff_t make_utf8str(const int32_t codepoint, char *dst){
  if(!dst)
    return -1;

  utf8proc_uint8_t *str8 = (utf8proc_uint8_t *)dst;
  return utf8proc_encode_char(codepoint, str8);
}
