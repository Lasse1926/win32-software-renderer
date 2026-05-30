#include "camera.h"
#include "geometry_utils.h"
#include "math_utils.h"
#include "quaternion.h"
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

  for (int i = 0; i < width * height; i++) {
    db[i] = FLT_MAX;
  }

  for (int m_index = 0; m_index < s->model_length; m_index++) {
    Model *m = &s->models[m_index];

    for (int i = 0; i < m->mesh_length; i++) {

      Triangle t = m->meshes[i];


      ClipTriangle ct = get_clip_from_trinagle(c, t, m);

      int behind = 0;
      for (int v = 0; v < 3; v++) {
        if (ct.vertices[v].w <= 0.0f)
          behind++;
      }
      if (behind >= 1) continue;

      ScreenTriangle st = ScreenTriangle_from_clipTriangle(c, ct);

      Vec2D v0 = Vec2D_from_Vec3D_XY(st.vertices[0]);
      Vec2D v1 = Vec2D_from_Vec3D_XY(st.vertices[1]);
      Vec2D v2 = Vec2D_from_Vec3D_XY(st.vertices[2]);

      float z0 = st.vertices[0].z;
      float z1 = st.vertices[1].z;
      float z2 = st.vertices[2].z;

      AABB aabb = AABB_from_ScreenTriangle(st);

      int x0 = clamp_int((int)aabb.x_min, 0, width - 1);
      int x1 = clamp_int((int)aabb.x_max, 0, width - 1);
      int y0 = clamp_int((int)aabb.y_min, 0, height - 1);
      int y1 = clamp_int((int)aabb.y_max, 0, height - 1);

      float area = line_determinant(v0, v1, v2);
      float invArea = 1.0f / (area);

      // edge coefficients (step amounts when moving right/down)
      float A0 = (v2.y - v1.y) ;
      float B0 = (v1.x - v2.x) ;

      float A1 = (v0.y - v2.y) ;
      float B1 = (v2.x - v0.x) ;

      float A2 = (v1.y - v0.y) ;
      float B2 = (v0.x - v1.x) ;

      // robust initialization using SAME edge function
      Vec2D p0 = Vec2D_XY(x0, y0);

      float w0_row = line_determinant(v1, v2, p0) ;
      float w1_row = line_determinant(v2, v0, p0) ;
      float w2_row = line_determinant(v0, v1, p0) ;

      for (int y = y0; y <= y1; y++) {

        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;

        for (int x = x0; x <= x1; x++) {

          int index = y * width + x;

          // UNIFIED inside test (no sign ambiguity anymore)
          if (w0 >= 0 && w1 >= 0 && w2 >= 0) {

            float b0 = w0 * invArea;
            float b1 = w1 * invArea;
            float b2 = w2 * invArea;

            float depth = z0 * b0 + z1 * b1 + z2 * b2;

            if (depth <= db[index]) {
              db[index] = depth;
              pixels[index] = st.color;
            }
          }

          w0 += A0;
          w1 += A1;
          w2 += A2;
        }

        w0_row += B0;
        w1_row += B1;
        w2_row += B2;
      }
    }
  }
}

mat4 build_view_matrix(Camera *c) {
  mat4 camRot = mat4_from_mat3(mat3_from_Quaternion(c->transform.rotation));
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
