#include "scene.h"
#include <stdlib.h>

void Model_addTriangle(Model* m, Triangle t)
{
    if (m->mesh_length >= m->mesh_capacity)
    {
        m->mesh_capacity *= 2;
        m->meshes = realloc(m->meshes, sizeof(Triangle) * m->mesh_capacity);
    }

    m->meshes[m->mesh_length++] = t;
}

void Model_popTriangle(Model* m)
{
    if (m->mesh_length > 0)
        m->mesh_length--;
}

void Model_removeTriangle(Model* m, int i)
{
    m->meshes[i] = m->meshes[m->mesh_length - 1];
    m->mesh_length--;
}


void Scene_addModel(Scene* s, Model m)
{
    if (s->model_length >= s->model_capacity)
    {
        s->model_capacity =
            (s->model_capacity == 0) ? 4 : s->model_capacity * 2;

        s->models = realloc(
            s->models,
            sizeof(Model) * s->model_capacity
        );
    }

    s->models[s->model_length++] = m;
}

void Scene_popModel(Scene* s)
{
    if (s->model_length > 0)
        s->model_length--;
}

void Scene_removeModel(Scene* s, int i)
{
    if (i < 0 || i >= s->model_length)
        return;

    s->models[i] = s->models[s->model_length - 1];
    s->model_length--;
}
