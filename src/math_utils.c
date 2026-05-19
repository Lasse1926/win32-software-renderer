#include "math_utils.h"

float Q_rsqrt(float number) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y = number;
  i = *(long *)&y;           // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1); // what the fuck?
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y)); // 1st iteration
  //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can
  //be removed

  return y;
}

Vec4D mul_mat4_vec4(mat4 M, Vec4D v) {
  Vec4D r;
  r.x = M.m[0][0] * v.x + M.m[0][1] * v.y + M.m[0][2] * v.z + M.m[0][3] * v.w;
  r.y = M.m[1][0] * v.x + M.m[1][1] * v.y + M.m[1][2] * v.z + M.m[1][3] * v.w;
  r.z = M.m[2][0] * v.x + M.m[2][1] * v.y + M.m[2][2] * v.z + M.m[2][3] * v.w;
  r.w = M.m[3][0] * v.x + M.m[3][1] * v.y + M.m[3][2] * v.z + M.m[3][3] * v.w;
  return r;
}

mat4 mat4_identity() {
  mat4 M = {0};

  M.m[0][0] = 1;
  M.m[1][1] = 1;
  M.m[2][2] = 1;
  M.m[3][3] = 1;

  return M;
}

mat4 mat4_translate_inverse(Vec3D p) {
  mat4 M = mat4_identity();
  M.m[0][3] = -p.x;
  M.m[1][3] = -p.y;
  M.m[2][3] = -p.z;
  return M;
}
mat4 mat4_transpose(mat4 M) {
  mat4 T;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      T.m[i][j] = M.m[j][i];
    }
  }
  return T;
}

mat4 mat4_multiply(mat4 A, mat4 B) {
  mat4 C = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0.0f;
      for (int k = 0; k < 4; k++) {
        sum += A.m[i][k] * B.m[k][j];
      }
      C.m[i][j] = sum;
    }
  }

  return C;
}
