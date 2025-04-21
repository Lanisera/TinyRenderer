#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "triangle.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

vec3_t camera_position = { .x = 0, .y = 0, .z = -5};

triangle_t* triangle_to_render = NULL;

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

	load_obj_mesh_data("./assets/f22.obj");
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
	/////////////////////////////////////////////////////////////////////////////////////////
	/// 稳定渲染帧率
	/////////////////////////////////////////////////////////////////////////////////////////
	int wait_time = FRAME_TIME_TARGET - (SDL_GetTicks() - previous_time);

	if (wait_time > 0 && wait_time < FRAME_TIME_TARGET) {
		SDL_Delay(wait_time);
	}

	previous_time = SDL_GetTicks();
	/////////////////////////////////////////////////////////////////////////////////////////
	
	triangle_to_render = NULL;

	mesh.rotation.x += 0.005;
	mesh.rotation.y += 0.000;
	mesh.rotation.z += 0.000;
	
	int num_face = array_length(mesh.faces);
	for (int i = 0; i < num_face; i ++) {
		vec3_t triangle_point[3];
		triangle_point[0] = mesh.vertices[mesh.faces[i].a - 1];
		triangle_point[1] = mesh.vertices[mesh.faces[i].b - 1];
		triangle_point[2] = mesh.vertices[mesh.faces[i].c - 1];

		triangle_t triangle;

		for (int j = 0; j < 3; j ++) {
			vec3_t translate_point = triangle_point[j];
			
			translate_point = vec3_rotate_x(translate_point, mesh.rotation.x);
			translate_point = vec3_rotate_y(translate_point, mesh.rotation.y);
			translate_point = vec3_rotate_z(translate_point, mesh.rotation.z);
			
			// 计算摄像机距离
			translate_point.z -= camera_position.z;

			vec2_t projected_point = project(translate_point);

			// 把投影目标移到屏幕中心
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			triangle.points[j] = projected_point;
		}

		array_push(triangle_to_render, triangle);
	}
}

void render(void) {
	draw_grid(0xFFFFFFFF);
	
	int num_face = array_length(triangle_to_render);
	for (int i = 0; i < num_face; i ++) {
		triangle_t triangle = triangle_to_render[i];
		draw_triangle(triangle.points[0].x, triangle.points[0].y,
									triangle.points[1].x, triangle.points[1].y,
									triangle.points[2].x, triangle.points[2].y,
									0xFFFFFF00);
	}

	render_color_buffer();
	clear_color_buffer(0x00000000);

	array_free(triangle_to_render);

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(color_buffer);
	array_free(mesh.vertices);
	array_free(mesh.faces);
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
	free_resources();

	return 0;
}
