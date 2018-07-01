#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"
#include "Matrix3x3.h"
#include "Console.h"

Triangle::Triangle(TriangleMesh * m, unsigned int i) :
	m_mesh(m), m_index(i)
{
}


Triangle::~Triangle()
{
}


void
Triangle::renderGL()
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}


void Triangle::preCalc()
{
	// bounding box
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = v0.x; ymin = v0.y; zmin = v0.z;
	xmax = v0.x; ymax = v0.y; zmax = v0.z;
	xmin = (xmin < v1.x) ? xmin : v1.x; xmax = (xmax > v1.x) ? xmax : v1.x;
	ymin = (ymin < v1.y) ? ymin : v1.y; ymax = (ymax > v1.y) ? ymax : v1.y;
	zmin = (zmin < v1.z) ? zmin : v1.z; zmax = (zmax > v1.z) ? zmax : v1.z;
	xmin = (xmin < v2.x) ? xmin : v2.x; xmax = (xmax > v2.x) ? xmax : v2.x;
	ymin = (ymin < v2.y) ? ymin : v2.y; ymax = (ymax > v2.y) ? ymax : v2.y;
	zmin = (zmin < v2.z) ? zmin : v2.z; zmax = (zmax > v2.z) ? zmax : v2.z;
	
	m_AABB.min.x = xmin; m_AABB.min.y = ymin; m_AABB.min.z = zmin;
	m_AABB.max.x = xmax; m_AABB.max.y = ymax; m_AABB.max.z = zmax;
}

bool
Triangle::intersect(HitInfo& result, const Ray& ray,float tMin, float tMax)
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & a = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & b = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & c = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	const Vector3 & d = ray.d;
	const Vector3 & o = ray.o;
	
	// solve Ax = y by Cramer's Rule
	Matrix3x3 A(a.x - b.x, a.x - c.x, d.x,
	            a.y - b.y, a.y - c.y, d.y,
	            a.z - b.z, a.z - c.z, d.z);
	Vector3 y(a.x - o.x, a.y - o.y, a.z - o.z);

	float det = A.det();

	if (det == 0)
		return false;

	Matrix3x3 Atemp;
	Atemp = A;
	Atemp.setColumn1(y);
	float detX = Atemp.det();
	Atemp = A;
	Atemp.setColumn2(y);
	float detY = Atemp.det();
	Atemp = A;
	Atemp.setColumn3(y);
	float detZ = Atemp.det();
	float X = detX / det;
	float Y = detY / det;
	float Z = detZ / det;
	float beta = X;
	float gamma = Y;
	float alpha = 1 - X - Y;
	float t = Z;

	if (!(alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1 && t > 0)) {
		return false;
	}

	result.t = t;
	result.P = ray.o + result.t * ray.d;

	Vector3 & an = m_mesh->normals()[ti3.x];
	Vector3 & bn = m_mesh->normals()[ti3.y];
	Vector3 & cn = m_mesh->normals()[ti3.z];
	an.normalize();
	bn.normalize();
	cn.normalize();

	result.N = an * alpha + bn * beta + cn * gamma;
	result.N.normalize();

	result.material = this->m_material;
	
	return true;
}
