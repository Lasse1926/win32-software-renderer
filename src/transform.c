#include "transform.h"

Transform Transform_ZERO()
{
    return (Transform){
        .position = Vec3D_ZERO(),
        .forward_vec = Vec3D_ZERO(),
    };
}
