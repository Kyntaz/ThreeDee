#pragma once
#include "colors.h"

//Holds material information.
struct _materialProperties {
	Color color;
	float diffuseComp, specularComp, shine, t, idxOfRefraction;
};
typedef struct _materialProperties MaterialProperties;
void printMaterialProperties(MaterialProperties matProps) {
	//std::cout << "Material Properties: " << std::endl;
	//std::cout << "Color: " << matProps.color.r << ", " << matProps.color.g << ", " << matProps.color.b << std::endl;
	//std::cout << "Kd: " << matProps.diffuseComp << "; Ks: " << matProps.specularComp << "; ";
	//std::cout << "Shine:" << matProps.shine << "; Transm: " << matProps.t << "; idx Refraction: " << matProps.idxOfRefraction << std::endl;
}