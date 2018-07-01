#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>
#include "Console.h"

Lambert::Lambert(const Vector3 & kd) :
	m_kd(kd)
{
	m_ks = Vector3(1.0f);
	m_kt = Vector3(1.0f);
	m_color = Vector3(1.0f);
	m_reflect = false;
	m_refract = false;
	m_glossy = false;
}

Lambert::~Lambert()
{
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

	const Vector3 viewDir = -ray.d; // d is a unit vector

	const Lights *lightlist = scene.lights();

	// loop over all of the lights
	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
	{
		PointLight* pLight = *lightIter;

		Vector3 l = pLight->position() - hit.P;
		Vector3 n = hit.N;

		// the inverse-squared falloff
		float falloff = l.length2();

		// normalize the light direction
		l /= sqrt(falloff);

		if (m_refract)
		{
			Vector3 w = ray.d;
			float ratio = 0.5;
			Vector3 r = ratio * (w - dot(w, n) * n) - sqrt(1 - ratio * ratio * (1 - dot(w, n) * dot(w,n))) * n;
			HitInfo refractHitInfo;
			Ray refractRay;
			refractRay.o = hit.P - 0.01 * n;
			refractRay.d = r;
			if (scene.trace(refractHitInfo, refractRay))
			{
				return m_kt * refractHitInfo.material->shade(refractRay, refractHitInfo, scene);
			}
			else
			{
				L = Vector3(0.0f);
			}
		}
		else if (m_reflect)
		{
			Vector3 r = 2.0f * dot(viewDir, n) * n - viewDir;
			HitInfo reflectHitInfo;
			Ray reflectRay;
			reflectRay.o = hit.P + 0.01 * n;
			reflectRay.d = r;
			if (scene.trace(reflectHitInfo, reflectRay))
			{
				return m_ks * reflectHitInfo.material->shade(reflectRay, reflectHitInfo, scene);
			}
			else
			{
				L = Vector3(0.0f);
			}
		}
		else
		{
			Vector3 n = hit.N;
			// get the irradiance
			if (m_glossy)
			{
				//
			}
			Vector3 irradiance = (pLight->color() * pLight->wattage()) * std::max(0.0f, dot(n, l)) / (4.0 * PI * falloff);
			L += irradiance * (m_kd / PI) * m_color;
		}
	}

	return L;
}
