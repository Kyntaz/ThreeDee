#pragma once
#include "vector3.h"
#include "colors.h"
struct _positionallight {
	Vector3 pos;
	Color color;
};
typedef struct _positionallight PositionalLight;