#ifndef MIRO_LAMBERT_H_INCLUDED
#define MIRO_LAMBERT_H_INCLUDED

#include "Material.h"

class Lambert : public Material
{
public:
	Lambert(const Vector3 & kd = Vector3(1));
	virtual ~Lambert();

	const Vector3 & kd() const {return m_kd;}
	const Vector3 & ks() const {return m_ks;}
	const Vector3 & kt() const {return m_kt;}

	void setKd(const Vector3 & kd) {m_kd = kd;}
	void setKs(const Vector3 & ks) {m_ks = ks;}
	void setKt(const Vector3 & kt) {m_kt = kt;}

	virtual void preCalc() {}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const;

protected:
	Vector3 m_kd;
	Vector3 m_ks;
	Vector3 m_kt;
};

#endif // MIRO_LAMBERT_H_INCLUDED
