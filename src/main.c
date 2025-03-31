#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

bool is_running = false;

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
	
	// printf("Debug: window_width: {%d}, window_height: {%d} \n", window_width, window_height);

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

void setup(void)
{
	// TODO:
	color_buffer = (uint32_t*)malloc(window_height * window_width * sizeof(uint32_t));
	texture_color_buffer = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_STREAMING, 
			window_width, 
			window_height
	);
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			if (event.key.keysym.sym == SDLK_a)
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			if (event.key.keysym.sym == SDLK_b)
				SDL_SetWindowFullscreen(window, SDL_WINDOW_BORDERLESS);

			break;
	}

}

void update(void)
{
	// TODO:
}

/// Helper Function

void draw_grid(uint32_t color)
{
	for (int y = 0; y < window_height; y += 50) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[y * window_width + x] = color;
		}
	}

	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x += 50) {
			color_buffer[y * window_width + x] = color;
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

void render(void)
{
	// TODO:
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(0xFF000000);

	render_color_buffer();
	clear_color_buffer(0xFFFFFFFF);

	SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main()
{
	is_running = initialize_window();
	
	setup();

	while(is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
