#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "display.h"
#include "triangle.h"
#include "vector.h"
#include "mesh.h"

vec3_t camera_position = { .x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0};

triangle_t triangle_project[N_MESH_FACES];

const int fov_factor = 512;

int previous_time = 0;

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

/////////////////////////////////////////////////////////////////////////////////////////
/// function that receives a 3D vector and returns a projected 2D point
/////////////////////////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point)
{
	vec2_t projected_point = {
		.x = fov_factor * point.x / point.z,
		.y = fov_factor * point.y / point.z
	};

	return projected_point;
}

void update(void)
{
	int wait_time = FRAME_TIME_TARGET - (SDL_GetTicks() - previous_time);

	if (wait_time > 0 && wait_time < FRAME_TIME_TARGET) {
		SDL_Delay(wait_time);
	}

	previous_time = SDL_GetTicks();


	cube_rotation.y += 0.005;
	cube_rotation.x += 0.005;
	cube_rotation.z += 0.005;
	
	for (int i = 0; i < N_MESH_FACES; i ++) {
		vec3_t triangle_point[3];
		triangle_point[0] = mesh_vertices[mesh_faces[i].a];
		triangle_point[1] = mesh_vertices[mesh_faces[i].b];
		triangle_point[2] = mesh_vertices[mesh_faces[i].c];

		triangle_t triangle;

		for (int j = 0; j < 3; j ++) {
			vec3_t translate_point = triangle_point[j];
			
			translate_point = vec3_rotate_x(translate_point, cube_rotation.x);
			translate_point = vec3_rotate_y(translate_point, cube_rotation.y);
			translate_point = vec3_rotate_z(translate_point, cube_rotation.z);
			
			// 计算摄像机距离
			translate_point.z -= camera_position.z;

			vec2_t projected_point = project(translate_point);

			// 把投影目标移到屏幕中心
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			triangle.points[j] = projected_point;
		}

		triangle_project[i] = triangle;
	}
}

void render(void) {
	draw_grid(0xFFFFFFFF);
	
	for (int i = 0; i < N_MESH_FACES; i ++) {
		triangle_t triangle = triangle_project[i];
		draw_triangle(triangle.points[0].x, triangle.points[0].y,
									triangle.points[1].x, triangle.points[1].y,
									triangle.points[2].x, triangle.points[2].y,
									0xFFFFFF00);
	}

	render_color_buffer();
	clear_color_buffer(0x00000000);

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
