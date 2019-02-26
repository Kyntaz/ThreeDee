#pragma once

// TODO: Usar cenas do Afonso :3
struct Vector3 {
};

struct _ray {
	Vector3 origin, versor;
	float maxDistance;
};
typedef struct _ray Ray;