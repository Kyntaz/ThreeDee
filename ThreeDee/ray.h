#pragma once
#include "vector3.h"


struct _ray {
	Vector3 origin, versor;
	bool inside;
};
typedef struct _ray Ray;