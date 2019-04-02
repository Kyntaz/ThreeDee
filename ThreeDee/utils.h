#pragma once
#include <stdlib.h>
#include <algorithm>
#define EPSILON 0.0002f

float generateRandom(float minValue, float maxValue) {
	return minValue + (maxValue - minValue) * ((float)rand() / RAND_MAX);
}

float clamp(float v, float lo, float hi) {
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

float min(float f1, float f2, float f3) {
	return std::min(std::min(f1, f2), f3);
}

float max(float f1, float f2, float f3) {
	return std::max(std::max(f1, f2), f3);
}

float getMaxFloat() {
	return std::numeric_limits<float>::max();
}
float getMinFloat() {
	return std::numeric_limits<float>::min();
}