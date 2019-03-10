#pragma once
#include "colors.h"
struct _materialProperties {
	Color color;
	float diffuseComp, specularComp, shine, t, idxOfRefraction;
};
typedef _struct _materialProperties MaterialProperties;