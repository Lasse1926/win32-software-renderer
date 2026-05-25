#include "camera.h"
#include "geometry_utils.h"
#include "math_utils.h"
#include "scene.h"
#include "transform.h"
#include "vector_utils.h"
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int clamp_int(int v, int lo, int hi) {
  if (v < lo)
    return lo;
  if (v > hi)
    return hi;
  return v;
}

Camera Camera_new(int screen_width, int screen_height, float fovY, float aspect,
                  float z_near, float z_far) {
  Camera c = {0};
  c.transform = Transform_ZERO();
  c.screen_width = screen_width;
  c.screen_height = screen_height;
  c.fovY = fovY;
  c.aspect = aspect;
  c.z_near = z_near;
  c.z_far = z_far;

  c.focal_length = 1.0f / tan(fovY * 0.5);

  c.perspective_matrix.m[0][0] = c.focal_length / c.aspect;
  c.perspective_matrix.m[1][1] = c.focal_length;
  c.perspective_matrix.m[2][2] = (c.z_far + c.z_near) / (c.z_near - c.z_far);
  c.perspective_matrix.m[2][3] =
      (2.0f * c.z_far * c.z_near) / (c.z_near - c.z_far);
  c.perspective_matrix.m[3][2] = -1.0f;

  return c;
}

void Camera_render(Camera *c, Scene *s, uint32_t *pixels, int width, int height,
                   float *db) {
  memset(pixels, 0, width * height * sizeof(uint32_t));
  float *end = db + width * height;
  for (float *p = db; p < end; p++) {
      *p = FLT_MAX;
  }

  for (int m_index = 0; m_index < s->model_length; m_index++) {
    Model *m = &s->models[m_index];
    for (int i = 0; i < m->mesh_length; i++) {
      Triangle t = m->meshes[i];

      ClipTriangle ct = get_clip_from_trinagle(c, t,m);

      int behind = 0;

      for (int i = 0; i < 3; i++) {
        if (ct.vertices[i].w <= 0.0f)
          behind++;
      }

      if (behind >= 1) {
        continue;
      }

      ScreenTriangle st = ScreenTriangle_from_clipTriangle(c, ct);

      AABB aabb = AABB_from_ScreenTriangle(st);

      int x0 = clamp_int((int)aabb.x_min, 0, width - 1);
      int x1 = clamp_int((int)aabb.x_max, 0, width - 1);
      int y0 = clamp_int((int)aabb.y_min, 0, height - 1);
      int y1 = clamp_int((int)aabb.y_max, 0, height - 1);

      for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
          int index = y * width + x;
          Vec2D p = Vec2D_XY(x, y);
          Vec3D depths =
              Vec3D_XYZ(st.vertices[0].z, st.vertices[1].z, st.vertices[2].z);
          Vec3D weights = get_screenTriangle_weights(p, st);



          float w_0 = line_determinant(Vec2D_from_Vec3D_XY(st.vertices[1]),
                                       Vec2D_from_Vec3D_XY(st.vertices[2]), p);
          float w_1 = line_determinant(Vec2D_from_Vec3D_XY(st.vertices[2]),
                                       Vec2D_from_Vec3D_XY(st.vertices[0]), p);
          float w_2 = line_determinant(Vec2D_from_Vec3D_XY(st.vertices[0]),
                                       Vec2D_from_Vec3D_XY(st.vertices[1]), p);

          if (w_0 >= 0 && w_1 >= 0 && w_2 >= 0) {
            float depth = Vec3D_dot(depths, weights);

            if (depth > db[index]) {
              continue;
            }
            db[index] = depth;
            pixels[index] = st.color;
          }
        }
      }
    }
  }
}

mat4 build_view_matrix(Camera *c) {
  mat4 camRot = mat4_from_mat3(c->transform.rotation);
  mat4 R_inv = mat4_transpose(camRot);
  mat4 T_inv = mat4_translate_inverse(c->transform.position);
  return mat4_multiply(R_inv, T_inv);
}

mat4 camera_projection_matrix(Camera *c) {
  float f = 1.0f / tanf(c->fovY * 0.5f);

  mat4 P = mat4_identity();

  P.m[0][0] = f / c->aspect;
  P.m[1][1] = f;

  P.m[2][2] = (c->z_far + c->z_near) / (c->z_near - c->z_far);
  P.m[2][3] = (2.0f * c->z_far * c->z_near) / (c->z_near - c->z_far);

  P.m[3][2] = -1.0f;
  P.m[3][3] = 0.0f;

  return P;
}
