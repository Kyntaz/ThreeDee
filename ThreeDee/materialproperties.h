#pragma once
#include "colors.h"

//Holds material information.
struct _materialProperties {
	Color color;
	float diffuseComp, specularComp, shine, t, idxOfRefraction;
};
typedef struct _materialProperties MaterialProperties;