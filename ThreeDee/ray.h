#pragma once
#include "vector3.h"
// TODO: Usar cenas do Afonso :3


struct _ray {
	Vector3 origin, versor;
	float maxDistance;
};
typedef struct _ray Ray;