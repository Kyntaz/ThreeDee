#pragma once

#include "ray.h"
#include "vector3.h"
#include "materialproperties.h"
#include <math.h>
#include <iostream>

struct _collision {
	Primitive* object;
	Vector3 point, normal;
};
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
		Collision col;

		float d_oc_sqr = (_pos.x - ray.origin.x) * (_pos.x * ray.origin.x) +
			(_pos.y - ray.origin.y) * (_pos.y * ray.origin.y) +
			(_pos.z - ray.origin.z) * (_pos.z * ray.origin.z);

		float b = ray.versor.x * (_pos.x - ray.origin.x) +
			ray.versor.y * (_pos.y - ray.origin.y) +
			ray.versor.z * (_pos.z - ray.origin.z);

		if (d_oc_sqr > _radius * _radius && b < 0) {
			col.object = nullptr;
			return col;
		}

		float r = b * b - d_oc_sqr + _radius * _radius;
		if (r < 0) {
			col.object = nullptr;
			return col;
		}

		float ti = (d_oc_sqr > _radius * _radius) ? (b - sqrt(r)) : (b + sqrt(r));
		col.point = addVector(ray.origin, vector3MultScalar(ray.versor, ti));
		col.normal = subVector(col.point, _pos);
		col.object = this;

		if (d_oc_sqr < _radius * _radius) {
			col.normal = vector3MultScalar(col.normal, -1);
		}

		return col;
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