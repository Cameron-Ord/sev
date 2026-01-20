#include "util.h"
#include <stdlib.h>
#include <SDL3/SDL_render.h>


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