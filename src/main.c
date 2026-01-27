

#include <SDL3/SDL.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <utf8proc.h>

#include "buffer/buffer.h"
#include "errdef.h"
#include "fonts/init.h"
#include "io/io.h"
#include "maps.h"
#include "renderer/renderer.h"
#include "util.h"
#include "window/window.h"

static void quit(void) { SDL_Quit(); }

bool init_sdl(void) { return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS); }

bool init_ttf(void) { return TTF_Init(); }

const u32 FPS_TARGET = 60;
const i32 BASE_WIDTH = 800;
const i32 BASE_HEIGHT = 600;
const u64 WFLAGS = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
const char *title = "sev";

const SDL_Color bg = {0, 0, 0, 255};

static struct buf_init_ret create_buffer(const char *path) {
  const i32 path_provided = path != NULL;

  switch (path_provided) {
  default: {
    return (struct buf_init_ret){NULL, BAD_PARAM};
  } break;

  case 1: {
    struct file_ret res = read_text_file(path);
    if (res.err != OK) {
      return (struct buf_init_ret){NULL, res.err};
    }

    struct codepoint_ret parse_res = bytestr_to_codepoints(res.data, res.size);
    if (parse_res.err != OK) {
      return (struct buf_init_ret){NULL, res.err};
    }

    struct buf_init_ret init_res = buf_init(parse_res.size, parse_res.buffer);
    if (init_res.err != OK) {
      return (struct buf_init_ret){NULL, init_res.err};
    }

    return init_res;
  } break;

  case 0: {
    struct buf_init_ret init_res = buf_init(1, NULL);
    if (init_res.err != OK) {
      return (struct buf_init_ret){NULL, init_res.err};
    }
    return init_res;
  } break;
  }
}

int main(int argc, char *argv[]) {
  if (!init_sdl()) {
    printf("Failed to initialize SDL! ->%s\n", SDL_GetError());
    return 1;
  }

  if (!init_ttf()) {
    printf("Failed to initialize SDL TTF! ->%s\n", SDL_GetError());
    return 1;
  }

  window win = {win_create(title, BASE_WIDTH, BASE_HEIGHT, WFLAGS), BASE_WIDTH,
                BASE_HEIGHT, title, WFLAGS};

  renderer rend = {
      renderer_create(win.window),
  };

  struct font_open_ret font_res = open_font("placeholder.ttf", 12.0f);
  TTF_Font *font = font_res.font;

  struct buf_map_ret bmap = buf_map_init(TABLE_BASE_CAPAC);
  if (bmap.err != OK) {
    return 1;
  }
  struct buf_map *map = bmap.map;
  // buf_map_insert(bmap.map, 0, buf_init(1, 1).buf);

  const i32 file_provided = argc > 1 && argc < 3;
  switch (file_provided) {
  case 1: {
    const char *path = argv[1];
    if (buf_map_insert(map, 0, create_buffer(path).buf).err != OK) {
      return 1;
    }
  } break;
  case 0: {
    if (buf_map_insert(map, 0, create_buffer(NULL).buf).err != OK) {
      return 1;
    }
  } break;
  }

  struct tex_map_ret tmap = tex_map_init(1 << 8);
  if (tmap.err != OK) {
    printf("Failed to initialize texture map!\n");
    return 1;
  }
  struct tex_map *texmap = tmap.map;
  tex_map_fill_defaults(font, rend.renderer, texmap);

  win_show(win.window);
  win_start_text_input(win.window);
  const u64 fg = (u64)1000 / FPS_TARGET;
  bool running = true;

  while (running) {
    u64 start = SDL_GetTicks();
    renderer_colour(rend.renderer, &bg);
    renderer_clear(rend.renderer);

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      default:
        break;

      case SDL_EVENT_TEXT_INPUT: {
      } break;

      case SDL_EVENT_QUIT: {
        running = false;
      } break;

      case SDL_EVENT_KEY_DOWN: {
      } break;
      }
    }

    renderer_present(rend.renderer);
    u64 ft = SDL_GetTicks() - start;
    if (ft < fg) {
      u64 delay = fg - ft;
      SDL_Delay((u32)delay);
    }
  }

  renderer_destroy(rend.renderer);
  win_destroy(win.window);
  quit();

  return 0;
}
