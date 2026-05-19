#pragma once

#include "vector_utils.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct { float m[4][4]; } mat4;

Vec4D mul_mat4_vec4(mat4 M, Vec4D v);
mat4 mat4_transpose(mat4 M);
mat4 mat4_translate_inverse(Vec3D p);
mat4 mat4_identity();
mat4 mat4_multiply(mat4 A, mat4 B);
float Q_rsqrt( float number );
