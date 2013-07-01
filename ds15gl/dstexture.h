#ifndef _DSTEXTURE_H
#define _DSTEXTURE_H

#include <GL/glut.h>
#include <fstream>

using std::ifstream;
GLuint dsLoadTextureBMP2D(const char* file_name);

#endif