#ifndef INIT_H
#define INIT_H
#include "../errdef.h"
#include "../typedefs.h"

struct tex_map;

#include <SDL3_ttf/SDL_ttf.h>
struct font_open_ret {
  TTF_Font *font;
  i32 err;
};
struct font_open_ret open_font(const char *path, f32 ptsize);
void tex_map_fill_defaults(TTF_Font *font, SDL_Renderer *rend,
                           struct tex_map *tm);
#endif
