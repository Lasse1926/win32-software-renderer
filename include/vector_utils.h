#pragma once

typedef struct Vec3D {
  float x;
  float y;
  float z;
}Vec3D;

Vec3D Vec3D_XYZ(float x, float y,float z);
Vec3D Vec3D_ZERO();
float Vec3D_dist(Vec3D a,Vec3D b);
float Vec3D_dist_sq(Vec3D a,Vec3D b);
Vec3D Vec3D_add(Vec3D a,Vec3D b);
Vec3D Vec3D_sub(Vec3D a,Vec3D b);
Vec3D Vec3D_scale(Vec3D a,float scalar);
float Vec3D_len(Vec3D a);
float Vec3D_len_sq(Vec3D a);
Vec3D Vec3D_normalize(Vec3D a);

typedef struct Vec2D {
  float x;
  float y;
}Vec2D;

Vec2D Vec2D_XY(float x, float y);
Vec2D Vec2D_ZERO();
float Vec2D_dist(Vec2D a,Vec2D b);
float Vec2D_dist_sq(Vec2D a,Vec2D b);
Vec2D Vec2D_add(Vec2D a,Vec2D b);
Vec2D Vec2D_sub(Vec2D a,Vec2D b);
Vec2D Vec2D_scale(Vec2D a,float scalar);
float Vec2D_len(Vec2D a);
float Vec2D_len_sq(Vec2D a);
Vec2D Vec2D_normalize(Vec2D a);
