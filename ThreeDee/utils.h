#pragma once
#include <stdlib.h>
#define EPSILON 0.0002f

float generateRandom(float minValue, float maxValue) {
	return minValue + (maxValue - minValue) * ((float)rand() / RAND_MAX);
}

float clamp(float v, float lo, float hi) {
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}