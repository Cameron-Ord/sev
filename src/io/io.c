#include "../errdef.h"
#include "io.h"
#include "../util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <utf8proc.h>


static FILE* sev_open_read(const char *const path){
    FILE *file = fopen(path, "r");
    if(!file){
        printf("Could not open file->%s\n", strerror(errno));
        return NULL;
    }
    return file;
}

static struct read_ret read_chunks(FILE* file){
    size_t read = 0, pos = 0, capacity = 4096;

    struct mem_ret result = sev_calloc(capacity, sizeof(char));
    if(result.err != OK){
        return (struct read_ret){ NULL, pos, result.err };
    }
    char *bytes = (char *)result.allocated;

    const size_t CHUNK = 256;
    char rdbuf[CHUNK];

    while ((read = fread(rdbuf, sizeof(char), CHUNK, file)) > 0){
        if(pos + read >= capacity){
            capacity *= 2;

            struct mem_ret tmp = sev_realloc((void**)&bytes, capacity * sizeof(char));
            if(tmp.err != OK) {
                sev_free(bytes);
                return (struct read_ret){ NULL, pos, tmp.err };
            }
            bytes = (char *)tmp.allocated;
        }

        memcpy(bytes + pos, rdbuf, read * sizeof(char));
        pos += read;
    }
    return (struct read_ret){ bytes, pos, OK };
}

//Probably wanna break this up and error check properly
//need to handle OOM properly
struct file_ret read_text_file(const char *const path){
    if(!path) return (struct file_ret){ NULL, 0, BAD_PARAM };

    FILE* file = sev_open_read(path);
    if(!file) return (struct file_ret){NULL, 0, FILE_OPEN_FAIL };

    struct mem_ret alloc_result = { NULL, UNSET };
    struct read_ret read_result = { NULL, 0, UNSET };

    read_result = read_chunks(file); 
    if(read_result.err != OK || read_result.pread < 1){
        fclose(file);
        return (struct file_ret){ NULL, read_result.pread, read_result.err };
    }
    char *bytes = read_result.data;
    const size_t len = read_result.pread;
    // Done with this
    fclose(file);

    // Shrink
    alloc_result = sev_realloc((void**)&bytes, len * sizeof(char));
    if(alloc_result.err != OK){
        sev_free(bytes);
        return (struct file_ret){ NULL, len, alloc_result.err };
    }
    bytes = (char *)alloc_result.allocated;
    printf("TOTAL READ: %zu\n=====", len);

    return (struct file_ret){ bytes, len, OK };
}

bool write_text_file(const char *const path, const i32 *const buffer, const size_t bsize){
    if(!buffer || !path) return false;
    return true;
}