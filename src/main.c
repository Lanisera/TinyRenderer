#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "display.h"
#include "vector.h"

/////////////////////////////////////////////////////////////////////////////////////////
/// Declare an array of vector/points
/////////////////////////////////////////////////////////////////////////////////////////
const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS];

bool is_running = false;

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

	int point_cnt = 0;
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { .x = x, .y = y, .z = z };
				cube_points[point_cnt++] = new_point;
			}
		}
	}
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

int main(void)
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
