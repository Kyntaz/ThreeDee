#pragma once

#include "ray.h"
#include <iostream>
struct _collision {};
typedef struct _collision Collision;

class Primitive {
public:
	float posx;
	float posy;
	float posz;
	Primitive(float x, float y, float z) {
		posx = x;
		posy = y;
		posz = z;
	}
	virtual Collision intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
public:
	float radius;

	Sphere(float x, float y, float z, float r) : Primitive(x, y, z), radius(r) {
		std::cout << "Created a sphere at: " << x << ", " << y << ", " << z << ";" << std::endl << "with radius: " << r << std::endl;
	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
