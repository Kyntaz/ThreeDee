#pragma once

#include "ray.h"
#include <iostream>
struct _collision {};
typedef struct _collision Collision;

class Primitive {
public:
	Vector3 pos;
	Primitive(Vector3 pos) {
		pos = pos;
	}
	virtual Collision intersect(Ray ray) = 0;
	Color color;
	float kd, ks, shine, transm, idxRefr;
};

class Sphere : public Primitive {
public:
	float radius;

	Sphere(Vector3 pos, float r) : Primitive(pos), radius(r) {
		
	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
