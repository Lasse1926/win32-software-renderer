#pragma once
#include "math_utils.h"
#include "vector_utils.h"

typedef struct Transform {
  Vec3D position;
  mat3 rotation;
  Vec3D euler_angles;
}Transform;

Transform Transform_ZERO();
mat4 rotation_from_forward(Vec3D forward);
mat4 position_to_translationMat(Vec3D position);
mat4 Transform_to_Model_mat4(Transform t);

void rotate_Transform_x(Transform* t, float r);
void rotate_Transform_y(Transform* t, float r);
void rotate_Transform_z(Transform* t, float r);
