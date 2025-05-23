#include "display.h"
#include <math.h>
#include <stdlib.h>

Cull_Method cull_method;
Render_Method render_method;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* texture_color_buffer = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error SDL_Init");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (window == NULL) {
		fprintf(stderr, "Error SDL_CreateWindow");
	}

	renderer = SDL_CreateRenderer(window,  -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error SDL_CreateRenderer"); 
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_pixel(int x, int y, uint32_t color)
{
	color_buffer[y * window_width + x] = color;
}

void draw_grid(uint32_t color)
{
	for (int y = 0; y < window_height; y += 50) {
		for (int x = 0; x < window_width; x += 50) {
			color_buffer[y * window_width + x] = color;
		}
	}

}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int current_x = i + x;
			int current_y = j + y;
			draw_pixel(current_x, current_y, color);
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;

	int longest_delta = (abs(delta_x) > abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)longest_delta;
	float y_inc = delta_y / (float)longest_delta;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_delta; i++) {
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
	
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}

void render_color_buffer(void)
{
	SDL_UpdateTexture(
			texture_color_buffer,
			NULL, 
			color_buffer, 
			(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, texture_color_buffer, NULL, NULL);
}

void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++)
		for (int x = 0; x < window_width; x++)
			color_buffer[y * window_width + x] = color;
}

void destroy_window(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

