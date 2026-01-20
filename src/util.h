#ifndef UTIL_H
#define UTIL_H

typedef struct SDL_Texture SDL_Texture;

void sev_free(void *ptr);
void sdl_texture_free(SDL_Texture *ptr);

#endif // UTIL_H