#pragma once
#include "vector3.h"

int current_ray_id = 0;

struct _ray {
	Vector3 origin, versor;
	int id;
};
typedef struct _ray Ray;

Ray createRay(Vector3 versor, Vector3 origin) {
	return { origin, versor, current_ray_id++ };
}