#include "camera.h"
#include "math_utils.h"
#include "scene.h"
#include "transform.h"
#include <stdint.h>
#include <math.h>

Camera Camera_new(float fovY,float aspect,float near,float far){
  Camera c;
  c.transform = Transform_ZERO();
  c.fovY = fovY;
  c.aspect = aspect;
  c.near = near;
  c.far = far;

  c.focal_length = 1.0f/tan(fovY*0.5);

  c.perspective_matrix.m[0][0] = c.focal_length/c.aspect;
  c.perspective_matrix.m[1][1] = c.focal_length;
  c.perspective_matrix.m[2][2] = (c.far+c.near)/(c.near-c.far);
  c.perspective_matrix.m[3][2] = (2.0f*c.far*c.near)/(c.near-c.far);
  c.perspective_matrix.m[2][3] = (2.0f*c.far*c.near)/(c.near-c.far);

  return c;
}

void Camera_render(Camera *c, Scene *s, uint32_t *pixels, int width,
                   int height) {
  // calc Camera Perspective matrix (implement rotation in transform)
  // vec4 world = Model * vec4(localPos, 1);
  // vec4 view  = View  * world;
  // vec4 clip  = P     * view;
  // 
  // vec3 ndc;
  // ndc.x = clip.x / clip.w;
  // ndc.y = clip.y / clip.w;
  // ndc.z = clip.z / clip.w;
  //
  // vec2 screen;
  // screen.x = (ndc.x + 1.0f) * 0.5f * width;
  // screen.y = (1.0f - ndc.y) * 0.5f * height;
  // ndc.z is depth
  for (int i = 0; i < s->mesh_length; i++) {
    Triangle t = s->meshes[i];


    // Transform to screen triangle -> AABB from screen triangle

    ScreenTriangle st = ScreenTriangle_from_Triangle(c, t);

    AABB aabb = AABB_from_ScreenTriangle(st);
    

    for (int y = aabb.y_min; y <= aabb.y_max; y++) {
      for (int x = aabb.x_min; x <= aabb.x_max; x++) {
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
