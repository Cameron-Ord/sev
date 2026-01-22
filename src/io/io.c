#include "../errdef.h"
#include "io.h"
#include "../util.h"
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


    const read_ret data = read_chunks(file); 
    if(data.err != OK){
        fclose(file);
        return NULL;
    }
    char *bytes = data.data;
    const size_t pread = data.pread;

    // Shrink
    mem_ret shrunk = sev_realloc((void**)&bytes, pread * sizeof(char));
    if(shrunk.err != OK){
        fclose(file);
        return NULL;
    }

    
    i32 *buffer = calloc(pos, sizeof(i32));
    if(!buffer){
        fclose(file);
        sev_free(bytes);
        return NULL;
    }

    ptrdiff_t i = 0, j = 0;
    const ptrdiff_t lpos = (ptrdiff_t)pos;
    while(i < lpos){
        const ptrdiff_t n = make_codepoint(bytes + i, &buffer[j]);
        if(n <= 0 || i + n > lpos)
            break;

        i += n, j++;
    }

    if(j < lpos){
        // Shrink
        buffer = realloc(buffer, (size_t)j * sizeof(i32));
    }

    for(i32 k = 0; k < j; k++){
        printf("%c", buffer[k]);
    }

    printf("\n=====\n");
    printf("TOTAL READ: %zu\n=====", pos);

    sev_free(bytes);
    fclose(file);

    return buffer;
}

bool write_text_file(const char *const path, const i32 *const buffer, const size_t bsize){
    if(!buffer || !path) return false;
    return true;
}