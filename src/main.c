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
#include "matrix.h"

vec3_t camera_position = {0, 0, 0};

triangle_t* triangle_to_render = NULL;

const int fov_factor = 512;

int previous_time = 0;

bool is_running = false;

void setup(void)
{
	cull_method = CULL_BACKFACE;
	render_method = RENDER_WIRE;

	color_buffer = (uint32_t*)malloc(window_height * window_width * sizeof(uint32_t));
	texture_color_buffer = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_STREAMING, 
			window_width, 
			window_height
	);

	load_cube_mesh_data();
	// load_obj_mesh_data("./assets/cube.obj");
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
			if (event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE;
			if (event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE_VERTEX;
			if (event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;
			if (event.key.keysym.sym == SDLK_d)
				cull_method = CULL_NONE;

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

/////////////////////////////////////////////////////////////////////////////////////////
/// Helper Function: Merge Sort
/////////////////////////////////////////////////////////////////////////////////////////
void merge(triangle_t* arr, int left, int mid, int right)
{
	int i = left, j = mid + 1;
	while (i <= mid && j <= right) {
		if (arr[i].avg_depth > arr[j].avg_depth) i++;
		else {
			triangle_t tmp = arr[j];
			for (int k = j; k > i; k--) arr[k] = arr[k - 1];
			arr[i] = tmp;

			i++;
			mid++;
			j++;
		}
	}
}

void merge_sort(triangle_t* arr, int left, int right)
{
	if (left < right) {
		int mid = (left + right) / 2;
		merge_sort(arr, left, mid);
		
		merge_sort(arr, mid + 1, right);
		
		merge(arr, left, mid, right);
	}

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
	mesh.rotation.y += 0.005;
	mesh.rotation.z += 0.005;

	mesh.translation.x += 0.005;

	mat4_t scale_mat = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t rotation_x_mat = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_y_mat = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_z_mat = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t translation_mat = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	mat4_t world_matrix = mat4_identity();
	world_matrix = mat4_mul_mat4(scale_mat, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_x_mat, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_y_mat, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_z_mat, world_matrix);
	world_matrix = mat4_mul_mat4(translation_mat, world_matrix);
	
	int num_face = array_length(mesh.faces);
	for (int i = 0; i < num_face; i ++) {
		vec4_t triangle_point[3];
		triangle_point[0] = vec4_from_vec3(mesh.vertices[mesh.faces[i].a - 1]);
		triangle_point[1] = vec4_from_vec3(mesh.vertices[mesh.faces[i].b - 1]);
		triangle_point[2] = vec4_from_vec3(mesh.vertices[mesh.faces[i].c - 1]);

		vec4_t transformed_vertices[3];

		// 三角形面片线性变换
		for (int j = 0; j < 3; j ++) {
			vec4_t transformed_point = triangle_point[j];
			
			transformed_point = mat4_mul_vec4(world_matrix, transformed_point);
			
			// 远离摄像机
			transformed_point.z += 5;

			transformed_vertices[j] = transformed_point;
		}

		if (cull_method == CULL_BACKFACE) {
			vec3_t vec3_a = vec3_from_vec4(transformed_vertices[0]); /*  A  */
			vec3_t vec3_b = vec3_from_vec4(transformed_vertices[1]); /* / \ */
			vec3_t vec3_c = vec3_from_vec4(transformed_vertices[2]); /* B-C */

			vec3_t vec3_ab = vec3_sub(vec3_b, vec3_a);
			vec3_t vec3_ac = vec3_sub(vec3_c, vec3_a);

			vec3_t normal = vec3_cross(vec3_ab, vec3_ac);

			vec3_t camera_ray = vec3_sub(camera_position, vec3_a);

			float dot = vec3_dot(normal, camera_ray);

			if (dot < 0) continue;
		}

		triangle_t triangle;
		triangle.color = mesh.faces[i].color;

		// 投影变换
		for (int j = 0; j < 3; j++) {
			vec2_t projected_point = project(vec3_from_vec4(transformed_vertices[j]));

			// 把投影目标移到屏幕中心
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			triangle.points[j] = projected_point;
		}

		triangle.avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		array_push(triangle_to_render, triangle);
	}

	int num_length = array_length(triangle_to_render);
	merge_sort(triangle_to_render, 0, num_length - 1);
	// for (int i = 0; i < num_length; i++) {
	// 	for (int j = i; j < num_length; j++) {
	// 		if (triangle_to_render[i].avg_depth < triangle_to_render[j].avg_depth) {
	// 			triangle_t tmp = triangle_to_render[i];
	// 			triangle_to_render[i] = triangle_to_render[j];
	// 			triangle_to_render[j] = tmp;
	// 		}
	// 	}
	// }
}

void render(void) {
	draw_grid(0xFFFFFFFF);
	
	int num_face = array_length(triangle_to_render);
	for (int i = 0; i < num_face; i ++) {
		triangle_t triangle = triangle_to_render[i];
		
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
										triangle.points[1].x, triangle.points[1].y,
										triangle.points[2].x, triangle.points[2].y,
										triangle.color);

		}

		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_triangle(triangle.points[0].x, triangle.points[0].y,
										triangle.points[1].x, triangle.points[1].y,
										triangle.points[2].x, triangle.points[2].y,
										0xFFFFFFFF);

		}

		if (render_method == RENDER_WIRE_VERTEX) {

		}
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
