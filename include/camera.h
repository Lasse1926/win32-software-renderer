#pragma once
#include "scene.h"
#include "transform.h"
#include <stdint.h>

typedef struct Camera {
  Transform transform;

}Camera;

void Camera_render(Camera* c,Scene* s, uint32_t* pixels, int width, int height);
