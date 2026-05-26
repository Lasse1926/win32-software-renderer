#pragma once

#include "vector_utils.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct { float m[4][4]; } mat4;
typedef struct { float m[3][3]; } mat3;

Vec4D mul_mat4_vec4(mat4 M, Vec4D v);
mat4 mat4_transpose(mat4 M);
mat4 mat4_translate_inverse(Vec3D p);
mat4 mat4_identity();
mat4 mat4_multiply(mat4 A, mat4 B);

Vec3D mul_mat3_vec3(mat3 M, Vec3D v);
mat3 mat3_identity();
mat3 mat3_multiply(mat3 A, mat3 B);

mat3 rotate_mat3_x(mat3 m,float r);
mat3 rotate_mat3_y(mat3 m,float r);
mat3 rotate_mat3_z(mat3 m,float r);

mat3 mat3_rotate_x(float r);
mat3 mat3_rotate_y(float r);
mat3 mat3_rotate_z(float r);

mat3 mat3_rotate_axis(Vec3D axis, float r);

mat4 mat4_from_mat3(mat3 A);

float Q_rsqrt( float number );
