#ifndef MIRO_MATERIAL_H_INCLUDED
#define MIRO_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"

class Material
{
public:
	Material();
	virtual ~Material();

	virtual void preCalc() {}

	virtual void setColor(Vector3 color) {m_color = color;}
	virtual Vector3 getColor() const {return m_color;}
	virtual void setReflect(bool reflect) {m_reflect = reflect;}
	virtual bool getReflect() const {return m_reflect;}
	virtual void setRefract(bool refract) {m_refract = refract;}
	virtual bool getRefract() const {return m_refract;}
	virtual void setGlossy(bool glossy) {m_glossy = glossy;}
	virtual bool getGlossy() const {return m_glossy;}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const;

protected:
	Vector3 m_color;
	bool m_reflect;
	bool m_refract;
	bool m_glossy;
};

#endif // MIRO_MATERIAL_H_INCLUDED
