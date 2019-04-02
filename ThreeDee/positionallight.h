#pragma once

#include <algorithm>

#include "vector3.h"
#include "colors.h"
#include "utils.h"

class Light{
public: 
	Vector3 pos;
	Color color;
	std::vector<Vector3> samples;
	
	Light(Vector3 pos, Color color) :pos(pos), color(color) {}

	virtual void generateSamples(int n) {
		int sqn = n * n;
		samples = std::vector<Vector3>();
		for (int i = 0; i < sqn; i++) {
			samples.push_back(pos);
		}
	}

	Vector3 getLightSample() {
		return samples[idx = (idx + 1) % samples.size()];
	}

	virtual Vector3 getCenter() {
		return pos;
	}

private:
	int idx = 0;
};
class AreaLight : public Light {
public:
	Vector3 p1, p2, p3;

	AreaLight(Vector3 pos, Color color, Vector3 p1, Vector3 p2, Vector3 p3) :
		Light(pos, color), p1(p1), p2(p2), p3(p3) {}

	void generateSamples(int n) {
		samples = std::vector<Vector3>();
		Vector3 v1 = subVector(p2, p1);
		Vector3 v2 = subVector(p3, p1);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				float xx = generateRandom((float)i / n, (float)(i + 1) / n);
				float yy = generateRandom((float)j / n, (float)(j + 1) / n);
				Vector3 sample = addVector(p1, vector3MultScalar(v1, xx));
				sample = addVector(sample, vector3MultScalar(v2, yy));
				samples.push_back(sample);
			}
		}

		std::random_shuffle(samples.begin(), samples.end());
	}

	Vector3 getCenter() {
		Vector3 v1 = subVector(p2, p1);
		Vector3 v2 = subVector(p3, p1);

		Vector3 sample = addVector(p1, vector3MultScalar(v1, 0.5f));
		sample = addVector(sample, vector3MultScalar(v2, 0.5f));
		return sample;
	}
};