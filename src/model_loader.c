#include "model_loader.h"
#include "colors.h"
#include "file_utils.h"
#include "geometry_utils.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_Model_from_Obj(Model *m, char *path) {
  char ch;

  FILE *fptr = NULL;

  errno_t err = fopen_s(&fptr, path, "r");

  if (err != 0 || fptr == NULL || !has_obj_extension(path)) {
    return;
  }

  char line[256];

  Vertex *vertices = NULL;
  size_t vertexCount = 0;
  size_t vertexCapacity = 0;

  while (fgets(line, sizeof(line), fptr)) {

    char *context = NULL;
    char *token = strtok_s(line, " \t", &context);

    if (!token) {
      continue;
    }

    switch (token[0]) {
    case 'v': {

      if (token[1] != '\0') {
        break; // only "v"
      }

      char *xStr = strtok_s(NULL, " \t", &context);
      char *yStr = strtok_s(NULL, " \t", &context);
      char *zStr = strtok_s(NULL, " \t", &context);

      if (!xStr || !yStr || !zStr){
        break;
      }

      Vertex v = {0};

      v.position.x = atof(xStr);
      v.position.y = atof(yStr);
      v.position.z = atof(zStr);

      if (vertexCount >= vertexCapacity) {
        vertexCapacity = vertexCapacity == 0 ? 1024 : vertexCapacity * 2;

        Vertex *tmp = realloc(vertices, vertexCapacity * sizeof(Vertex));
        if (!tmp)
          break;

        vertices = tmp;
      }

      vertices[vertexCount++] = v;
      break;
    }

    case 'f': {
      if (token[1] != '\0')

        break; // only "f"

      char *xIndexStr = strtok_s(NULL, " \t", &context);
      char *yIndexStr = strtok_s(NULL, " \t", &context);
      char *zIndexStr = strtok_s(NULL, " \t", &context);

      if (!xIndexStr || !yIndexStr || !zIndexStr)
        break;

      char *end;

      size_t i0 = (size_t)strtoul(xIndexStr, &end, 10);
      size_t i1 = (size_t)strtoul(yIndexStr, &end, 10);
      size_t i2 = (size_t)strtoul(zIndexStr, &end, 10);

      if (i0 == 0 || i1 == 0 || i2 == 0)
        break;

      i0--;
      i1--;
      i2--;

      if (i0 >= vertexCount || i1 >= vertexCount || i2 >= vertexCount)
        break;

      Triangle t = {0};

      t.vertices[0] = vertices[i0];
      t.vertices[1] = vertices[i1];
      t.vertices[2] = vertices[i2];

      t.color = BLUE;

      Model_addTriangle(m, t);

      break;
    }

    case 'o':
      break;

    case 'g':
      break;

    case '#':
      break;

    default:
      break;
    }
  }

  fclose(fptr);

  free(vertices);
}
