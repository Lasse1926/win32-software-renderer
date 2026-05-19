#pragma once
#include "vector_utils.h"


typedef struct Vertex{
  Vec3D position;  
} Vertex;

typedef struct Triangle{
  Vertex vertices[3];
} Triangle;



typedef struct AABB{
  float x_max;
  float x_min;

  float y_max;
  float y_min;
}AABB;

AABB AABB_from_Triangle(Triangle a);
Vec2D flaten_Vertex_Z(Vertex v);
float line_determinant(Vec2D edge_start, Vec2D edge_end, Vec2D candidate);
