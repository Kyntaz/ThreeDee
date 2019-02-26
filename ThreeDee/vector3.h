#pragma once
#include <math.h> 
struct _vector
{
	float x, y, z;
};
typedef struct _vector Vector3;

Vector3 addVector(Vector3 v1, Vector3 v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 subVector(Vector3 v1, Vector3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

float vector3Length(Vector3 v1) {
	return sqrt((v1.x)*(v1.x) + (v1.y)*(v1.y) + (v1.z)*(v1.z));
}

Vector3 normalize(Vector3 v1) {
	float size = vector3Length(v1);
	return {v1.x / size, v1.y / size, v1.z / size};
}

float internalProduct(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
}

Vector3 externalProduct(Vector3 v1, Vector3 v2) {
	return {v1.y * v2.z - v1.z * v2.y, - v1.x * v2.z + v1.z * v2.x, v1.x * v2.y - v1.y * v2.x};
}