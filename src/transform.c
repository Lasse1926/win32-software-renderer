#include "transform.h"
#include "math_utils.h"
#include "quaternion.h"
#include "vector_utils.h"

Transform Transform_ZERO() {
  return (Transform){
      .position = Vec3D_ZERO(),
      .rotation = Quaternion_identity(),
      .scale = Vec3D_XYZ(1.0f,1.0f,1.0f),
  };
}

mat4 rotation_from_forward(Vec3D forward) {
  Vec3D worldUp = {0, 1, 0};

  forward = Vec3D_normalize(forward);

  Vec3D right = Vec3D_normalize(Vec3D_cross(worldUp, forward));
  Vec3D up = Vec3D_cross(forward, right);

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
  Quaternion dq = Quaternion_rotate_x(r);

  t->rotation = Quaternion_mult(dq, t->rotation);
  Quaternion_normalized(&t->rotation);
}

void rotate_Transform_y(Transform *t, float r) {
  Quaternion dq = Quaternion_rotate_y(r);

  t->rotation = Quaternion_mult(dq, t->rotation);
  Quaternion_normalized(&t->rotation);
}

void rotate_Transform_z(Transform *t, float r) {
  Quaternion dq = Quaternion_rotate_z(r);

  t->rotation = Quaternion_mult(dq, t->rotation);
  Quaternion_normalized(&t->rotation);
}

mat4 Transform_to_Model_mat4(Transform t)
{
    mat4 M = mat4_identity();

    mat3 R = mat3_from_Quaternion(t.rotation);

    // apply scale into rotation matrix space
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            M.m[i][j] = R.m[i][j];
        }
    }

    // apply scale per axis (IMPORTANT)
    M.m[0][0] *= t.scale.x;
    M.m[1][0] *= t.scale.x;
    M.m[2][0] *= t.scale.x;

    M.m[0][1] *= t.scale.y;
    M.m[1][1] *= t.scale.y;
    M.m[2][1] *= t.scale.y;

    M.m[0][2] *= t.scale.z;
    M.m[1][2] *= t.scale.z;
    M.m[2][2] *= t.scale.z;

    // translation
    M.m[0][3] = t.position.x;
    M.m[1][3] = t.position.y;
    M.m[2][3] = t.position.z;

    return M;
}

static void rotate_around_point(Transform *t, Vec3D point, Quaternion dq) {
  Vec3D offset = Vec3D_sub(t->position, point);

  mat3 R = mat3_from_Quaternion(dq);
  Vec3D rotated = mul_mat3_vec3(R, offset);

  t->position = Vec3D_add(point, rotated);

  t->rotation = Quaternion_mult(dq, t->rotation);
  Quaternion_normalized(&t->rotation);
}

void rotate_Transform_around_point_x(Transform *t, Vec3D point, float r) {
  Quaternion dq = Quaternion_rotate_x(r);
  rotate_around_point(t, point, dq);
}

void rotate_Transform_around_point_y(Transform *t, Vec3D point, float r) {
  Quaternion dq = Quaternion_rotate_y(r);
  rotate_around_point(t, point, dq);
}

void rotate_Transform_around_point_z(Transform *t, Vec3D point, float r) {
  Quaternion dq = Quaternion_rotate_z(r);
  rotate_around_point(t, point, dq);
}

void rotate_Transform_around_point_axis(Transform *t, Vec3D point, float r, Vec3D axis) {
  Vec3D axis_n = Vec3D_normalize(axis);

  Quaternion dq = Quaternion_rotate_axis(axis_n, r);
  rotate_around_point(t, point, dq);
}

Vec3D get_Transform_basis_x(Transform* t){
  return get_Quaternion_basis_x(t->rotation);
}
Vec3D get_Transform_basis_y(Transform* t){
  return get_Quaternion_basis_y(t->rotation);
}
Vec3D get_Transform_basis_z(Transform* t){
  return get_Quaternion_basis_y(t->rotation);
}
