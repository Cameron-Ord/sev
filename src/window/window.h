#ifndef WINDOW_H
#define WINDOW_H

#include "../typedefs.h"
#include <SDL3/SDL_video.h>

typedef struct {
    SDL_Window *window;
    i32 w, h;
    const char *title;
    u64 flags_at_creation;
} window;

SDL_Window *win_create(const char *title, i32 w, i32 h, u64 flags);
void win_destroy(SDL_Window *win);
void win_update_dimensions(SDL_Window *win, i32 *w, i32 *h);
void win_show(SDL_Window *win);
void win_start_text_input(SDL_Window *win);
void win_stop_text_input(SDL_Window *win);


#endif // WINDOW_H