#pragma once

#include "ray.h"
#include "materialproperties.h"
#include <iostream>
struct _collision {};
typedef struct _collision Collision;

class Primitive {
public:
	MaterialProperties* _matProps;
	Primitive( MaterialProperties *matProps) {
		_matProps = matProps;
	}
	virtual Collision intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
public:
	float _radius;
	Vector3 _pos;

	Sphere(Vector3 pos, float r, MaterialProperties *matProps) : Primitive( matProps), _pos(pos), _radius(r) {

	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
class Cilinder : public Primitive {
public:
	float _baseRadius, _apexRadius;
	Vector3 _basePos, _apexPos;

	Cilinder(Vector3 basePos, float baseR, Vector3 apexPos, float apexR, MaterialProperties* matProps) :
		Primitive( matProps), _basePos(basePos), _apexPos(apexPos), _baseRadius(baseR), _apexRadius(apexR) {

	}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
class Plane : public Primitive {
public:
	Vector3 _p1, _p2, _p3;

	Plane(Vector3 p1, Vector3 p2, Vector3 p3,MaterialProperties* matProps) :
		Primitive(matProps), _p1(p1), _p2(p2), _p3(p3) {}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};
class Poligon : public Primitive {
public:
	std::vector<Vector3> _vertices;

	Poligon(std::vector<Vector3> vtxs, MaterialProperties* matProps) :
		Primitive(matProps), _vertices(vtxs) {}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};

class PoligonalPatch : public Poligon {
public:
	std::vector<Vector3> _normals;

	PoligonalPatch(std::vector<Vector3> vtxs, std::vector<Vector3> normals, MaterialProperties* matProps) :
		Poligon(vtxs,matProps), _normals(normals) {}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		return {};
	};
};