#ifndef PFM_LOADER_H_INCLUDED
#define PFM_LOADER_H_INCLUDED

#include "Vector3.h"

Vector3* readPFMImage(const char * filename, int * width, int * height);

#endif
