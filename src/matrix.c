#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void) {
	mat4_t m = {{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
	return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
	mat4_t m = mat4_identity();
	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;
	return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
	mat4_t m = mat4_identity();
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	return m;
}

mat4_t mat4_make_rotation_x(float angle) {
	float c = cos(angle);
	float s = sin(angle);

	mat4_t m = {{
		{ 1,  0,  0, 0 },
		{ 0,  c, -s, 0 },
		{ 0,  s,  c, 0 },
		{ 0,  0,  0, 1 }
	}};
	return m;
}

mat4_t mat4_make_rotation_y(float angle) {
	float c = cos(angle);
	float s = sin(angle);

	mat4_t m = {{
		{ c,  0,  s, 0 },
		{ 0,  1,  0, 0 },
		{-s,  0,  c, 0 },
		{ 0,  0,  0, 1 }
	}};
	return m;
}

mat4_t mat4_make_rotation_z(float angle) {
	float c = cos(angle);
	float s = sin(angle);

	mat4_t m = {{
		{ c, -s,  0, 0 },
		{ s,  c,  0, 0 },
		{ 0,  0,  1, 0 },
		{ 0,  0,  0, 1 }
	}};
	return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
	vec4_t vec = {
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
		m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
	};
	return vec;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
	mat4_t m; 
	for (int i = 0; i < 4; i ++) {
		for (int j = 0; j < 4; j++) {
			m.m[i][j] = 
				a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}
	return m;
} 
