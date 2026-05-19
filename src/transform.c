#include "transform.h"
#include "vector_utils.h"

Transform Transform_ZERO()
{
    return (Transform){
        .position = Vec3D_ZERO(),
        .forward_vec = Vec3D_XYZ(0.0f,0.0f,1.0f),
    };
}

mat4 rotation_from_forward(Vec3D forward)
{
    Vec3D worldUp = {0, 1, 0};

    forward = Vec3D_normalize(forward);

    Vec3D right = Vec3D_normalize(Vec3_cross(worldUp, forward));
    Vec3D up    = Vec3_cross(forward, right);

    mat4 m = {0};

    m.m[0][0] = right.x;
    m.m[0][1] = right.y;
    m.m[0][2] = right.z;
    m.m[0][3] = 0;

    m.m[1][0] = up.x;
    m.m[1][1] = up.y;
    m.m[1][2] = up.z;
    m.m[1][3] = 0;

    m.m[2][0] = -forward.x;
    m.m[2][1] = -forward.y;
    m.m[2][2] = -forward.z;
    m.m[2][3] = 0;

    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;

    return m;
}
