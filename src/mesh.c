#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "vector.h"

mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = {0, 0, 0},
	.translation = {0, 0, 0},
	.scale = {1.0, 1.0, 1.0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1},
	{.x = -1, .y =  1, .z = -1},
	{.x =  1, .y =  1, .z = -1},
	{.x =  1, .y = -1, .z = -1},
	{.x =  1, .y =  1, .z =  1},
	{.x =  1, .y = -1, .z =  1},
	{.x = -1, .y =  1, .z =  1},
	{.x = -1, .y = -1, .z =  1}
};


face_t cube_faces[N_CUBE_FACES] = {
	// front
	{1, 2, 3, .color = 0xFFFF0000 },
	{1, 3, 4, .color = 0xFFFF0000 },
	//back
	{6, 5, 7, .color = 0xFF0000FF },
	{6, 7, 8, .color = 0xFF0000FF },
	//top
	{2, 7, 5, .color = 0xFFFF00FF },
	{2, 5, 3, .color = 0xFFFF00FF },
	//bottom
	{8, 1, 4, .color = 0xFF00FFFF },
	{8, 4, 6, .color = 0xFF00FFFF },
	//left
	{8, 7, 2, .color = 0xFFFFFF00 },
	{8, 2, 1, .color = 0xFFFFFF00 },
	//right
	{4, 3, 5, .color = 0xFF00FFFF },
	{4, 5, 6, .color = 0xFF00FFFF }

};

void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		array_push(mesh.vertices, cube_vertices[i]);
	}

	for (int i = 0; i < N_CUBE_FACES; i++) {
		array_push(mesh.faces, cube_faces[i]);
	}
}

void load_obj_mesh_data(char* file) {
	FILE* fp = fopen(file, "r");

	char line[1024];

	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "v ", 2) == 0) {
			vec3_t vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
			// printf("%f %f %f\n", vertex.x, vertex.y, vertex.z);
		}
		else if (strncmp(line, "f ", 2) == 0) {
			int vertex_index[3];
			int texture_index[3];
			int normal_index[3];
			sscanf(line,
						"f %d/%d/%d %d/%d/%d %d/%d/%d", 
						&vertex_index[0], &texture_index[0], &normal_index[0],
						&vertex_index[1], &texture_index[1], &normal_index[1],
						&vertex_index[2], &texture_index[2], &normal_index[2]
			);
			
			face_t face = {
				.a = vertex_index[0],
				.b = vertex_index[1],
				.c = vertex_index[2]
			};
			array_push(mesh.faces, face);
			// printf("%d %d %d\n", face.a, face.b, face.c);
		}
	}
}


