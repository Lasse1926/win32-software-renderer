#include "file_utils.h"
#include <string.h>
int has_obj_extension(const char *path)
{
    const char *ext = strrchr(path, '.');

    if (ext == NULL)
        return 0;

    return strcmp(ext, ".obj") == 0;
}
