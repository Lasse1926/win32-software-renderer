#pragma once
#include "geometry_utils.h"

typedef struct Scene {
    Triangle* meshes;
    int mesh_length;
    int mesh_capacity;
} Scene;

void Scene_addTriangle(Scene* s, Triangle t);
void Scene_popTriangle(Scene* s);
void Scene_removeTriangle(Scene* s, int i);
