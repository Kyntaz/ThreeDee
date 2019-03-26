#pragma once
#include <stdlib.h>
float generateRandom(float minValue, float maxValue) {
	return minValue + (maxValue - minValue) * ((float)rand() / RAND_MAX);
}
