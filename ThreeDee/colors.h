#pragma once

struct _color {
	float r, g, b;
};
typedef struct _color Color;

Color addColors(Color c1, Color c2) {
	return { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
}

Color colorTimesConstant(Color c, float f) {
	return { c.r * f, c.g * f, c.b * f };
}
