#include "Vector3.h"
#include "Object.h"

class Cube : public Object
{
public:
	Cube();
	Cube(Vector3 o, float x, float y, float z);
	virtual ~Cube();

	virtual void renderGL();
	virtual bool intersect(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX);

protected:
	Vector3 m_origin;
	float m_xlength;
	float m_ylength;
	float m_zlength;
	Vector3 v[8];
};
