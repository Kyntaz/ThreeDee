#pragma once

#include "ray.h"
#include "vector3.h"
#include "materialproperties.h"
#include "aabb.h"
#include "utils.h"

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
	AABB aabb;
	Primitive( MaterialProperties *matProps) {
		_matProps = matProps;
	}
	virtual Collision intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
public:
	float _radius;
	Vector3 _pos;
	Sphere(Vector3 pos, float r, MaterialProperties *matProps) : Primitive(matProps), _pos(pos), _radius(r) {
		aabb = { {pos.x - r - EPSILON, pos.y - r - EPSILON, pos.z - r - EPSILON},
		{pos.x + r + EPSILON, pos.y + r + EPSILON, pos.z + r + EPSILON} };
	
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
			//o raio aponta no sentido contrario ao centro da esfera 
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
		col.inside = false;
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
		aabb = {
			//pmin = min todas as componentes de todos os vertices
			{min(_vertices[0].x, _vertices[1]. x,_vertices[2].x),     //minx
				min(_vertices[0].y, _vertices[1].y, _vertices[2].y),  //miny
				min(_vertices[0].z, _vertices[1].z, _vertices[2].z)}, //minz
			//pmax = max todas as componentes de todos os vertices	 
			{max(_vertices[0].x, _vertices[1].x, _vertices[2].x),     //maxx
				max(_vertices[0].y, _vertices[1].y, _vertices[2].y),  //maxy
				max(_vertices[0].z, _vertices[1].z, _vertices[2].z)}  //maxz
		};

	}

	Collision intersect(Ray ray) {
		Collision col;
		col.inside = false;

		// I tried to make this look like the algorithm in the book for simplicity.
		//Tomas Möller, Ben Trumbore
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

class AxisAlignedBoundingBox : public Primitive {
public:
	Vector3 _p0, _p1;

	AxisAlignedBoundingBox(Vector3 p0, Vector3 p1, MaterialProperties* matProps) : Primitive(matProps), _p0(p0), _p1(p1) {}

	Collision intersect(Ray ray) {
		Collision col;
		col.object = nullptr;

		float ox = ray.origin.x, oy = ray.origin.y, oz = ray.origin.z;
		float dx = ray.versor.x, dy = ray.versor.y, dz = ray.versor.z;

		col.inside = ox > _p0.x && ox < _p1.x &&
			oy > _p0.y && oy < _p1.y &&
			oz > _p0.z && oz < _p1.z;

		float tx_min, ty_min, tz_min;
		float tx_max, ty_max, tz_max;

		float a = 1.0f / dx;
		if (a >= 0) {
			tx_min = (_p0.x - ox) * a;
			tx_max = (_p1.x - ox) * a;
		}
		else {
			tx_min = (_p1.x - ox) * a;
			tx_max = (_p0.x - ox) * a;
		}

		float b = 1.0f / dy;
		if (b >= 0) {
			ty_min = (_p0.y - oy) * b;
			ty_max = (_p1.y - oy) * b;
		}
		else {
			ty_min = (_p1.y - oy) * b;
			ty_max = (_p0.y - oy) * b;
		}

		float c = 1.0f / dz;
		if (c >= 0) {
			tz_min = (_p0.z - oz) * c;
			tz_max = (_p1.z - oz) * c;
		}
		else {
			tz_min = (_p1.z - oz) * c;
			tz_max = (_p0.z - oz) * c;
		}

		float t0, t1;
		
		// find largest entering t val
		if (tx_min > ty_min) {
			t0 = tx_min;
			col.normal = { 0, 0, 0 };
			col.normal.x = (ray.versor.x > 0) ? -1 : 1;
		}
		else {
			t0 = ty_min;
			col.normal = { 0, 0, 0 };
			col.normal.y = ray.versor.y > 0 ? -1 : 1;
		}

		if (tz_min > t0) {
			t0 = tz_min;
			col.normal = { 0, 0, 0 };
			col.normal.z = ray.versor.z > 0 ? -1 : 1;
		}

		// find smallest exiting t val
		if (tx_max < ty_max) {
			t1 = tx_max;
			if (t1 <= t0) {
				col.normal = { 0, 0, 0 };
				col.normal.x = (ray.versor.x > 0) ? -1 : 1;
			}
		}
		else {
			t1 = ty_max;
			if (t1 <= t0) {
				col.normal = { 0, 0, 0 };
				col.normal.y = (ray.versor.y > 0) ? -1 : 1;
			}
		}

		if (tz_max < t1) {
			t1 = tz_max;
			if (t1 <= t0) {
				col.normal = { 0, 0, 0 };
				col.normal.z = (ray.versor.z > 0) ? -1 : 1;
			}
		}

		float t = (col.inside) ? t1 : t0;

		if (!(t0 < t1 && t1 > 0) || t < 0) {
			return col;
		}

		col.object = this;
		col.point = addVector(ray.origin, vector3MultScalar(ray.versor, t));

		return col;
	}
};