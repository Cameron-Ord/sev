#include "../errdef.h"
#include "io.h"
#include "../util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <utf8proc.h>


static ptrdiff_t codepoint_buffer_fill(const char *const bytestr, i32 *const buffer, const ptrdiff_t pread){
    ptrdiff_t i = 0, j = 0;
    while(i < pread){
        const ptrdiff_t n = make_codepoint(bytestr + i, &buffer[j], pread - i);
//        if (n <= 0) {
//            return NULL;
//        }
        i += n, j++;
    }
    return j;
}

static FILE* sev_open_read(const char *const path){
    FILE *file = fopen(path, "r");
    if(!file){
        printf("Could not open file->%s\n", strerror(errno));
        return NULL;
    }
    return file;
}

static read_ret read_chunks(FILE* file){
    size_t read = 0, pos = 0, capacity = 4096;

    mem_ret bytes_ret = sev_calloc(capacity, sizeof(char));
    if(bytes_ret.err != OK){
        return (read_ret){ NULL, pos, bytes_ret.err };
    }
    char *bytes = (char *)bytes_ret.allocated;

    const size_t CHUNK = 256;
    char rdbuf[CHUNK];

    while ((read = fread(rdbuf, sizeof(char), CHUNK, file)) > 0){
        if(pos + read >= capacity){
            capacity *= 2;

            mem_ret tmp = sev_realloc((void**)&bytes, capacity * sizeof(char));
            if(tmp.err != OK) {
                sev_free(bytes);
                return (read_ret) { NULL, pos, tmp.err };
            }
            bytes = (char *)tmp.allocated;
        }

        memcpy(bytes + pos, rdbuf, read * sizeof(char));
        pos += read;
    }
    return (read_ret){ bytes, pos, OK };
}

//Probably wanna break this up and error check properly
//need to handle OOM properly
i32 *read_text_file(const char *const path){
    if(!path) return NULL;

    FILE* file = sev_open_read(path);
    if(!file) return NULL;

    mem_ret alloc_result = { NULL, UNSET };
    read_ret read_result = { NULL, 0, UNSET };

    read_result = read_chunks(file); 
    if(read_result.err != OK){
        fclose(file);
        return NULL;
    }
    char *bytes = read_result.data;
    const size_t pread = read_result.pread;
    // Done with this
    fclose(file);

    // Shrink
    alloc_result = sev_realloc((void**)&bytes, pread * sizeof(char));
    if(alloc_result.err != OK){
        return NULL;
    }
    bytes = (char *)alloc_result.allocated;
    
    // Allocate the codepoint buffer (used for editing and rendering)
    alloc_result = sev_calloc(pread, sizeof(i32));
    if(alloc_result.err != OK){
        sev_free(bytes);
        return NULL;
    }
    i32 *buffer = (i32 *)alloc_result.allocated; 

    const ptrdiff_t pdif_pread = (ptrdiff_t)pread;
    const ptrdiff_t j = codepoint_buffer_fill(bytes, buffer, pdif_pread);
    // Done with this
    sev_free(bytes);
 
    // Shrink if necessary - avoid passing a negative or 0 value so the cast doesnt blow up
    // above negative value case isnt handled yet
    if(j < pdif_pread && j > 0){
        const size_t uj = (size_t)j;
        alloc_result = sev_realloc((void **)&buffer, uj * sizeof(i32));
        if(alloc_result.err != OK){
            sev_free(buffer);
            return NULL;           
        }
        buffer = (i32 *)alloc_result.allocated;
    }

    printf("TOTAL READ: %zu\n=====", pread);
    return buffer;
}

bool write_text_file(const char *const path, const i32 *const buffer, const size_t bsize){
    if(!buffer || !path) return false;
    return true;
}