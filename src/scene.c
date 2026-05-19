#include "scene.h"
#include <stdlib.h>

void Scene_addTriangle(Scene* s, Triangle t)
{
    if (s->mesh_length >= s->mesh_capacity)
    {
        s->mesh_capacity *= 2;
        s->meshes = realloc(s->meshes, sizeof(Triangle) * s->mesh_capacity);
    }

    s->meshes[s->mesh_length++] = t;
}

void Scene_popTriangle(Scene* s)
{
    if (s->mesh_length > 0)
        s->mesh_length--;
}

void Scene_removeTriangle(Scene* s, int i)
{
    s->meshes[i] = s->meshes[s->mesh_length - 1];
    s->mesh_length--;
}
