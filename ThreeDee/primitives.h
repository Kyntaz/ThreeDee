#pragma once

#include "ray.h"
#include "vector3.h"
#include "materialproperties.h"
#include <math.h>
#include <iostream>

class Primitive; //This is needed so that we can have the object reference in the collision struct.

struct _collision {
	Primitive* object;
	Vector3 point, normal;
	bool inside;
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
		col.inside = false;

		float d_oc_sqr = (_pos.x - ray.origin.x) * (_pos.x - ray.origin.x) +
			(_pos.y - ray.origin.y) * (_pos.y - ray.origin.y) +
			(_pos.z - ray.origin.z) * (_pos.z - ray.origin.z);

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
		col.normal = normalize(subVector(col.point, _pos));
		col.object = this;

		if (d_oc_sqr < _radius * _radius) {
			col.normal = vector3MultScalar(col.normal, -1);
			col.inside = true;
		}

		return col;
	}
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
		Collision col;
		col.object = nullptr;
		return col;
	};
};

class Plane : public Primitive {
public:
	Vector3 _p1, _p2, _p3;
	Vector3 _normal;

	Plane(Vector3 p1, Vector3 p2, Vector3 p3,MaterialProperties* matProps) :
		Primitive(matProps), _p1(p1), _p2(p2), _p3(p3) { 
		_normal = normalize(externalProduct(subVector(_p2, _p1), subVector(_p3, _p1)));
	}

	Collision intersect(Ray ray) {
		Collision col;
		col.object = nullptr;
		col.inside = false;

		float p = internalProduct(_normal,ray.versor);
		if (p == 0) {
			//plane and ray are parallel, thus intersection is not computed
			col.object = nullptr;
			return col;
		}

		float ti = -((internalProduct(subVector(ray.origin,_p1),_normal))/(internalProduct(_normal,ray.versor)));

		if (ti < 0) {
			//intersection is behind the origin of the ray, thus we can reject ti
			col.object = nullptr;
			return col;
		}

		col.point = addVector(ray.origin, vector3MultScalar(ray.versor, ti));
		col.normal = _normal;
		col.object = this;

		return col;
	};
};

// This is actualy a triangle for now.
class Poligon : public Primitive {
public:
	std::vector<Vector3> _vertices;
	Vector3 _normal;

	Poligon(std::vector<Vector3> vtxs, MaterialProperties* matProps) :
		Primitive(matProps), _vertices(vtxs) {
		_normal = normalize(externalProduct(subVector(_vertices[1], _vertices[0]), subVector(_vertices[2], _vertices[0])));
	}

	Collision intersect(Ray ray) {
		Collision col;
		col.inside = false;

		// I tried to make this look like the algorithm in the book for simplicity.
		Vector3 v0 = _vertices[0];
		Vector3 v1 = _vertices[1];
		Vector3 v2 = _vertices[2];

		float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.versor.x, d = v0.x - ray.origin.x;
		float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.versor.y, h = v0.y - ray.origin.y;
		float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.versor.z, l = v0.z - ray.origin.z;

		float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		float q = g * i - e * k, s = e * j - f * i;

		float inv_denom = 1.0f / (a * m + b * q + c * s);

		float e1 = d * m - b * n - c * p;
		float beta = e1 * inv_denom;

		if (beta < 0) {
			col.object = nullptr;
			return col;
		}

		float r = e * l - h * i;
		float e2 = a * n + d * q + c * r;
		float gamma = e2 * inv_denom;

		if (gamma < 0 || beta + gamma > 1) {
			col.object = nullptr;
			return col;
		}

		float e3 = a * p - b * r + d * s;
		float t = e3 * inv_denom;

		if (t < 0) {
			col.object = nullptr;
			return col;
		}

		col.point = addVector(ray.origin, vector3MultScalar(ray.versor, t));
		col.normal = _normal;
		col.object = this;

		return col;
	};
};

class PoligonalPatch : public Poligon {
public:
	std::vector<Vector3> _normals;

	PoligonalPatch(std::vector<Vector3> vtxs, std::vector<Vector3> normals, MaterialProperties* matProps) :
		Poligon(vtxs,matProps), _normals(normals) {}

	Collision intersect(Ray ray) {
		//TODO: make it happen!
		Collision col;
		col.object = nullptr;
		return col;
	};
};