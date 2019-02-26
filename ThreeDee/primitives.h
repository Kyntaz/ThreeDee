#pragma once

#include "ray.h"

struct _collision {};
typedef struct _collision Collision;

class Primitive {
	public:
		virtual Collision intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
	public:
	float radius;
	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
