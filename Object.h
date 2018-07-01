#ifndef MIRO_OBJECT_H_INCLUDED
#define MIRO_OBJECT_H_INCLUDED

#include <vector>
#include "Vector3.h"
#include "Miro.h"
#include "Material.h"
#include "AABB.h"

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	void setMaterial(const Material* m) {m_material = m;}

	virtual void renderGL() {}
	virtual void preCalc() {}
	inline AABB getAABB() const { return m_AABB; }

	virtual bool intersect(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) = 0;

protected:
	const Material* m_material;
	AABB m_AABB;
};

typedef std::vector<Object*> Objects;

#endif // MIRO_OBJECT_H_INCLUDED
