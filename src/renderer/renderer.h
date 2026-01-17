#ifndef RENDERER_H
#define RENDERER_H

#include "../typedefs.h"
#include <SDL3/SDL_render.h>

typedef struct {
    SDL_Renderer *renderer;
} renderer;

SDL_Renderer *renderer_create(SDL_Window *win);
void renderer_destroy(SDL_Renderer *rend);
void renderer_clear(SDL_Renderer *rend);
void renderer_colour(SDL_Renderer *rend, const SDL_Color *col);
void renderer_present(SDL_Renderer *rend);
#endif // RENDERER_H