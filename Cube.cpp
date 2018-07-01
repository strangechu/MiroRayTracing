#include "Cube.h"
#include "Matrix3x3.h"

Cube::Cube(Vector3 o, float x, float y, float z) :
	m_origin(o), m_xlength(x), m_ylength(y), m_zlength(z)
{
	v[0] = o;
	v[1] = o; v[1].x += x;
	v[2] = o; v[2].y += y;
	v[3] = o; v[3].x += x; v[3].y += y;
	v[4] = o; v[4].z += z;
	v[5] = o; v[5].x += x; v[5].z += z;
	v[6] = o; v[6].y += y; v[6].z += z;
	v[7] = o; v[7].x += x; v[7].y += y; v[7].z += z;
}


Cube::~Cube()
{
}


void Cube::renderGL()
{
	// rear
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);
		glVertex3f(v[3].x, v[3].y, v[3].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();

	// left
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
		glVertex3f(v[6].x, v[6].y, v[6].z);
		glVertex3f(v[4].x, v[4].y, v[4].z);
	glEnd();

	// right
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[1].x, v[1].y, v[1].z);
		glVertex3f(v[3].x, v[3].y, v[3].z);
		glVertex3f(v[7].x, v[7].y, v[7].z);
		glVertex3f(v[5].x, v[5].y, v[5].z);
	glEnd();

	// top
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[2].x, v[2].y, v[2].z);
		glVertex3f(v[3].x, v[3].y, v[3].z);
		glVertex3f(v[7].x, v[7].y, v[7].z);
		glVertex3f(v[6].x, v[6].y, v[6].z);
	glEnd();

	// bottom
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);
		glVertex3f(v[5].x, v[5].y, v[5].z);
		glVertex3f(v[4].x, v[4].y, v[4].z);
	glEnd();

	// far
	glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(v[4].x, v[4].y, v[4].z);
		glVertex3f(v[5].x, v[5].y, v[5].z);
		glVertex3f(v[7].x, v[7].y, v[7].z);
		glVertex3f(v[6].x, v[6].y, v[6].z);
	glEnd();
}

bool Cube::intersect(HitInfo& result, const Ray& ray,float tMin, float tMax)
{
	return false;
}
