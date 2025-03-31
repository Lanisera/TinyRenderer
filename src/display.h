#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* texture_color_buffer;

extern int window_width;
extern int window_height;

bool initialize_window(void);

void draw_grid(uint32_t color);

void render_color_buffer(void);

void clear_color_buffer(uint32_t color);

void destroy_window(void);

#endif
