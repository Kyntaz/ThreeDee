#pragma once
#include "vector3.h"
class Camera
{
	Camera() {}
	
public:
	Camera(Vector3 from, Vector3 at, Vector3 up, float angle, float hither, int resX, int resY) :
		eye(from), at(at), up(up), angle(angle), nearC(hither), resX(resX), resY(resY) {
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

private:
	Vector3 eye, at, up;
	float angle, nearC;
	int resX, resY;
};