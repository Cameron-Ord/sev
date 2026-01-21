//#include <stdlib.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include <utf8proc.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "window/window.h"
#include "renderer/renderer.h"
#include "maps.h"
#include "events/events.h"
#include "buffer/buffer.h"

static void quit(void){
  SDL_Quit();
}

bool init_sdl(void){
  return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

bool init_ttf(void){
  return TTF_Init();
}

static utf8proc_ssize_t make_codepoint(const char *utf8str, int32_t *dst){
  if(!utf8str || !dst)
    return -1;

  const utf8proc_uint8_t *str8 = (const utf8proc_uint8_t *)utf8str;
  return utf8proc_iterate(str8, -1, dst);
}

static utf8proc_ssize_t make_utf8str(const int32_t codepoint, char *dst){
  if(!dst)
    return -1;

  utf8proc_uint8_t *str8 = (utf8proc_uint8_t *)dst;
  return utf8proc_encode_char(codepoint, str8);
}

const u32 FPS_TARGET = 60;
const i32 BASE_WIDTH = 800;
const i32 BASE_HEIGHT = 600;
const u64 WFLAGS = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
const char *title = "sev";

const SDL_Color bg = { 0, 0, 0, 255 };

int main(int argc, char *argv[]){
  if(!(argc > 1 && argc < 3)){
    printf("Usage: %s filename\n", title);
    return 0;
  }
  
  if(!init_sdl()){
    printf("Failed to initialize SDL! ->%s\n", SDL_GetError());
    return 1;
  }

  if(!init_ttf()){
    printf("Failed to initialize SDL TTF! ->%s\n", SDL_GetError());
    return 1;
  }

  window win = { 
    win_create(title, BASE_WIDTH, BASE_HEIGHT, WFLAGS), 
    BASE_WIDTH, 
    BASE_HEIGHT, 
    title, 
    WFLAGS
  };

  renderer rend = {
    renderer_create(win.window),
  };
  
  //struct buf_map *bmap = buf_map_init(TABLE_BASE_CAPAC);
  //buf_map_insert(bmap, 0, buf_alloc(1, 1));

  win_show(win.window);
  win_start_text_input(win.window);
  const u64 fg = (u64)1000 / FPS_TARGET;
  bool running = true;
  
  while(running){
    u64 start = SDL_GetTicks();
    renderer_colour(rend.renderer, &bg);
    renderer_clear(rend.renderer);

    SDL_Event ev;
    while(SDL_PollEvent(&ev)){
      switch(ev.type){
        default: break;

        case SDL_EVENT_TEXT_INPUT: {
          int32_t cp = 0;
          make_codepoint(ev.text.text, &cp);

        } break;
        
        case SDL_EVENT_QUIT: {
          running = false;
        }break;

        case SDL_EVENT_KEY_DOWN:{
        }break;
      }       
    }
    
    renderer_present(rend.renderer);
    u64 ft = SDL_GetTicks() - start;
    if(ft < fg){
      u64 delay = fg - ft;
      SDL_Delay((u32)delay);
    }
  }

  renderer_destroy(rend.renderer);
  win_destroy(win.window);
  quit();

  return 0;
}
