#include "camera.h"
#include "geometry_utils.h"
#include "vector_utils.h"
#include <stdio.h>

void Camera_render(Camera *c, Scene *s, uint32_t *pixels, int width,
                   int height) {
  for (int i = 0; i < s->mesh_length; i++) {
    Triangle t = s->meshes[i];
    AABB aabb = AABB_from_Triangle(t);

    for (int y = aabb.y_min; y <= aabb.y_max; y++) {
      for (int x = aabb.x_min; x <= aabb.x_max; x++) {
        Vec2D p = Vec2D_XY(x, y);

        float w_0 = line_determinant(t.vertices[1], t.vertices[2], p);
        float w_1 = line_determinant(t.vertices[2], t.vertices[0], p);
        float w_2 = line_determinant(t.vertices[0], t.vertices[1], p);

        if (w_0 >= 0 && w_1 >= 0 && w_2 >= 0) {
          int index = y * width + x;
          pixels[index] = 0x002020FF;
        }
      }
    }
  }
}
