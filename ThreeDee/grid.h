#pragma once
#include "vector3.h"
#include "utils.h"
#include "primitives.h"
#include <limits>
#include <math.h>

#define M 2
struct _AABB {
	Vector3 pmin, pmax;
};
typedef struct _AABB AABB;

bool isIntersecting(AABB a, AABB b) {
	return (a.pmin.x <= b.pmax.x && a.pmax.x >= b.pmin.x) &&
		(a.pmin.x <= b.pmax.y && a.pmax.y >= b.pmin.y) &&
		(a.pmin.z <= b.pmax.z && a.pmax.z >= b.pmin.z);
}

class Grid{
	AABB gridAABB;
	int m = M;
	int Nx, Ny, Nz;

	std::vector<std::vector<Primitive*>> cells;

public:

	int calculateGridIndex(Vector3 i) {
		return i.x + Nx * i.y + Nx * Ny * i.z;
	}


	float maxValue = std::numeric_limits<float>::max();
	float minValue = std::numeric_limits<float>::min();

	Grid(std::vector<Primitive*> primitives) {
		cells = std::vector<std::vector<Primitive*>>();
		AABB objectAABB;
		int nrPrimitives = primitives.size();
		Vector3 pmin = { maxValue, maxValue, maxValue };
		Vector3 pmax = { minValue, minValue, minValue };

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
		
		int totalNrCells = m * m * m * nrPrimitives;
		/*Inicialização das Celulas (1D ARRAY)*/
		for (int i = 0; i < totalNrCells; i++) {
			cells.push_back(std::vector<Primitive*>());
		}
		for (Primitive* p : primitives){
			/* Compute indices of both cells that contain min and max coord of obj bbox*/
			int ixmin = clamp(((p->aabb.pmin.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x)), 0, Nx - 1);
			int iymin = clamp(((p->aabb.pmin.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y)), 0, Ny - 1);
			int izmin = clamp(((p->aabb.pmin.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z)), 0, Nz - 1);
			int ixmax = clamp(((p->aabb.pmax.x - gridAABB.pmin.x) * Nx / (gridAABB.pmax.x - gridAABB.pmin.x)), 0, Nx - 1);
			int iymax = clamp(((p->aabb.pmax.y - gridAABB.pmin.y) * Ny / (gridAABB.pmax.y - gridAABB.pmin.y)), 0, Ny - 1);
			int izmax = clamp(((p->aabb.pmax.z - gridAABB.pmin.z) * Nz / (gridAABB.pmax.z - gridAABB.pmin.z)), 0, Nz - 1);
			/*insert obj to the overlaped cells*/
			
			for (int iz = izmin; iz <= izmax; iz++) {
				for (int iy = iymin; iy <= iymax; iy++) {
					for (int ix = ixmin; ix <= ixmax; ix++) {
						int idx = calculateGridIndex({ ix,iy,iz });
						cells[idx].push_back(p);
					}
				}
			}
		}
	}
};