#include "vector.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////
/// function for 2D vector
/////////////////////////////////////////////////////////////////////////////////////////
float vec2_length(vec2_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t new_vec2 = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return new_vec2;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t new_vec2 = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return new_vec2;
}

vec2_t vec2_mul(vec2_t a, float factor) {
	vec2_t new_vec2 = {
		.x = a.x * factor,
		.y = a.y * factor
	};
	return new_vec2;
}

vec2_t vec2_div(vec2_t a, float factor) {
	vec2_t new_vec2 = {
		.x = a.x / factor,
		.y = a.y / factor
	};
	return new_vec2;
}

float vec2_dot(vec2_t a, vec2_t b) {
	return a.x * b.x + a.y * b.y;
}

/////////////////////////////////////////////////////////////////////////////////////////
/// function for 3D vector
/////////////////////////////////////////////////////////////////////////////////////////
float vec3_length(vec3_t v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t new_vec3 = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return new_vec3;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t new_vec3 = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return new_vec3;
}

vec3_t vec3_mul(vec3_t a, float factor) {
	vec3_t new_vec3 = {
		.x = a.x * factor,
		.y = a.y * factor,
		.z = a.z * factor
	};
	return new_vec3;
}

vec3_t vec3_div(vec3_t a, float factor) {
	vec3_t new_vec3 = {
		.x = a.x / factor,
		.y = a.y / factor,
		.z = a.z / factor
	};
	return new_vec3;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t new_vec3 = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return new_vec3;
}

float vec3_dot(vec3_t a, vec3_t b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

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
