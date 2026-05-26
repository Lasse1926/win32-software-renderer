#pragma once

#include "math_utils.h"
#include "vector_utils.h"
typedef struct Quaternion {
    float w; // scalar part
    float x;
    float y;
    float z;
}Quaternion;


Quaternion Quaternion_identity();
void Quaternion_normalized(Quaternion* q);

Quaternion Quaternion_rotate_x(float r);
Quaternion Quaternion_rotate_y(float r);
Quaternion Quaternion_rotate_z(float r);

Quaternion Quaternion_rotate_axis(Vec3D axis, float r);

Quaternion Quaternion_mult(Quaternion a,Quaternion b);

mat3 mat3_from_Quaternion(Quaternion q);

Vec3D get_Quaternion_basis_z(Quaternion q);
Vec3D get_Quaternion_basis_x(Quaternion q);
Vec3D get_Quaternion_basis_y(Quaternion q);
