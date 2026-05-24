#include "geometry_utils.h"
#include "camera.h"
#include "math_utils.h"
#include "scene.h"
#include "transform.h"
#include "vector_utils.h"

AABB AABB_from_Triangle(Triangle a) {
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

AABB AABB_from_ScreenTriangle(ScreenTriangle a) {
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

Vec2D flaten_Vertex_Z(Vertex v) {
  return (Vec2D){
      .x = v.position.x,
      .y = v.position.y,
  };
}

ScreenTriangle ScreenTriangle_Empty() {
  ScreenTriangle st = {
      .vertices = {Vec3D_ZERO(), Vec3D_ZERO(), Vec3D_ZERO()},
  };
  return st;
}

ScreenTriangle ScreenTriangle_Vec3(Vec3D a,Vec3D b,Vec3D c){
  ScreenTriangle st = {
    .vertices = {a,b,c},
    .color = 0x00FFFFFF,
  };
  return st;
}

float line_determinant(Vec2D edge_start, Vec2D edge_end, Vec2D candidate) {

  Vec2D start_to_end = Vec2D_sub(edge_end, edge_start);

  Vec2D start_to_candidate = Vec2D_sub(candidate, edge_start);

  return start_to_end.y * start_to_candidate.x -
         start_to_end.x * start_to_candidate.y;
}

float singed_triangle_area(ScreenTriangle t){
  Vec2D a = Vec2D_from_Vec3D_XY(t.vertices[0]);
  Vec2D b = Vec2D_from_Vec3D_XY(t.vertices[1]);
  Vec2D c = Vec2D_from_Vec3D_XY(t.vertices[2]);
  Vec2D a_c = Vec2D_sub(c,a);
  Vec2D abPerp = Vec2D_perpendicular(Vec2D_sub(b,a));
  return Vec2D_dot(a_c,abPerp)/2.0f;
}

ClipTriangle get_clip_from_trinagle(Camera *c, Triangle t, Scene *s){
  mat4 Model = Transform_to_Model_mat4(s->transform);
  mat4 View = build_view_matrix(c);
  mat4 Proj = c->perspective_matrix;

  mat4 MVP = mat4_multiply(Proj, mat4_multiply(View, Model));

  ClipTriangle clip;

  for (int i = 0; i < 3; i++) {
    clip.vertices[i] = mul_mat4_vec4(MVP, Vec4D_from_Vec3D(t.vertices[i].position, 1));
  }
  clip.color = t.color;
  return clip;
}

ScreenTriangle ScreenTriangle_from_clipTriangle(Camera *c, ClipTriangle clip, Scene *s){
  ScreenTriangle st;

  for (int i = 0; i < 3; i++) {
    Vec3D ndc;
    ndc.x = clip.vertices[i].x / clip.vertices[i].w;
    ndc.y = clip.vertices[i].y / clip.vertices[i].w;
    ndc.z = clip.vertices[i].z / clip.vertices[i].w;

    Vec3D screen;
    screen.x = (ndc.x + 1.0f) * 0.5f * c->screen_width;
    screen.y = (1.0f - ndc.y) * 0.5f * c->screen_height;
    screen.z = ndc.z;

    st.vertices[i] = screen;
  }
  st.color = clip.color;

  return st;
}

Vec3D get_screenTriangle_weights(Vec2D p, ScreenTriangle t){
  Vec3D a = t.vertices[0];
  Vec3D b = t.vertices[0];
  Vec3D c = t.vertices[0];
  Vec3D point = Vec3D_XYZ(p.x,p.y,1.0f);

  float areaABP = singed_triangle_area(ScreenTriangle_Vec3(a,b,point));
  float areaBCP = singed_triangle_area(ScreenTriangle_Vec3(b,c,point));
  float areaCAP = singed_triangle_area(ScreenTriangle_Vec3(c,a,point));

  float invAreaSum = 1.0f/(areaABP+areaBCP+areaCAP);

  Vec3D weights = Vec3D_XYZ(areaBCP*invAreaSum,areaCAP*invAreaSum,areaABP*invAreaSum);
  return weights;

}
