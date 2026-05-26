#include "quaternion.h"
#include "math_utils.h"
#include <math.h>

Quaternion Quaternion_identity() {
  return (Quaternion){1.0f, 0.0f, 0.0f, 0.0f};
}

void Quaternion_normalized(Quaternion *q) {
  float sum = q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;

  float inv_len = Q_rsqrt(sum);

  q->w *= inv_len;
  q->x *= inv_len;
  q->y *= inv_len;
  q->z *= inv_len;
}

Quaternion Quaternion_rotate_x(float r) {
  float half = r * 0.5f;

  Quaternion q;
  q.w = cosf(half);
  q.x = sinf(half);
  q.y = 0.0f;
  q.z = 0.0f;

  return q;
}
Quaternion Quaternion_rotate_y(float r) {
  float half = r * 0.5f;

  Quaternion q;
  q.w = cosf(half);
  q.x = 0.0f;
  q.y = sinf(half);
  q.z = 0.0f;

  return q;
}
Quaternion Quaternion_rotate_z(float r) {
  float half = r * 0.5f;

  Quaternion q;
  q.w = cosf(half);
  q.x = 0.0f;
  q.y = 0.0f;
  q.z = sinf(half);

  return q;
}

Quaternion Quaternion_rotate_axis(Vec3D axis, float r) {
  axis = Vec3D_normalize(axis);

  float half = r * 0.5f;
  float s = sinf(half);
  float c = cosf(half);

  Quaternion q;

  q.w = c;
  q.x = axis.x * s;
  q.y = axis.y * s;
  q.z = axis.z * s;

  return q;
}

Quaternion Quaternion_mult(Quaternion a, Quaternion b) {
  Quaternion r;

  r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

  r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;

  r.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;

  r.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;

  return r;
}

mat3 mat3_from_Quaternion(Quaternion q) {
    mat3 m;

    float x = q.x;
    float y = q.y;
    float z = q.z;
    float w = q.w;

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;

    float xy = x * y;
    float xz = x * z;
    float yz = y * z;

    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    m.m[0][0] = 1.0f - 2.0f * (yy + zz);
    m.m[0][1] = 2.0f * (xy - wz);
    m.m[0][2] = 2.0f * (xz + wy);

    m.m[1][0] = 2.0f * (xy + wz);
    m.m[1][1] = 1.0f - 2.0f * (xx + zz);
    m.m[1][2] = 2.0f * (yz - wx);

    m.m[2][0] = 2.0f * (xz - wy);
    m.m[2][1] = 2.0f * (yz + wx);
    m.m[2][2] = 1.0f - 2.0f * (xx + yy);

    return m;
}

Vec3D get_Quaternion_basis_z(Quaternion q) {
    Vec3D z;

    z.x = -2.0f * (q.x*q.z - q.w*q.y);
    z.y = -2.0f * (q.y*q.z + q.w*q.x);
    z.z = -(1.0f - 2.0f * (q.x*q.x + q.y*q.y));

    return z;
}
Vec3D get_Quaternion_basis_x(Quaternion q) {
    Vec3D x;

    x.x = 1.0f - 2.0f * (q.y*q.y + q.z*q.z);
    x.y = 2.0f * (q.x*q.y + q.w*q.z);
    x.z = 2.0f * (q.x*q.z - q.w*q.y);

    return x;
}
Vec3D get_Quaternion_basis_y(Quaternion q) {
    Vec3D y;

    y.x = 2.0f * (q.x*q.y - q.w*q.z);
    y.y = 1.0f - 2.0f * (q.x*q.x + q.z*q.z);
    y.z = 2.0f * (q.y*q.z + q.w*q.x);

    return y;
}
