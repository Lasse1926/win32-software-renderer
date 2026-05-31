#pragma once
#include "vector_utils.h"
#include <stdint.h>

typedef struct Model Model;
typedef struct Camera Camera;

typedef struct Vertex {
  Vec3D position;
} Vertex;

typedef struct Triangle {
  Vertex vertices[3];
  uint32_t color;
} Triangle;

typedef struct ClipTriangle {
  Vec4D vertices[3];
  uint32_t color;
} ClipTriangle;

typedef struct ScreenTriangle {
  Vec3D vertices[3];
  uint32_t color;
} ScreenTriangle;

typedef struct AABB {
  float x_max;
  float x_min;

  float y_max;
  float y_min;
} AABB;

typedef struct Plane {
    Vec3D normal;
    float d;
} Plane;

AABB AABB_from_Triangle(Triangle a);
AABB AABB_from_ScreenTriangle(ScreenTriangle a);
Vec2D flaten_Vertex_Z(Vertex v);

ClipTriangle get_clip_from_trinagle(Camera *c, Triangle t, Model *m);
ScreenTriangle ScreenTriangle_from_clipTriangle(Camera *c, ClipTriangle clip);

ScreenTriangle ScreenTriangle_Empty();
ScreenTriangle ScreenTriangle_Vec3(Vec3D a,Vec3D b,Vec3D c);
float line_determinant(Vec2D edge_start, Vec2D edge_end, Vec2D candidate);
float signed_triangle_area(ScreenTriangle t);
Vec3D get_screenTriangle_weights(Vec2D p, ScreenTriangle t);
void scale_Triangle(Triangle *t,Vec3D origin, Vec3D scale);
Plane plane_from_point_normal(Vec3D point, Vec3D normal);
Plane plane_from_Vec4(Vec4D v);
