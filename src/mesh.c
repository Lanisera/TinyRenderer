#include "mesh.h"
#include "triangle.h"
#include "vector.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
	{.x = -1, .y = -1, .z = -1},
	{.x = -1, .y =  1, .z = -1},
	{.x =  1, .y =  1, .z = -1},
	{.x =  1, .y = -1, .z = -1},
	{.x =  1, .y =  1, .z =  1},
	{.x =  1, .y = -1, .z =  1},
	{.x = -1, .y =  1, .z =  1},
	{.x = -1, .y = -1, .z =  1}
};


face_t mesh_faces[N_MESH_FACES] = {
	// front
	{0, 1, 2},
	{0, 2, 3},
	//back
	{5, 4, 6},
	{5, 6, 7},
	//top
	{1, 6, 4},
	{1, 4, 2},
	//bottom
	{7, 0, 3},
	{7, 3, 5},
	//left
	{7, 6, 1},
	{7, 1, 0},
	//right
	{3, 2, 4},
	{3, 4, 5}

};
