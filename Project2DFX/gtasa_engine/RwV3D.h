#include "rw_types.h"

#ifndef _RWV3D
#define _RWV3D

typedef struct RwV3d RwV3d;

struct RwV3d
{
    RwReal x;   /**< X value */
    RwReal y;   /**< Y value */
    RwReal z;   /**< Z value */
};
typedef RwV3d RwV3D;

#endif // _RWV3D