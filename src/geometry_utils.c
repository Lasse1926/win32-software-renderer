#include "geometry_utils.h"
#include "camera.h"

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

AABB AABB_from_ScreenTriangle(ScreenTriangle a){
  AABB aabb;
  float xmin = MIN(a.vertices[0].x, a.vertices[1].x);
  float xmax = MAX(a.vertices[0].x, a.vertices[1].x);

  xmin = MIN(xmin, a.vertices[2].x);
  xmax = MAX(xmax, a.vertices[2].x);

  aabb.x_min = xmin;
  aabb.x_max = xmax;

  float ymin = MIN(a.vertices[0].y, a.vertices[1].y);
  float ymax = MAX(a.vertices[0].y, a.vertices[1].y);

  ymin = MIN(ymin, a.vertices[2].y);
  ymax = MAX(ymax, a.vertices[2].y);

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

ScreenTriangle ScreenTriangle_from_Triangle(Camera* c,Triangle t){
  for(int i = 0; i < 3;i++){
    // Vec4D world = Model * vec4(localPos, 1); If adding local and world space
    mat4 cam_view = build_view_matrix(c->transform.position,rotation_from_forward(c->transform.forward_vec));
    Vec4D view  = mul_mat4_vec4(cam_view,Vec4D_from_Vec3D(t.vertices[i].position,1.0f));
    Vec4D clip  = mul_mat4_vec4(c->perspective_matrix,view);
  }
}

