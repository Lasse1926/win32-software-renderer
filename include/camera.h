#pragma once
#include "scene.h"
#include "transform.h"
#include <stdint.h>

typedef struct Camera {
  Transform transform;
  float fovY;
  float aspect;
  float near;
  float far;
  float focal_length;
  float perspective_matrix[4][4];
}Camera;

Camera Camera_new(float fovY,float aspect,float near,float far);
void Camera_render(Camera* c,Scene* s, uint32_t* pixels, int width, int height);
