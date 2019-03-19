#pragma once
#include "vector3.h"


struct _ray {
	Vector3 origin, versor;
};
typedef struct _ray Ray;