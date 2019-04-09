#pragma once
#include "vector3.h"
#include <algorithm>

class Camera
{
	Camera() {}
	
public:
	float lens_radius = 0;
	float focal_distance = 1;

	std::vector<Vector3> samples;

	Camera(Vector3 from, Vector3 at, Vector3 up, float angle, float hither, int resX, int resY, float focus, float rad) :
		eye(from), at(at), up(up), angle(angle), nearC(hither), resX(resX), resY(resY), lens_radius(rad), focal_distance(focus) {
		nearC = vector3Length(subVector(at, from));
	}
	int GetResX() {
		return resX;
	}
	int GetResY() {
		return resY;
	}

	Vector3 GetFrom() {
		return eye;
	}

	Vector3 GetAt() {
		return at;
	}

	float GetHither() {
		return nearC;
	}

	Vector3 GetRefY() {
		return externalProduct(GetRefZ(), GetRefX());
	}

	Vector3 GetRefZ() {
		return vector3MultScalar(normalize(subVector(at, eye)), -1);
	}

	Vector3 GetRefX() {
		return normalize(externalProduct(up, GetRefZ()));
	}

	float GetPlaneHeight() {
		return 2 * nearC * tan(angle / 2);
	}

	float GetPlaneWidth() {
		return (GetResX() / GetResY()) * GetPlaneHeight();
	}

	void GenerateSamples(int n) {
		samples = std::vector<Vector3>();

		for (int i = 0; i < n*n; i++) {
			// Rejection Sampling:
			float xx = generateRandom(-lens_radius, lens_radius);
			float yy = generateRandom(-lens_radius, lens_radius);
			while (xx*xx + yy*yy > lens_radius * lens_radius) {
				xx = generateRandom(-lens_radius, lens_radius);
				yy = generateRandom(-lens_radius, lens_radius);
			}

			
			samples.push_back({ xx, yy, 0 });
		}

		std::random_shuffle(samples.begin(), samples.end());
	}

	Vector3 GetSample() {
		return samples[idx = (idx + 1) % samples.size()];
	}

	Vector3 GetPointInFocalPlane(Vector3 ps) {
		float px = ps.x * (focal_distance / nearC);
		float py = ps.y * (focal_distance / nearC);
		return { px, py, -focal_distance };
	}

private:
	Vector3 eye, at, up;
	float angle, nearC;
	int resX, resY;
	int idx = 0;
};