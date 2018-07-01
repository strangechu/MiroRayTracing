#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "PFMLoader.h"
#include <time.h>

Scene * g_scene = 0;

extern long long g_numberOfRayTriangleIntersection;
extern long long g_numberOfRayTraversal;

void
Scene::openGL(Camera *cam)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam->drawGL();

	// draw objects
	for (size_t i = 0; i < m_objects.size(); ++i) 
	{
		m_objects[i]->renderGL();
	}

	glutSwapBuffers();
}

void
Scene::preCalc()
{
	Objects::iterator it;
	for (it = m_objects.begin(); it != m_objects.end(); it++)
	{
		Object* pObject = *it;
		pObject->preCalc();
	}
	Lights::iterator lit;
	for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
	{
		PointLight* pLight = *lit;
		pLight->preCalc();
	}

	m_accel.build(&m_objects);
}

void
Scene::raytraceImage(Camera *cam, Image *img)
{
	Ray ray;
	HitInfo hitInfo;
	Vector3 shadeResult;

	size_t timeElapsed = time(0);
	long long numberOfRays = 0;

//test pfm
/*
int* w = new int(640);
int* h = new int(640);
Vector3* pfm_img = readPFMImage("kitchen_probe.pfm", w, h);
for (int i = 0; i < img->height(); ++i)
{
	for (int j = 0; j < img->width(); ++j)
	{
		img->setPixel(i, j, pfm_img[i][j]);
		debug("<%f>", pfm_img[i*640+j].x);
	}
}
return;
*/
	// loop over all pixels in the image
	for (int j = 0; j < img->height(); ++j)
	{
		for (int i = 0; i < img->width(); ++i)
		{
			ray = cam->eyeRay(i, j, img->width(), img->height(), true);
			numberOfRays++;
			if (trace(hitInfo, ray))
			{
				// test shadow occlusion
				Ray shadowRay;
				HitInfo shadowHitInfo;
				shadowRay.o = hitInfo.P + hitInfo.N * 0.01;
				shadowRay.d = m_lights[0]->position() - hitInfo.P;
				bool isShadow = trace(shadowHitInfo, shadowRay);
				// shade color
				if (!isShadow){
					shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
				}
				else{
					shadeResult = Vector3(0.0, 0.0, 0.0);
				}
				img->setPixel(i, j, shadeResult);
			}

// dof sample
/*
			shadeResult = Vector3(0.0f);
			for (int k = 0; k < 4; k++)
			{

			ray = cam->eyeRay(i, j, img->width(), img->height(), true);
			numberOfRays++;
			if (trace(hitInfo, ray))
			{
				// test shadow occlusion
				Ray shadowRay;
				HitInfo shadowHitInfo;
				shadowRay.o = hitInfo.P + hitInfo.N * 0.01;
				shadowRay.d = m_lights[0]->position() - hitInfo.P;
				bool isShadow = trace(shadowHitInfo, shadowRay);
				// shade color
				if (!isShadow){
					shadeResult += hitInfo.material->shade(ray, hitInfo, *this);
				}
				else{
					shadeResult += Vector3(0.0, 0.0, 0.0);
				}
			}

			}
			img->setPixel(i, j, shadeResult);
*/
// dof sample
		}
		img->drawScanline(j);
		glFinish();
		//printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
		//fflush(stdout);
	}

	printf("Rendering Progress: 100.000%\n");
	debug("done Raytracing!\n");
	timeElapsed = time(0) - timeElapsed;
	debug("Time elapsed : %d sec\n", timeElapsed);
	debug("Number of rays : [%lld]\n", numberOfRays);
	debug("Number of ray-traversals : [%lld]\n", g_numberOfRayTraversal);
	debug("Number of ray-triangle intersections : [%lld]\n", g_numberOfRayTriangleIntersection);
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
	return m_accel.intersect(minHit, ray, tMin, tMax);
}
