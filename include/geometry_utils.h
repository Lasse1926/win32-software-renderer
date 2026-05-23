#pragma once
#include "vector_utils.h"

typedef struct Scene Scene ;
typedef struct Camera Camera;


typedef struct Vertex{
  Vec3D position;  
} Vertex;

typedef struct Triangle{
  Vertex vertices[3];
} Triangle;

typedef struct ClipTriangle{
    Vec4D vertices[3];
} ClipTriangle;

typedef struct ScreenTriangle{
  Vec2D vertices[3];
} ScreenTriangle;

typedef struct AABB{
  float x_max;
  float x_min;

  float y_max;
  float y_min;
}AABB;

AABB AABB_from_Triangle(Triangle a);
AABB AABB_from_ScreenTriangle(ScreenTriangle a);
Vec2D flaten_Vertex_Z(Vertex v);

ClipTriangle get_clip_from_trinagle(Camera *c, Triangle t, Scene *s);
ScreenTriangle ScreenTriangle_from_clipTriangle(Camera *c, ClipTriangle clip, Scene *s);

ScreenTriangle ScreenTriangle_Empty();
float line_determinant(Vec2D edge_start, Vec2D edge_end, Vec2D candidate);
