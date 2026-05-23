#include "transform.h"
#include "math_utils.h"
#include "vector_utils.h"

Transform Transform_ZERO() {
  return (Transform){
      .position = Vec3D_ZERO(),
      .rotation = mat3_identity(),
  };
}

mat4 rotation_from_forward(Vec3D forward) {
  Vec3D worldUp = {0, 1, 0};

  forward = Vec3D_normalize(forward);

  Vec3D right = Vec3D_normalize(Vec3_cross(worldUp, forward));
  Vec3D up = Vec3_cross(forward, right);

  mat4 m = {0};

  m.m[0][0] = right.x;
  m.m[0][1] = right.y;
  m.m[0][2] = right.z;
  m.m[0][3] = 0;

  m.m[1][0] = up.x;
  m.m[1][1] = up.y;
  m.m[1][2] = up.z;
  m.m[1][3] = 0;

  m.m[2][0] = -forward.x;
  m.m[2][1] = -forward.y;
  m.m[2][2] = -forward.z;
  m.m[2][3] = 0;

  m.m[3][0] = 0;
  m.m[3][1] = 0;
  m.m[3][2] = 0;
  m.m[3][3] = 1;

  return m;
}

mat4 position_to_translationMat(Vec3D position) {
  mat4 M = mat4_identity();

  M.m[0][3] = position.x;
  M.m[1][3] = position.y;
  M.m[2][3] = position.z;

  return M;
}

void rotate_Transform_x(Transform *t, float r) {
  t->rotation = rotate_mat3_x(t->rotation, r);
}
void rotate_Transform_y(Transform *t, float r) {
  t->rotation = rotate_mat3_y(t->rotation, r);
}
void rotate_Transform_z(Transform *t, float r) {
  t->rotation = rotate_mat3_z(t->rotation, r);
}

mat4 Transform_to_Model_mat4(Transform t) {
  mat4 M = mat4_identity();

  // rotation (3x3 → 4x4)
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      M.m[i][j] = t.rotation.m[i][j];

  // translation
  M.m[0][3] = t.position.x;
  M.m[1][3] = t.position.y;
  M.m[2][3] = t.position.z;

  return M;
}
