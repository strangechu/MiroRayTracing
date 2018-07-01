#include "Accel.h"
#include "Ray.h"
#include "Console.h"
#include "Cube.h"
#include "Scene.h"
#include "AABB.h"
#include <algorithm>
#include <vector>
#include <deque>
#include <time.h>

long long g_numberOfRayTraversal = 0;
long long g_numberOfRayTriangleIntersection = 0;

#define COST_OBJECT	1.0f
#define COST_AABB	1.0f

AABB Accel::mergeAABB(AABB a, AABB b)
{
	AABB result;
	result.min.x = (a.min.x < b.min.x) ? a.min.x : b.min.x;
	result.min.y = (a.min.y < b.min.y) ? a.min.y : b.min.y;
	result.min.z = (a.min.z < b.min.z) ? a.min.z : b.min.z;
	result.max.x = (a.max.x > b.max.x) ? a.max.x : b.max.x;
	result.max.y = (a.max.y > b.max.y) ? a.max.y : b.max.y;
	result.max.z = (a.max.z > b.max.z) ? a.max.z : b.max.z;
	return result;
}

AABB Accel::createAABBfromObjects(Objects objects)
{
	AABB result;
	for (size_t i = 0; i < objects.size(); ++i)
	{
		AABB aabb = objects[i]->getAABB();
		result = mergeAABB(result, aabb);
	}
	return result;
}

bool sorter_x (const Object* a, const Object* b) {
	return a->getAABB().center(0) < b->getAABB().center(0);
}

bool sorter_y (const Object* a, const Object* b) {
	return a->getAABB().center(1) < b->getAABB().center(1);
}

bool sorter_z (const Object* a, const Object* b) {
	return a->getAABB().center(2) < b->getAABB().center(2);
}

void Accel::sortObjectsByAxis (Objects& objects, int axis)
{
	if (axis == 0)
		sort(objects.begin(), objects.end(), sorter_x);
	else if (axis == 1)
		sort(objects.begin(), objects.end(), sorter_y);
	else if (axis == 2)
		sort(objects.begin(), objects.end(), sorter_z);
}

void Accel::constructBVH_internal(Objects objects, int node_index)
{
	BVHNode new_node;
	//while (m_BVHNodes.size() <= node_index + 1)
	//	m_BVHNodes.push_back(new_node);
	BVHNode& node = m_BVHNodes[node_index];
	node.aabb = createAABBfromObjects(objects);
	// worst case - intersect with all objects
	float best_cost = COST_OBJECT * objects.size();
	int best_axis = -1; // 0:x 1:y 2:z
	int best_split_index = -1;
	float root_surface_area = node.aabb.surface_area();
	for (int axis = 0; axis < 3; ++axis)
	{
		sortObjectsByAxis (objects, axis);
	
		// split
		std::deque<Object*> s1;
		std::deque<Object*> s2;
		for (Objects::iterator it = objects.begin(); it != objects.end(); ++it)
			s2.push_back(*it);
		AABB s1_aabb;
		AABB s2_aabb;

		std::vector<float> s1_sa(objects.size() + 1, MIRO_TMAX), s2_sa(objects.size() + 1, MIRO_TMAX);

		for (int i = 0; i < objects.size(); ++i)
		{
			s1_sa[i] = s1_aabb.surface_area();
			if (s2.size() > 0) {
				// move min of s2 to max of s1
				Object* object = s2.front();
				s1.push_back(object);
				s2.pop_front();
				s1_aabb = mergeAABB(s1_aabb, object->getAABB());
			}
		}

		for (int i = objects.size(); i >= 0; --i)
		{
			s2_sa[i] = s2_aabb.surface_area();
			if (s1.size() > 0 && s2.size() > 0)
			{
				// calculate cost
				float cost = 2 * COST_AABB + (s1_sa[i] * s1.size() + s2_sa[i] * s2.size()) * COST_OBJECT / root_surface_area;
				// update the best cost
				if (cost < best_cost)
				{
					best_cost = cost;
					best_axis = axis;
					best_split_index = i;
				}
			}

			if (s1.size() > 0)
			{
				// move max of s1 to min of s2
				Object* object = s1.back();
				s2.push_front(object);
				s1.pop_back();
				s2_aabb = mergeAABB(s2_aabb, object->getAABB());
			}
		}
	}
	if (best_axis == -1)
	{
		// leaf node
		node.left = node.right = -1;
		node.objects = objects;
		m_leafNodeCount++;
		//debug("node [%d] leaf, size = %d\n", node_index, objects.size());
	}
	else
	{
		sortObjectsByAxis(objects, best_axis);

		// not leaf node, make leaf nodes
		node.left = m_nodeCount; m_nodeCount++;
		node.right = m_nodeCount; m_nodeCount++;

		Objects left_objects(objects.begin(), objects.begin() + best_split_index);
		Objects right_objects(objects.begin() + best_split_index, objects.end());

		//debug("node [%d] split, left = %d, right = %d\n",node_index, node->left, node->right);
		// recursion
		constructBVH_internal (left_objects, node.left);
		constructBVH_internal (right_objects, node.right);
	}
}


void Accel::debug_draw_aabb(AABB aabb)
{
	Cube* c = new Cube(aabb.min, aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y, aabb.max.z - aabb.min.z);
	g_scene->addObject(c);
}

void Accel::tree_debug(int node_index)
{
	BVHNode node = m_BVHNodes[node_index];
	debug_draw_aabb(node.aabb);
	if (node.left != -1)
	{
		tree_debug(node.left);
		tree_debug(node.right);
	}
}

void
Accel::build(Objects * objs)
{
	m_objects = objs;
	
	// construct the bounding volume hierarchy
	m_nodeCount = 1;
	m_leafNodeCount = 0;
	debug("starting constructing BVH...\n");
	size_t timeElapsed = time(0);
	constructBVH_internal(*m_objects, 0);
	timeElapsed = time(0) - timeElapsed;
	debug("done constructing BVH, number of node is [%d], number of leaf node is [%d], construction time is [%d]\n", m_nodeCount, m_leafNodeCount, timeElapsed);
	//tree_debug(0);
}

bool Accel::intersectAABB(AABB aabb, const Ray& ray) const
{
	float t_max = MIRO_TMAX;
	float t_min = -MIRO_TMAX;

	float t1, t2, t_near, t_far;

	t1 = (aabb.min.x - ray.o.x) / ray.d.x;
	t2 = (aabb.max.x - ray.o.x) / ray.d.x;
	t_near = (t1 < t2) ? t1 : t2;
	t_far = (t1 > t2) ? t1 : t2;
	t_max = (t_max < t_far) ? t_max : t_far;
	t_min = (t_min > t_near) ? t_min : t_near;
	if (t_min > t_max) return false;

	t1 = (aabb.min.y - ray.o.y) / ray.d.y;
	t2 = (aabb.max.y - ray.o.y) / ray.d.y;
	t_near = (t1 < t2) ? t1 : t2;
	t_far = (t1 > t2) ? t1 : t2;
	t_max = (t_max < t_far) ? t_max : t_far;
	t_min = (t_min > t_near) ? t_min : t_near;
	if (t_min > t_max) return false;

	t1 = (aabb.min.z - ray.o.z) / ray.d.z;
	t2 = (aabb.max.z - ray.o.z) / ray.d.z;
	t_near = (t1 < t2) ? t1 : t2;
	t_far = (t1 > t2) ? t1 : t2;
	t_max = (t_max < t_far) ? t_max : t_far;
	t_min = (t_min > t_near) ? t_min : t_near;
	if (t_min > t_max) return false;

	return true;
}

void merge_objects(Objects& a, Objects& b)
{
	a.reserve(a.size() + b.size());
	a.insert(a.end(), b.begin(), b.end());
}

Objects Accel::traverse(int node_index, const Ray& ray) const
{
	g_numberOfRayTraversal++;
	//debug("traverse : node index %d\n", node_index);
	Objects result = Objects();
	Objects child_result = Objects();

	BVHNode node = m_BVHNodes[node_index];
	if (intersectAABB(node.aabb, ray))
	{
		// ray intersects with this AABB
		if (node.left != -1) // not leaf node
		{
			child_result = traverse(node.left, ray);
			if (!child_result.empty())
			merge_objects(result, child_result);
			{
				//debug("left\n");
			}
			result = traverse(node.right, ray);
			merge_objects(result, child_result);
			if (!result.empty())
			{
				//debug("right\n");
			}
		}
		else
		{
			//debug("found, size %d\n",node.objects.size());
			return node.objects;
		}
	}
	else
	{
		//debug("not found\n");
		return result;
	}
	merge_objects(result, child_result);
	return result;
}

bool
Accel::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
	// Here you would need to traverse the acceleration data structure to perform ray-intersection
	// acceleration. For now we just intersect every object.

	bool hit = false;
	HitInfo tempMinHit;
	minHit.t = MIRO_TMAX;

	// traverse the BVH tree
	Objects objects = traverse(0, ray);

	for (size_t i = 0; i < objects.size(); ++i)
	{
		g_numberOfRayTriangleIntersection++;
		if (objects[i]->intersect(tempMinHit, ray, tMin, tMax))
		{
			hit = true;
			if (tempMinHit.t < minHit.t)
				minHit = tempMinHit;
		}
	}
/*
	for (size_t i = 0; i < m_objects->size(); ++i)
	{
		g_numberOfRayTriangleIntersection++;
		if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
		{
			hit = true;
			if (tempMinHit.t < minHit.t)
				minHit = tempMinHit;
		}
	}
*/
	return hit;
}
