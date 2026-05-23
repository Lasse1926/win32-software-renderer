#pragma once
#include "math_utils.h"
#include "scene.h"
#include "transform.h"
#include <stdint.h>

typedef struct Camera {
  int screen_height;
  int screen_width;
  Transform transform;
  float fovY;
  float aspect;
  float z_near;
  float z_far;
  float focal_length;
  mat4 perspective_matrix;
}Camera;

Camera Camera_new(int screen_width,int screen_height,float fovY,float aspect,float near,float far);
void Camera_render(Camera* c,Scene* s, uint32_t* pixels, int width, int height);
mat4 build_view_matrix(Camera* c);
mat4 camera_projection_matrix(Camera* c);
