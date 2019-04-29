#pragma once
#include "vector3.h"
#include "utils.h"
#include "aabb.h"
#include "primitives.h"
#include <limits>
#include <math.h>



struct _traverseInfo {
	bool collision;
	float t0, t1;
	Vector3 tmax, tmin;
};
typedef struct _traverseInfo TraverseInfo;

bool isIntersecting(AABB a, AABB b) {
	return (a.pmin.x <= b.pmax.x && a.pmax.x >= b.pmin.x) &&
		(a.pmin.x <= b.pmax.y && a.pmax.y >= b.pmin.y) &&
		(a.pmin.z <= b.pmax.z && a.pmax.z >= b.pmin.z);
}

bool isPointInsideAABB(Vector3 p, AABB aabb) {
	float p0x = aabb.pmin.x, p0y = aabb.pmin.y, p0z = aabb.pmin.z;
	float p1x = aabb.pmax.x, p1y = aabb.pmax.x, p1z = aabb.pmax.x;
	return p.x  > p0x && p.x < p1x &&
		p.y > p0y && p.y < p1y &&
		p.z > p0z && p.z < p1z;
}

TraverseInfo RayAABBIntersection(AABB aabb, Ray ray) {
	

	float ox = ray.origin.x, oy = ray.origin.y, oz = ray.origin.z;
	float dx = ray.versor.x, dy = ray.versor.y, dz = ray.versor.z;

	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;

	float a = 1.0f / dx;
	if (a >= 0) {
		tx_min = (aabb.pmin.x - ox) * a;
		tx_max = (aabb.pmax.x - ox) * a;
	}
	else {
		tx_min = (aabb.pmax.x - ox) * a;
		tx_max = (aabb.pmin.x - ox) * a;
	}

	float b = 1.0f / dy;
	if (b >= 0) {
		ty_min = (aabb.pmin.y - oy) * b;
		ty_max = (aabb.pmax.y - oy) * b;
	}
	else {
		ty_min = (aabb.pmax.y - oy) * b;
		ty_max = (aabb.pmin.y - oy) * b;
	}

	float c = 1.0f / dz;
	if (c >= 0) {
		tz_min = (aabb.pmin.z - oz) * c;
		tz_max = (aabb.pmax.z - oz) * c;
	}
	else {
		tz_min = (aabb.pmax.z - oz) * c;
		tz_max = (aabb.pmin.z - oz) * c;
	}

	float t0, t1;

	// find largest entering t val
	if (tx_min > ty_min) {
		t0 = tx_min;
	}
	else {
		t0 = ty_min;
	}

	if (tz_min > t0) {
		t0 = tz_min;
	}

	// find smallest exiting t val
	if (tx_max < ty_max) {
		t1 = tx_max;
	}
	else {
		t1 = ty_max;
	}

	if (tz_max < t1) {
		t1 = tz_max;
	}
	// if tmin > tmax swap tmin with tmax
	if (tx_min > tx_max) {
		tx_min, tx_max = tx_max, tx_min;
	}
	if (ty_min > ty_max) {
		ty_min, ty_max = ty_max, ty_min;
	}
	if (tz_min > tz_max) {
		tz_min, tz_max = tz_max, tz_min;
	}

	TraverseInfo tInfo = TraverseInfo();
	tInfo.collision = (t0 < t1 && t1 > EPSILON);
	tInfo.t0 = t0;
	tInfo.t1 = t1;
	tInfo.tmin = { tx_min,ty_min,tz_min };
	tInfo.tmax = { tx_max,ty_max,tz_max };
	return tInfo;
}

class Grid{
	AABB gridAABB;
	float m = M;
	int Nx, Ny, Nz;

	std::vector<std::vector<Primitive*>> cells;

public:

	int calculateGridIndex(int ix,int iy,int iz) {
		return ix + Nx * iy + Nx * Ny * iz;
	}
	

	Grid(std::vector<Primitive*> primitives) {
		cells = std::vector<std::vector<Primitive*>>();
		AABB objectAABB;
		int nrPrimitives = primitives.size();
		Vector3 pmin = { getMaxFloat(), getMaxFloat(), getMaxFloat() };
		Vector3 pmax = { getMinFloat(), getMinFloat(), getMinFloat() };

		for (int i = 0; i < nrPrimitives; i++) {
			objectAABB = primitives[i]->aabb;
			if (objectAABB.pmin.x < pmin.x) pmin.x = objectAABB.pmin.x;
			if (objectAABB.pmin.y < pmin.y) pmin.y = objectAABB.pmin.y;
			if (objectAABB.pmin.z < pmin.z) pmin.z = objectAABB.pmin.z;
			if (objectAABB.pmax.x > pmax.x) pmax.x = objectAABB.pmax.x;
			if (objectAABB.pmax.y > pmax.y) pmax.y = objectAABB.pmax.y;
			if (objectAABB.pmax.z > pmax.z) pmax.z = objectAABB.pmax.z;
			
		}
		pmin.x -= EPSILON; pmin.y -= EPSILON; pmin.z -= EPSILON;
		pmax.x += EPSILON; pmax.y += EPSILON; pmax.z += EPSILON;
		gridAABB = { pmin, pmax };
		float wx = (pmax.x - pmin.x);
		float wy = (pmax.y - pmin.y);
		float wz = (pmax.z - pmin.z);
		float s = cbrt((wx * wy * wz) / nrPrimitives);
		Nx = floor(m * wx / s) + 1;
		Ny = floor(m * wy / s) + 1;
		Nz = floor(m * wz / s) + 1;
		
		int totalNrCells = Nx * Ny * Nz;
		/*Inicialização das Celulas (1D ARRAY)*/
		for (int i = 0; i < totalNrCells; i++) {
			cells.push_back(std::vector<Primitive*>());
		}
		for (Primitive* p : primitives){
			/* Compute indices of both cells that contain min and max coord of obj bbox*/
			int ixmin = clamp((p->aabb.pmin.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x), 0, Nx - 1);
			int iymin = clamp((p->aabb.pmin.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y), 0, Ny - 1);
			int izmin = clamp((p->aabb.pmin.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z), 0, Nz - 1);
			int ixmax = clamp((p->aabb.pmax.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x), 0, Nx - 1);
			int iymax = clamp((p->aabb.pmax.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y), 0, Ny - 1);
			int izmax = clamp((p->aabb.pmax.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z), 0, Nz - 1);
			/*insert obj to the overlaped cells*/
			
			for (int iz = izmin; iz <= izmax; iz++) {
				for (int iy = iymin; iy <= iymax; iy++) {
					for (int ix = ixmin; ix <= ixmax; ix++) {
						int idx = calculateGridIndex( ix, iy, iz );
						cells[idx].push_back(p);
					}
				}
			}
		}
	}


	Collision traverse(Ray ray) {
		TraverseInfo tInfo = RayAABBIntersection(gridAABB, ray);
		Collision result;
		result.object = nullptr;
		result.inside = false;

		if (!tInfo.collision) {
			return { nullptr,{0, 0, 0},{0, 0, 0},false };
		}
		int ix, iy, iz;

		if (isPointInsideAABB(ray.origin, gridAABB)) {
			ix = clamp((ray.origin.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x), 0, Nx - 1);
			iy = clamp((ray.origin.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y), 0, Ny - 1);
			iz = clamp((ray.origin.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z), 0, Nz - 1);
		}
		else {
			Vector3 p = addVector(ray.origin, vector3MultScalar(ray.versor, tInfo.t0));
			ix = clamp((p.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x), 0, Nx - 1);
			iy = clamp((p.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y), 0, Ny - 1);
			iz = clamp((p.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z), 0, Nz - 1);
		}
		// ray parameter increments per cell in the x, y, and z directions

		float dtx = (tInfo.tmax.x - tInfo.tmin.x) / Nx;
		float dty = (tInfo.tmax.y - tInfo.tmin.y) / Ny;
		float dtz = (tInfo.tmax.z - tInfo.tmin.z) / Nz;
		float tx_next, ty_next, tz_next;
		int ix_step, iy_step, iz_step;
		int ix_stop, iy_stop, iz_stop;

		if (ray.versor.x > 0) {
			tx_next = tInfo.tmin.x + (ix + 1) * dtx;
			ix_step = +1;
			ix_stop = Nx;
		}
		else {
			tx_next = tInfo.tmin.x + (Nx - ix) * dtx;
			ix_step = -1;
			ix_stop = -1;
		}
		if (ray.versor.x == 0.0f) {
			tx_next = getMaxFloat();
			ix_step = -1; // just to initialize. Never used
			ix_stop = -1;
		}

		if (ray.versor.y > 0) {
			ty_next = tInfo.tmin.y + (iy + 1) * dty;
			iy_step = +1;
			iy_stop = Ny;
		}
		else {
			ty_next = tInfo.tmin.y + (Ny - iy) * dty;
			iy_step = -1;
			iy_stop = -1;
		}
		if (ray.versor.y == 0.0f) {
			ty_next = getMaxFloat();
			iy_step = -1; // just to initialize. Never used
			iy_stop = -1;
		}

		if (ray.versor.z > 0) {
			tz_next = tInfo.tmin.z + (iz + 1) * dtz;
			iz_step = +1;
			iz_stop = Nz;
		}
		else {
			tz_next = tInfo.tmin.z + (Nz - iz) * dtz;
			iz_step = -1;
			iz_stop = -1;
		}
		if (ray.versor.z == 0.0f) {
			tz_next = getMaxFloat();
			iz_step = -1; // just to initialize. Never used
			iz_stop = -1;
		}
		while (true) {
			std::vector<Primitive*> pVector = cells[calculateGridIndex(ix, iy, iz)];
			if (tx_next < ty_next && tx_next < tz_next) {
				for (Primitive* p : pVector) {
					Collision c = p->intersect(ray);
					if (c.object != nullptr && vector3Length(subVector(c.point, ray.origin)) < tx_next) {
						if (result.object == nullptr || vector3Length(subVector(c.point, ray.origin)) < vector3Length(subVector(result.point, ray.origin))) {
							result = c;
						}
					}
				}
				if (result.object != nullptr) return result;

				tx_next += dtx;
				ix += ix_step;
				
				if (ix == ix_stop) return result;
			}
			else if (ty_next < tz_next) {
				for (Primitive* p : pVector) {
					Collision c = p->intersect(ray);
					if (c.object != nullptr && vector3Length(subVector(c.point, ray.origin)) < ty_next) {
						if (result.object == nullptr || vector3Length(subVector(c.point, ray.origin)) < vector3Length(subVector(result.point, ray.origin))) {
							result = c;
						}
					}
				}
				if (result.object != nullptr) return result;

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop) return result;
			}
			else {
				for (Primitive* p : pVector) {
					Collision c = p->intersect(ray);
					if (c.object != nullptr && vector3Length(subVector(c.point, ray.origin)) < tz_next) {
						if (result.object == nullptr || vector3Length(subVector(c.point, ray.origin)) < vector3Length(subVector(result.point, ray.origin))) {
							result = c;
						}
					}
				}
				if (result.object != nullptr) return result;

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop) return result;
			}
		}

	}
};