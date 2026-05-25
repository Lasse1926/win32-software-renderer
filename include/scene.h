#pragma once
#include "geometry_utils.h"
#include "transform.h"

typedef struct Model {
    Transform transform;
    Triangle* meshes;
    int mesh_length;
    int mesh_capacity;
} Model;

typedef struct Scene{
    Model* models;
    int model_length;
    int model_capacity;
} Scene ;


void Model_addTriangle(Model* m, Triangle t);
void Model_popTriangle(Model* m);
void Model_removeTriangle(Model* m, int i);

void Scene_addModel(Scene* s, Model m);
void Scene_popModel(Scene* s);
void Scene_removeModel(Scene* s, int i);
