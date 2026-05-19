#include "geometry_utils.h"
#include "vector_utils.h"

AABB AABB_from_Triangle(Triangle a){
  AABB aabb;
  float xmin = MIN(a.vertices[0].position.x, a.vertices[1].position.x);
  float xmax = MAX(a.vertices[0].position.x, a.vertices[1].position.x);

  xmin = MIN(xmin, a.vertices[2].position.x);
  xmax = MAX(xmax, a.vertices[2].position.x);

  aabb.x_min = xmin;
  aabb.x_max = xmax;

  float ymin = MIN(a.vertices[0].position.y, a.vertices[1].position.y);
  float ymax = MAX(a.vertices[0].position.y, a.vertices[1].position.y);

  ymin = MIN(ymin, a.vertices[2].position.y);
  ymax = MAX(ymax, a.vertices[2].position.y);

  aabb.y_min = ymin;
  aabb.y_max = ymax;
  return aabb;
}

Vec2D flaten_Vertex_Z(Vertex v){
  return (Vec2D){
    .x = v.position.x,
    .y = v.position.y,
  };
}

float line_determinant(Vec2D edge_start, Vec2D edge_end, Vec2D candidate){

    Vec2D start_to_end = Vec2D_sub(edge_end,edge_start);

    Vec2D start_to_candidate = Vec2D_sub(candidate,edge_start);

    return start_to_end.y * start_to_candidate.x - start_to_end.x * start_to_candidate.y;
}

