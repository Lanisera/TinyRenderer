#include "vector.h"
#include <math.h>

// TODO: 向量运算相关函数

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
	vec3_t rotate_vec = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotate_vec;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
	vec3_t rotate_vec = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rotate_vec;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
	vec3_t rotate_vec = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotate_vec;
}
