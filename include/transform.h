#pragma once
#include "vector_utils.h"

typedef struct Transform {
  Vec3D position;
  Vec3D forward_vec;
}Transform;

Transform Transform_ZERO();
