#include "window.h"
#include <SDL3/SDL_keyboard.h>
#include <stdio.h>

SDL_Window *win_create(const char *title, i32 w, i32 h, u64 flags){
    SDL_Window *tmp = SDL_CreateWindow(title, w, h, flags);
    if(!tmp){
        printf("Failed to create window! -> %s\n", SDL_GetError());
        return NULL;
    }
    return tmp;
}

void win_destroy(SDL_Window *win){
    SDL_DestroyWindow(win);
}

void win_update_dimensions(SDL_Window *win, i32 *w, i32 *h){
    SDL_GetWindowSize(win, w, h);
}

void win_show(SDL_Window *win){
    SDL_ShowWindow(win);
}

void win_stop_text_input(SDL_Window *win){
    SDL_StopTextInput(win);
}

void win_start_text_input(SDL_Window *win){
    SDL_StartTextInput(win);
}