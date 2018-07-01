#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include "Vector3.h"
#include "Miro.h"

struct AABB {
	Vector3 min;
	Vector3 max;
	AABB()
	{
		reset();
	}
	inline void reset() {
		min = Vector3(MIRO_TMAX, MIRO_TMAX, MIRO_TMAX);
		max = Vector3(-MIRO_TMAX, -MIRO_TMAX, -MIRO_TMAX);
	}
	inline Vector3 center() {
		return (min + max) / 2;
	}
	inline float center(int axis) {
		if (axis == 0)
			return (min.x + max.x) / 2;
		else if (axis == 1)
			return (min.y + max.y) / 2;
		else if (axis == 2)
			return (min.z + max.z) / 2;
		return 0.0f;
	}
	inline float surface_area() {
		float dx = max.x - min.x;
		float dy = max.y - min.y;
		float dz = max.z - min.z;
		return 2 * (dx * dy + dx * dz + dy * dz);
	}
};

#endif
