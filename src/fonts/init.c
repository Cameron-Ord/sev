#include "init.h"
#include <stdio.h>

#include "../maps.h"

const i32 ASCII_START = 32;
const i32 ASCII_END = 127;

struct font_open_ret open_font(const char *path, f32 ptsize) {
  TTF_Font *font = TTF_OpenFont(path, ptsize);
  if (!font) {
    printf("Failed to open font-> %s\n", SDL_GetError());
    return (struct font_open_ret){NULL, UNSPECIFIED};
  }
  return (struct font_open_ret){font, OK};
}

void tex_map_fill_defaults(TTF_Font *font, SDL_Renderer *rend,
                           struct tex_map *tm) {
  SDL_Color base = {255, 255, 255, 255};
  for (i32 i = ASCII_START; i < ASCII_END; i++) {
    SDL_Surface *surf = TTF_RenderGlyph_Blended(font, (u32)i, base);
    if (!surf) {
      printf("NON_FATAL: Failed to create surface! -> %s\n", SDL_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
    if (!texture) {
      printf("NON_FATAL: Failed to create texture! -> %s\n", SDL_GetError());
    }

    if (tex_map_insert(tm, i, texture).err != OK) {
      printf("NON_FATAL: Texture could not be inserted!\n");
    }
  }
}
