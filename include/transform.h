#pragma once
#include "math_utils.h"
#include "quaternion.h"
#include "vector_utils.h"

typedef struct Transform {
  Vec3D position;
  Quaternion rotation;
}Transform;

Transform Transform_ZERO();
mat4 rotation_from_forward(Vec3D forward);
mat4 position_to_translationMat(Vec3D position);
mat4 Transform_to_Model_mat4(Transform t);

void rotate_Transform_x(Transform* t, float r);
void rotate_Transform_y(Transform* t, float r);
void rotate_Transform_z(Transform* t, float r);

void rotate_Transform_around_point_x(Transform* t,Vec3D point,float r);
void rotate_Transform_around_point_y(Transform* t,Vec3D point,float r);
void rotate_Transform_around_point_z(Transform* t,Vec3D point,float r);
void rotate_Transform_around_point_axis(Transform *t, Vec3D point, float r, Vec3D axis);

Vec3D get_Transform_basis_z(Transform* t);
Vec3D get_Transform_basis_x(Transform* t);
Vec3D get_Transform_basis_y(Transform* t);
