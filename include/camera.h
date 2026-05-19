#pragma once
#include "math_utils.h"
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
  mat4 perspective_matrix;
}Camera;

Camera Camera_new(float fovY,float aspect,float near,float far);
void Camera_render(Camera* c,Scene* s, uint32_t* pixels, int width, int height);
mat4 build_view_matrix(Vec3D camPos, mat4 camRot);
