#ifndef MIRO_ACCEL_H_INCLUDED
#define MIRO_ACCEL_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "AABB.h"
#include <vector>

struct BVHNode
{
public:
	AABB aabb;
	int left;
	int right;
	BVHNode()
	{
		left = -1;
		right = -1;
	}
	Objects objects;
};

class Accel
{
public:
	Accel () { m_BVHNodes = std::vector<BVHNode>(100000, BVHNode()); }

	void debug_draw_aabb(AABB aabb);
	void tree_debug(int node_index);
	AABB mergeAABB (AABB a, AABB b);
	AABB createAABBfromObjects (Objects objects);
	void sortObjectsByAxis (Objects& objects, int axis);
	void constructBVH_internal (Objects objects, int node_index);

	void build(Objects * objs);
	bool intersectAABB(AABB aabb, const Ray& ray) const;
	Objects traverse(int node_index, const Ray& ray) const;
	bool intersect(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) const;

protected:
	Objects * m_objects;
	std::vector<BVHNode> m_BVHNodes;
	int m_nodeCount;
	int m_leafNodeCount;
	int m_rayCount;
	int m_rayTraversalCount;
	int m_rayIntersectionCount;
};

#endif // MIRO_ACCEL_H_INCLUDED
