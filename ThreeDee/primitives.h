#pragma once

#include "ray.h"
#include "materialproperties.h"
#include <iostream>
struct _collision {};
typedef struct _collision Collision;

class Primitive {
public:
	Vector3 pos;
	MaterialProperties* matProps;
	Primitive(Vector3 pos,MaterialProperties* matProps) {
		pos = pos;
		matProps = matProps;
	}
	virtual Collision intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
public:
	float radius;

	Sphere(Vector3 pos, float r, MaterialProperties* matProps) : Primitive(pos,matProps), radius(r) {
		
	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
class Cilinder : public Primitive {
public:
	float baseRadius,apexRadius;
	Vector3 apexPos;

	Cilinder(Vector3 basePos, float baseR, Vector3 apexPos, float apexR, MaterialProperties* matProps) : 
		Primitive(basePos, matProps), apexPos(apexPos), baseRadius(baseR), apexRadius(apexR) {

	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
