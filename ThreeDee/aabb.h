#pragma once

#include "vector3.h"

struct _AABB {
	Vector3 pmin, pmax;
};
typedef struct _AABB AABB;