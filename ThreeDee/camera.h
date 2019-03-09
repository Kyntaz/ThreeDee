#pragma once
#include "vector3.h"
class Camera
{
	Camera() {}
	
public:
	Camera(Vector3 from, Vector3 at, Vector3 up, float angle, float hither, int resX, int resY) :
		from(from), at(at), up(up), angle(angle), hither(hither), resX(resX), resY(resY) {}
	int GetResX() {
		return resX;
	}
	int GetResY() {
		return resY;
	}

private:
	Vector3 from, at, up;
	float angle, hither;
	int resX, resY;
};