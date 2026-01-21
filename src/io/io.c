#include "io.h"
#include "../util.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <utf8proc.h>

//Probably wanna break this up and error check properly
//need to handle OOM properly
i32 *read_text_file(const char *const path){
    if(!path) return NULL;
    FILE *file = fopen(path, "r");
    if(!file){
        printf("Could not open file->%s\n", strerror(errno));
        return NULL;
    }
    
    size_t read = 0, pos = 0;
    const size_t CHUNK = 256;
    size_t CAPACITY = 4096;
    char rdbuf[CHUNK];
    char *bytes = calloc(CAPACITY, sizeof(char));
    if(!bytes){
        fclose(file);
        return NULL;
    }

    while ((read = fread(rdbuf, sizeof(char), CHUNK, file)) > 0){
        if(pos + read >= CAPACITY){
            CAPACITY *= 2;
            char *tmp = realloc(bytes, CAPACITY * sizeof(char));
            bytes = tmp;
        }

        memcpy(bytes + pos, rdbuf, read * sizeof(char));
        pos += read;
    }
    // Shrink
    bytes = realloc(bytes, pos * sizeof(char));

    
    i32 *buffer = calloc(pos, sizeof(i32));
    if(!buffer){
        fclose(file);
        sev_free(bytes);
        return NULL;
    }

    long long i = 0, j = 0;
    const long long lpos = (long long)pos;
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