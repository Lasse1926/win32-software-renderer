#include "camera.h"
#include "math_utils.h"
#include "scene.h"
#include "transform.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

Camera Camera_new(int screen_width,int screen_height,float fovY,float aspect,float z_near,float z_far){
  Camera c = {0};
  c.transform = Transform_ZERO();
  c.screen_width = screen_width;
  c.screen_height = screen_height;
  c.fovY = fovY;
  c.aspect = aspect;
  c.z_near = z_near;
  c.z_far = z_far;

  c.focal_length = 1.0f/tan(fovY*0.5);

  c.perspective_matrix.m[0][0] = c.focal_length/c.aspect;
  c.perspective_matrix.m[1][1] = c.focal_length;
  c.perspective_matrix.m[2][2] = (c.z_far+c.z_near)/(c.z_near-c.z_far);
  c.perspective_matrix.m[2][3] = (2.0f*c.z_far*c.z_near)/(c.z_near-c.z_far);
  c.perspective_matrix.m[3][2] = -1.0f;

  return c;
}

void Camera_render(Camera *c, Scene *s, uint32_t *pixels, int width,
                   int height) {
  for (int i = 0; i < s->mesh_length; i++) {
    Triangle t = s->meshes[i];

    ScreenTriangle st = ScreenTriangle_from_Triangle(c, t);

    AABB aabb = AABB_from_ScreenTriangle(st);
    

    int x0 = clamp_int((int)aabb.x_min, 0, width  - 1);
    int x1 = clamp_int((int)aabb.x_max, 0, width  - 1);
    int y0 = clamp_int((int)aabb.y_min, 0, height - 1);
    int y1 = clamp_int((int)aabb.y_max, 0, height - 1);
    memset(pixels, 0, width * height * sizeof(uint32_t));
    for (int y = y0; y <= y1; y++) {
      for (int x = x0; x <= x1; x++) {
        Vec2D p = Vec2D_XY(x, y);
        float w_0 = line_determinant(st.vertices[1], st.vertices[2], p);
        float w_1 = line_determinant(st.vertices[2], st.vertices[0], p);
        float w_2 = line_determinant(st.vertices[0], st.vertices[1], p);

        if (w_0 >= 0 && w_1 >= 0 && w_2 >= 0) {
          int index = y * width + x;
          pixels[index] = 0x002020FF;
        }
      }
    }
  }
}

mat4 build_view_matrix(Vec3D camPos, mat4 camRot)
{
    mat4 R_inv = mat4_transpose(camRot);
    mat4 T_inv = mat4_translate_inverse(camPos);
    return mat4_multiply(R_inv, T_inv);
}
