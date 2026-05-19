#pragma once
#include "math_utils.h"
#include "vector_utils.h"

typedef struct Transform {
  Vec3D position;
  Vec3D forward_vec;
}Transform;

Transform Transform_ZERO();
mat4 rotation_from_forward(Vec3D forward);
