#include "display.h"

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
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

