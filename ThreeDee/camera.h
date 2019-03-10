#pragma once
#include "vector3.h"
class Camera
{
	Camera() {}
	
public:
	Camera(Vector3 from, Vector3 at, Vector3 up, float angle, float hither, int resX, int resY) :
		eye(from), at(at), up(up), angle(angle), nearC(hither), resX(resX), resY(resY) {}
	int GetResX() {
		return resX;
	}
	int GetResY() {
		return resY;
	}

private:
	Vector3 eye, at, up;
	float angle, nearC;
	int resX, resY;
};