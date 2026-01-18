#include <stdio.h>
#include "renderer.h"

SDL_Renderer *renderer_create(SDL_Window *win){
    SDL_Renderer *tmp = SDL_CreateRenderer(win, NULL);
    if(!tmp){
        printf("Failed to create a render context! -> %s\n", SDL_GetError());
        return NULL;
    }
    return tmp;
}

void renderer_destroy(SDL_Renderer *rend){
    SDL_DestroyRenderer(rend);
}

void renderer_clear(SDL_Renderer *rend){
    SDL_RenderClear(rend);
}

void renderer_colour(SDL_Renderer *rend, const SDL_Color *col){
    SDL_SetRenderDrawColor(rend, col->r, col->g, col->b, col->a);
}

void renderer_present(SDL_Renderer *rend){
    SDL_RenderPresent(rend);
}