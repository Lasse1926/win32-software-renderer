#include "vector_utils.h"
#include "math_utils.h"
#include <math.h>

Vec3D Vec3D_XYZ(float x, float y, float z) {
  return (Vec3D){.x = x, .y = y, .z = z};
}
Vec3D Vec3D_ZERO() {
  return (Vec3D){
      .x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
  };
}

float Vec3D_dist(Vec3D a, Vec3D b) {
  Vec3D a_to_b = Vec3D_sub(b, a);
  return Vec3D_len(a_to_b);
}

float Vec3D_dist_sq(Vec3D a, Vec3D b) {
  Vec3D a_to_b = Vec3D_sub(b, a);
  return Vec3D_len_sq(a_to_b);
}

Vec3D Vec3D_add(Vec3D a, Vec3D b) {
  return (Vec3D){
      .x = b.x + a.x,
      .y = b.y + a.y,
      .z = b.z + a.z,
  };
};
Vec3D Vec3D_sub(Vec3D a, Vec3D b) {
  return (Vec3D){
      .x = a.x - b.x,
      .y = a.y - b.y,
      .z = a.z - b.z,
  };
}
Vec3D Vec3D_scale(Vec3D a, float scalar) {
  return (Vec3D){
      .x = a.x * scalar,
      .y = a.y * scalar,
      .z = a.z * scalar,
  };
}

float Vec3D_len(Vec3D a) { return sqrtf(a.z * a.z + a.y * a.y + a.x * a.x); }

float Vec3D_len_sq(Vec3D a) { return a.z * a.z + a.y * a.y + a.x * a.x; }

Vec3D Vec3D_normalize(Vec3D a) {
  float len_sq = Vec3D_len_sq(a);

  if (len_sq == 0.0f)
    return a;

  float inv_len = Q_rsqrt(len_sq);

  return (Vec3D){
      .x = a.x * inv_len,
      .y = a.y * inv_len,
      .z = a.z * inv_len,
  };
}
Vec3D Vec3_cross(Vec3D a, Vec3D b)
{
    Vec3D r;

    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;

    return r;
}

// Vec2D ----------------------

Vec2D Vec2D_XY(float x, float y) {
  return (Vec2D){
      .x = x,
      .y = y,
  };
}
Vec2D Vec2D_ZERO() {
  return (Vec2D){
      .x = 0.0f,
      .y = 0.0f,
  };
}

float Vec2D_dist(Vec2D a, Vec2D b) {
  Vec2D a_to_b = Vec2D_sub(b, a);
  return Vec2D_len(a_to_b);
}

float Vec2D_dist_sq(Vec2D a, Vec2D b) {
  Vec2D a_to_b = Vec2D_sub(b, a);
  return Vec2D_len_sq(a_to_b);
}

Vec2D Vec2D_add(Vec2D a, Vec2D b) {
  return (Vec2D){
      .x = b.x + a.x,
      .y = b.y + a.y,
  };
};
Vec2D Vec2D_sub(Vec2D a, Vec2D b) {
  return (Vec2D){
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
}
Vec2D Vec2D_scale(Vec2D a, float scalar) {
  return (Vec2D){
      .x = a.x * scalar,
      .y = a.y * scalar,
  };
}

float Vec2D_len(Vec2D a) { return sqrtf(a.y * a.y + a.x * a.x); }

float Vec2D_len_sq(Vec2D a) { return a.y * a.y + a.x * a.x; }

Vec2D Vec2D_normalize(Vec2D a) {
  float len_sq = Vec2D_len_sq(a);

  if (len_sq == 0.0f)
    return a;

  float inv_len = Q_rsqrt(len_sq);

  return (Vec2D){
      .x = a.x * inv_len,
      .y = a.y * inv_len,
  };
}

Vec4D Vec4D_from_Vec3D(Vec3D v, float w) {
  return (Vec4D){.x = v.x, .y = v.y, .z = v.z, .w = w};
}
Vec4D Vec4D_multiply(Vec4D a, Vec4D b) {
  Vec4D result;
  result.x = a.x * b.x;
  result.y = a.y * b.y;
  result.z = a.z * b.z;
  result.w = a.w * b.w;
  return result;
}
