#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error SDL_Init");
		return false;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS
	);
	if (window == NULL) {
		fprintf(stderr, "Error SDL_CreateWindow");
	}

	renderer = SDL_CreateRenderer(window,  -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error SDL_CreateRenderer"); 
	}

	return true;
}

void setup(void)
{
	// TODO:
}

void process_input(void)
{
	// TODO:
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
	}

}

void update()
{
	// TODO:
}

void render()
{
	// TODO:
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
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

	return 0;
}
