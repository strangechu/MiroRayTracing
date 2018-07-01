#include "Material.h"

Material::Material()
{
	m_color = Vector3(1.0f);
	m_reflect = false;
	m_refract = false;
}

Material::~Material()
{
}

Vector3
Material::shade(const Ray&, const HitInfo&, const Scene&) const
{
	return Vector3(1.0f, 1.0f, 1.0f);
}
